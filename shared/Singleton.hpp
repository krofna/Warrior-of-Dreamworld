#ifndef SINGLETON_DEFINED
#define SINGLETON_DEFINED

#include "Config.hpp"

// TODO: Make Singleton<Class> thread safe.

template<class Class>
class Singleton
{
    static Class* GetInstance() // AUTO MAGICALLY CREATE THE INSTANCE OF CLASS !
    {
        if (sInstance == nullptr)
            sInstance = new Class;

        return sInstance;
    }
    static void DestroyInstance()
    {
        if (sInstance)
            delete sInstance;
    }

    protected:
    Singleton() { }
    ~Singleton() { }

    private:
    static Class* sInstance;
};

template<class Class>
static Class* Singleton<Class>::sInstance = nullptr;

#ifdef HAVE_VARIADIC_TEMPLATE
template<class Class, typename... Args>
class Singleton
{
    public:
    static Class* GetInstance() // NEVER CREATE AUTOMAGICALLY THE INSTANCE OF CLASS !
    {
        return sInstance;
    }

    static void CreateInstance(bool recreate, Args const && ... args)
    {
        if (sInstance != nullptr)
        {
            if (recreate)
                DestroyInstance();
            else
                return;
        }

        sInstance = new Class(std::forward<Args>(args)...);
    }
    static void DestroyInstance()
    {
        if (sInstance)
            delete sInstance;
    }

    protected:
    Singleton() { }
    ~Singleton() { }

    private:
    static Class* sInstance;
};

template<class Class, typename... Args>
static Class* Singleton<Class, Args>::sInstance = nullptr;

#endif

#endif
