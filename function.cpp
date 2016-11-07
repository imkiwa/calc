#include "function.h"
#include "varscope.h"
#include "expression.h"

namespace kiva {
    namespace function {
        static std::map<String, IFunction*>& getFunctions()
        {
            static std::map<String, IFunction*> functions;
            return functions;
        }

        bool FunctionTable::contains(const String &name)
        {
            auto &f = getFunctions();
            return f.find(name) != f.end();
        }

        IFunction* FunctionTable::getFunction(const String &name)
        {
            auto &f = getFunctions();
            auto iter = f.find(name);
            if (iter != f.end()) {
                return (*iter).second;
            }

            return nullptr;
        }

        void FunctionTable::addFunction(IFunction *function)
        {
            if (!function) {
                return;
            }

            if (contains(function->getName())) {
                return;
            }

            auto &f = getFunctions();
            f[function->getName()] = function;
        }

        void FunctionTable::linkFunction(const String &name,
                          const Callable &callable)
        {
            if (contains(name)) {
                return;
            }

            IFunction *f = new NativeFunction(name, callable);
            addFunction(f);
        }

        Function::Function(const String &name)
            :mName(name)
        {}

        Function::Function(const String &name, const String &body)
            :mName(name), mBody(body)
        {}

        Function::Function(const Function &other)
            :mName(other.mName), mBody(other.mBody), mParams(other.mParams)
        {}

        Var Function::invoke(const Args &args, int &resultType) throw(std::runtime_error)
        {
            using namespace var;
            if (args.size() != getParamCount()) {
                throw std::runtime_error("Arguments count doesn't match.");
            }

            VarScope *scope = VarScope::newScope(getName());
            for (int i = 0; i < mParams.size(); ++i) {
                scope->setVar(mParams[i], args[i]);
            }

            Var v = kiva::expression::evalDirectly(mBody, resultType);
            VarScope::removeTop();
            return v;
        }

        NativeFunction::NativeFunction(const String &name)
            :mName(name)
        {
        }

        NativeFunction::NativeFunction(const String &name,
                                       const Callable &callable)
            :mName(name), mCallable(callable)
        {
        }

        NativeFunction::NativeFunction(const NativeFunction &other)
            :mName(other.mName), mCallable(other.mCallable)
        {
        }
    }
}

