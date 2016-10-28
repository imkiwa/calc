#ifndef VARSCOPE_H
#define VARSCOPE_H

#include <stack>
#include <map>
#include <stdexcept>
#include <exception>

#include "var.h"
#include "config.h"

namespace kiva {
    namespace var {
        class VarScope
        {
        public:
            static std::stack<VarScope*>& getAllScopes();
            static VarScope* getCurrent();
            static VarScope* getGlobal();
            static VarScope* newScope(const String &name);
            static void removeTop();

        private:
            String mName;
            VarScope *mParent;
            std::map<String, Var> mVars;

        public:
            VarScope(const String &name, VarScope *parent);
            ~VarScope();

            bool contains(const String &name) const;

            void setVar(const String &name, Var var);

            Var getVar(const String &name) throw(std::runtime_error);

            const String& getName() const
            {
                return mName;
            }

            const std::map<String, Var>& getVars() const
            {
                return mVars;
            }
        };

    }
}

#endif // VARSCOPE_H
