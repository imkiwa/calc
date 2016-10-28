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
                case POW:
                    res = std::pow(l, r);
                    break;
                case MOD:
                    res = static_cast<int>(l) % static_cast<int>(r);
                    break;
            }
            opds.push(res);
        }

        void calcNumber(std::stack<Real> &opds, int opt)
        {
            switch (opt) {
                case ADD:
                case SUB:
                case MUL:
                case DIV:
                case POW:
                case MOD:
                    calcNumber2(opds, opt);
                    return;
                case NAV:
                    Real n = opds.top(); opds.pop();
                    opds.push(-n);
                    return;
            }
        }

        Var evalDirectly(const String &str) throw(std::runtime_error)
        {
            Tokenizer tk(str);
            Token t;

            std::stack<Real> nums;
            std::stack<int>  opts;

            while (tk.next(t)) {
                if (t.token == ADD || t.token == SUB
                        || t.token == MUL || t.token == DIV
                        || t.token == MOD || t.token == POW
                        || t.token == NAV) {
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
                    nums.push(t.numval);

                }
            }

            while (opts.size() != 0) {
                int opt = opts.top();
                calcNumber(nums, opt);
                opts.pop();
            }

            return nums.size() == 0 ? Var() : Var(nums.top());
        }
    }
}

