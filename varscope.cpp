#include "varscope.h"

namespace kiva {
    namespace var {
        std::stack<VarScope*>& VarScope::getAllScopes()
        {
            static std::stack<VarScope*> scopes;
            return scopes;
        }

        VarScope* VarScope::getCurrent()
        {
            auto &s = getAllScopes();
            if (s.size() == 0) {
                return getGlobal();
            }
            return s.top();
        }

        VarScope* VarScope::getGlobal()
        {
            static VarScope *global = nullptr;
            if (!global) {
                global = new VarScope("::", nullptr);
                getAllScopes().push(global);
            }
            return global;
        }

        VarScope* VarScope::newScope(const String &name)
        {
            VarScope *s = new VarScope(name, getCurrent());
            return s;
        }

        void VarScope::removeTop()
        {
            auto &s = getAllScopes();
            if (s.size() <= 1) {
                return;
            }

            VarScope *scope = getCurrent();
            delete scope;
            getAllScopes().pop();
        }

        VarScope::VarScope(const String &name, VarScope *parent)
            :mName(name), mParent(parent)
        {
        }

        VarScope::~VarScope()
        {
        }

        bool VarScope::contains(const String &name) const
        {
            if (mVars.find(name) != mVars.end()) {
                return true;
            }
            return mParent ? mParent->contains(name) : false;
        }

        void VarScope::setVar(const String &name, Var var)
        {
            mVars[name] = var;
        }

        Var VarScope::getVar(const String &name) throw(std::runtime_error)
        {
            auto iter = mVars.find(name);
            if (iter != mVars.end()) {
                return (*iter).second;
            }

            if (!mParent) {
                throw std::runtime_error(String("Variable '") + name + "'' not found");
            }

            return mParent->getVar(name);
        }
    }
}

