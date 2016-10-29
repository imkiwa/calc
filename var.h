#ifndef VAR_H
#define VAR_H

#include <typeinfo>
#include <stdexcept>
#include <exception>
#include <string>

namespace kiva {
    namespace var {
        class IHolder
        {
        public:
            IHolder() = default;
            virtual ~IHolder() = default;
            virtual const std::type_info& getType() const = 0;
            virtual bool equals(const IHolder *other) const = 0;
            virtual IHolder* clone() = 0;
        };

        template <typename T>
        class Holder : public IHolder
        {
        private:
            T mData;
        public:
            Holder() = default;

            Holder(const T &t):mData(t) {
            }

            virtual ~Holder() = default;

            virtual const std::type_info& getType() const override
            {
                return typeid(mData);
            }

            virtual bool equals(const IHolder *other) const override
            {
                if (other == this) {
                    return true;
                }
                if (!other || other->getType() != this->getType()) {
                    return false;
                }

                const Holder<T> *p = dynamic_cast<const Holder<T>*>(other);
                return p ? p->get() == this->get() : false;
            }

            virtual IHolder* clone() override
            {
                return new Holder<T>(mData);
            }

            T& get()
            {
                return mData;
            }

            const T& get() const
            {
                return mData;
            }

            void set(const T &data)
            {
                mData = data;
            }
        };


        class Var
        {
        private:
            IHolder *mData;

            void safeDelete()
            {
                if (mData) {
                    delete mData;
                    mData = nullptr;
                }
            }

            IHolder* safeClone() const
            {
                return mData ? mData->clone() : nullptr;
            }

            template <typename T>
            Holder<T>* safeCastHolder() const
            {
                return mData ? dynamic_cast<Holder<T>*>(mData) : nullptr;
            }

        public:
            Var() :mData(nullptr) {}

            template <typename T>
            Var(const T &t) :mData(new Holder<T>(t)) {}

            Var(const char *s) :Var(std::string(s)) {}

            Var(const Var &other) :mData(other.safeClone()) {}

            const std::type_info& getType() const
            {
                if (mData == nullptr) {
                    return typeid(nullptr);
                }

                return mData->getType();
            }

            template <typename T>
            Var& operator=(const T &t)
            {
                safeDelete();
                mData = new Holder<T>(t);
                return *this;
            }

            Var& operator=(const char *s)
            {
                return operator=(std::string(s));
            }

            Var& operator=(const Var &v)
            {
                safeDelete();
                mData = v.safeClone();
                return *this;
            }

            template <typename T>
            bool operator==(const T &t) const
            {
                if (getType() != typeid(t)) {
                    return false;
                }

                const Holder<T> *p = safeCastHolder<T>();
                return p ? p->get() == t : false;
            }

            bool operator==(const Var &other) const
            {
                if (mData == nullptr && other.mData == nullptr) {
                    return true;
                }
                if (mData) {
                    return mData->equals(other.mData);
                }
                return false;
            }

            bool isValid() const
            {
                return mData && getType() != typeid(nullptr);
            }

            template <typename T>
            T& as() throw(std::runtime_error)
            {
                if (getType() != typeid(T)) {
                    throw std::runtime_error("Uncheckek object cast.");
                }

                Holder<T> *p = safeCastHolder<T>();
                if (p) {
                    return p->get();
                }
                throw std::runtime_error("Bad object: cast failed.");
            }

            template <typename T>
            const T& as() const throw(std::runtime_error)
            {
                if (getType() != typeid(T)) {
                    throw std::runtime_error("Uncheckek object cast.");
                }

                const Holder<T> *p = safeCastHolder<T>();
                if (p) {
                    return p->get();
                }
                throw std::runtime_error("Bad object: cast failed.");
            }
        };
    }
}

#endif // VAR_H
