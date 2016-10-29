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

        class IFunction
        {
        public:
            IFunction() = default;
            virtual ~IFunction() = default;

            virtual bool isNative() const = 0;
            virtual void addParam(const String &name) = 0;
            virtual int getParamCount() const = 0;
            virtual const String& getName() const = 0;
            virtual Var invoke(const std::vector<Var> &vars, int &resultType) throw(std::runtime_error) = 0;
        };

        class FunctionTable
        {
        public:
            static bool contains(const String &name);
            static IFunction* getFunction(const String &name);
            static void addFunction(IFunction *function);
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

            virtual void addParam(const String &name) override
            {
                mParams.push_back(name);
            }

            virtual Var invoke(const std::vector<Var> &vars, int &resultType) throw(std::runtime_error) override;

            virtual int getParamCount() const override
            {
                return mParams.size();
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
        };
    }
}

#endif // FUNCTION_H
