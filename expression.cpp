#include "expression.h"
#include "config.h"
#include "var.h"
#include "varscope.h"
#include "operator.h"
#include "tokenizer.h"

#include <cmath>
#include <stack>
#include <cctype>

namespace kiva {
    namespace expression {
        using namespace kiva::var;
        using namespace kiva::parser;

        template <typename T>
        static int castInt(const T &t)
        {
            return static_cast<int>(t);
        }

        bool realEquals(Real a, Real b)
        {
            static Real W = 0.000001;
            return std::abs(a - b) <= W;
        }

        void calcNumber2(std::stack<Real> &opds, int opt)
        {
            Real r = opds.top(); opds.pop();
            Real l = opds.top(); opds.pop();
            Real res = 0;

            switch (opt) {
                case ADD:
                    res = l + r;
                    break;
                case SUB:
                    res = l - r;
                    break;
                case MUL:
                    res = l * r;
                    break;
                case DIV:
                    res = l / r;
                    break;
                case XOR:
                    res = castInt(l) ^ castInt(r);
                    break;
                case AND:
                    res = castInt(l) & castInt(r);
                    break;
                case OR:
                    res = castInt(l) | castInt(r);
                    break;
                case LSHF:
                    res = castInt(l) << castInt(r);
                    break;
                case RSHF:
                    res = castInt(l) >> castInt(r);
                    break;
                case MOD:
                    res = castInt(l) % castInt(r);
                    break;
                case LAND:
                    res = castInt(l) && castInt(r);
                    break;
                case LOR:
                    res = castInt(l) || castInt(r);
                    break;
                case LE:
                    res = l <= r;
                    break;
                case LT:
                    res = l < r;
                    break;
                case GE:
                    res = l >= r;
                    break;
                case GT:
                    res = l > r;
                    break;
                case EQ:
                    res = realEquals(l, r);
                    break;
                case NEQ:
                    res = !realEquals(l, r);
                    break;
            }
            opds.push(res);
        }

        void calcNumber(std::stack<Real> &opds, int opt)
        {
            switch (opt) {
                case NAV: {
                    Real n = opds.top(); opds.pop();
                    opds.push(-n);
                    break;
                }
                case NOT: {
                    Real n = opds.top(); opds.pop();
                    opds.push(!castInt(n));
                    break;
                }
                default:
                    calcNumber2(opds, opt);
                    break;
            }
        }

        void calcFinalResult(std::stack<int> &opts, std::stack<Real> &nums, int &resultType)
        {
            while (opts.size() != 0) {
                int opt = opts.top();
                calcNumber(nums, opt);
                opts.pop();

                resultType = Operator::isLogicalOperatorToken(opt)
                             ? RESULT_BOOL
                             : RESULT_NUMBER;
            }
        }

        Var evalDirectly(const String &str) throw(std::runtime_error)
        {
            int i;
            return evalDirectly(str, i);
        }

        Var evalDirectly(const String &str, int &resultType) throw(std::runtime_error)
        {
            Tokenizer tk(str);
            Token t;

            std::stack<Real> nums;
            std::stack<int>  opts;

            Var result;
            resultType = RESULT_NONE;
            String lastId;

            while (tk.next(t)) {
                if (Operator::isOperatorToken(t.token)) {
                    if (opts.size() == 0) {
                        opts.push(t.token);
                    } else {
                        int topOpt = opts.top();
                        int topPriority = Operator::getPriority(topOpt);
                        int priority = Operator::getPriority(t.token);

                        if (priority > topPriority) {
                            opts.push(t.token);
                        } else {
                            while (priority <= topPriority) {
                                opts.pop();
                                calcNumber(nums, topOpt);

                                if (opts.size() > 0) {
                                    topOpt = opts.top();
                                    topPriority = Operator::getPriority(topOpt);
                                } else {
                                    break;
                                }
                            }
                            opts.push(t.token);
                        }
                    }

                } else if (t.token == '(') {
                    opts.push(t.token);

                } else if (t.token == ')') {
                    while (opts.top() != '(') {
                        int opt = opts.top();
                        opts.pop();
                        calcNumber(nums, opt);
                    }
                    opts.pop();

                } else if (t.token == NUMBER) {
                    resultType = RESULT_NUMBER;
                    nums.push(t.numval);

                } else if (t.token == STRING) {
                    resultType = RESULT_STRING;
                    return Var(t.strval);

                } else if (t.token == ID) {
                    lastId = t.strval;
                    if (tk.peekChar() == '(') {
                        // 函数
                        throw std::runtime_error("Function not supported.");

                    } else {
                        // 变量
                        Token peek;
                        if (tk.next(peek, true) && peek.token == ASSIGN) {
                            continue;
                        }

                        VarScope *current = VarScope::getCurrent();
                        Var v = current->getVar(t.strval);
                        if (!v.isValid()) {
                            resultType = RESULT_NIL;
                            return v;
                        }

                        if (v.getType() == typeid(Real)) {
                            nums.push(v.as<Real>());
                            resultType = RESULT_NUMBER;

                        } else if (v.getType() == typeid(String)) {
                            resultType = RESULT_STRING;
                            return v;

                        } else {
                            throw std::runtime_error("Unsupported variable type.");
                        }
                    }

                } else if (t.token == ASSIGN) {
                    if (lastId.empty()) {
                        throw std::runtime_error("Variable name cannot be null.");
                    }

                    String rest = tk.duplicateFromHere();
                    String::size_type pos = rest.find_first_of(';');

                    bool restNotAll = pos != String::npos;
                    if (restNotAll) {
                        rest = rest.substr(0, pos);
                        tk.skipUntil(';');
                    }

                    VarScope::getCurrent()->setVar(lastId, evalDirectly(rest));
                    lastId.clear();

                    // 整个赋值已经完毕，不会有其他结果
                    if (!restNotAll) {
                        resultType = RESULT_NONE;
                        return Var();
                    }

                } else if (t.token == VAR) {
                    resultType = RESULT_NONE;

                    tk.next(t);
                    if (t.token != ID) {
                        throw std::runtime_error("Bad variable name.");
                    }

                    VarScope *s = VarScope::getCurrent();
                    if (s->contains(t.strval)) {
                        throw std::runtime_error(String("Redefinition of '") + t.strval + "'.");
                    }

                    s->setVar(t.strval, Var());
                    lastId = t.strval;

                } else if (t.token == FUNCTION) {
                    resultType = RESULT_NONE;

                    tk.next(t);
                    if (t.token != ID) {
                        throw std::runtime_error("Bad function name.");
                    }

                } else if (t.token == ';') {
                    // 一个表达式完毕，需要计算结果
                    // 返回值暂时舍弃
                    // TODO: return value
                    calcFinalResult(opts, nums, resultType);
                    lastId.clear();
                }
            }

            calcFinalResult(opts, nums, resultType);
            if (resultType == RESULT_NUMBER || resultType == RESULT_BOOL) {
                result = nums.size() == 0 ? 0 : nums.top();
            }
            return result;
        }
    }
}

