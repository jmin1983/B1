//
// B1LibraryLoader.h
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1UTIL_LIBRARY_LOADER_H
#define _B1UTIL_LIBRARY_LOADER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(_WIN32)
#include <Windows.h>
#endif

namespace BnD {
    class B1LibraryLoader {
    public:
        B1LibraryLoader() : _handle(NULL) {}
        virtual ~B1LibraryLoader() {}
    protected:
#if defined(_WIN32)
        typedef HMODULE Handle;
#else
        typedef void* Handle;
#endif
    private:
        Handle _handle;
    protected:
        virtual B1String libraryPath() = 0;
        virtual bool onLoaded() { return true; }
        virtual void onUnloading() {}
    protected:
        void* getFunction(const B1String& name);
    public:
        bool isValid() const { return _handle != NULL; }
        bool load();
        void unload();
    };
}   //  !BnD

#if !defined(_DO_NOT_USE_B1_MODULE_FUNCTION_MACRO)
#define B1_MODULE_FUNCTION_LOCAL(func) func##_t func = (func##_t)getFunction(#func);
#define B1_MODULE_FUNCTION_MEMBER(func, mem) mem = (func##_t)getFunction(#func);
#endif

#endif  // !_B1UTIL_LIBRARY_LOADER_H
