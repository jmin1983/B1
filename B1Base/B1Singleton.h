//
// B1StaticSingleton.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_SINGLETON_H
#define _B1BASE_SINGLETON_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    template <typename T>
    class B1StaticSingleton {
    protected:
        B1StaticSingleton() {}
    public:
        virtual ~B1StaticSingleton() {}
        static T& get() {
            static T s_instance;
            return s_instance;
        }
    };

    template <typename T>
    class B1Singleton {
    private:
        static T* _singleton;
    public:
        B1Singleton()
        {
            assert(0 == _singleton);
            std::ptrdiff_t offset = (std::ptrdiff_t)(T*)1 - (std::ptrdiff_t)(B1Singleton<T>*)1;
            _singleton = (T*)(this + offset);
        }
        virtual ~B1Singleton()
        {
            _singleton = 0;
        }
        static T* get()
        {
            return _singleton;
        }
        template <typename U>
        static U* getTemplate()
        {
            return dynamic_cast<U*>(_singleton);
        }
        template <typename U>
        static U* getTemplateStatic()
        {
            return static_cast<U*>(_singleton);
        }
    };

    template <typename T>
    T* B1Singleton<T>::_singleton = 0;
}   //  !BnD

#endif  // !_B1BASE_SINGLETON_H
