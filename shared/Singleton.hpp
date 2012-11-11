#ifndef SINGLETON_DEFINED_HPP
#define SINGLETON_DEFINED_HPP

#include <memory>
#include <mutex>

// Thread-Safe version using :
// Lambda
// std::once_flag
// std::unique_ptr

template<class T>
class Singleton
{
    public:
        virtual ~Singleton() { }
        static T* GetInstance()
        {
            std::call_once(m_onceFlag, []()
            {
                m_instance.reset(new T);
            });

            return m_instance.get();
        }

    private:
        static std::unique_ptr<T> m_instance;
        static std::once_flag m_onceFlag;

        Singleton(const Singleton& src) = default;
        Singleton& operator=(const Singleton& rhs) = default;
    protected:
        Singleton() = default;
   };

template<class T>
std::unique_ptr<T> Singleton<T>::m_instance = nullptr;
template<class T>
std::once_flag Singleton<T>::m_onceFlag;

#endif
