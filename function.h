#ifndef FUNCTION_H
#define FUNCTION_H

#include "config.h"
#include "var.h"
#include <stdexcept>
#include <exception>
#include <vector>
#include <map>

namespace kiva {
    namespace function {
        using namespace var;

        typedef std::vector<Var> Args;

        typedef std::function<Var(const Args &, int &)> Callable;

        class IFunction
        {
        public:
            IFunction() = default;
            virtual ~IFunction() = default;

            virtual bool isNative() const = 0;
            virtual const String& getName() const = 0;
            virtual Var invoke(const Args &args, int &resultType) throw(std::runtime_error) = 0;
        };

        class FunctionTable
        {
        public:
            static bool contains(const String &name);
            static IFunction* getFunction(const String &name);
            static void addFunction(IFunction *function);
            static void linkFunction(const String &name, const Callable &callable);
        };

        class Function : public IFunction
        {
        private:
            String mName;
            String mBody;
            std::vector<String> mParams;

        public:
            Function() = default;
            Function(const String &name);
            Function(const String &name, const String &body);
            Function(const Function &other);

            virtual ~Function() = default;

            virtual Var invoke(
                const Args &args,
                int &resultType) throw(std::runtime_error) override;

            int getParamCount() const
            {
                return mParams.size();
            }
            
            void addParam(const String &name)
            {
                mParams.push_back(name);
            }

            void setBody(const String &body)
            {
                mBody = body;
            }

            virtual const String& getName() const override
            {
                return mName;
            }

            virtual bool isNative() const override
            {
                return false;
            }

        };

        class NativeFunction : public IFunction
        {
        private:
            String mName;
            Callable mCallable;

        public:
            NativeFunction() = default;
            NativeFunction(const String &name);
            NativeFunction(const String &name, const Callable &callable);
            NativeFunction(const NativeFunction &other);

            virtual ~NativeFunction() = default;

            virtual Var invoke(
                const Args &args,
                int &resultType) throw(std::runtime_error) override
            {
                return mCallable ? mCallable(args, resultType)
                                 : Var();
            }

            virtual const String& getName() const override
            {
                return mName;
            }

            virtual bool isNative() const override
            {
                return true;
            }
        };
        
        
    }
}

#endif // FUNCTION_H
