#pragma once
#include <cassert>

template <class T>
class Singleton
{
public:
    typedef Singleton<T> base;

    Singleton()
    {
        assert(nullptr == msInstance);
        msInstance = static_cast<T*>(this);
    }

    virtual ~Singleton()
    {
        assert(nullptr != msInstance);
        msInstance = nullptr;
    }

    static T& getInstance()
    {
        return (*getInstancePtr());
    }

    static T* getInstancePtr()
    {
		assert(nullptr != msInstance);
        return msInstance;
    }

private:
    static T* msInstance;
};

template<class T>
T * Singleton<T>::msInstance = nullptr;
