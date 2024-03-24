//
// B1LibraryLoader.cpp
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Util.h"
#include "B1LibraryLoader.h"

#if !defined(_WIN32)
#include <dlfcn.h>
#include <stdio.h>
#endif

using namespace BnD;

void* B1LibraryLoader::getFunction(const B1String& name)
{
    assert(_handle != NULL);
#if defined(_WIN32)
    return GetProcAddress(_handle, name.cString());
#else
    dlerror();
    void* func = dlsym(_handle, name.cString());
    const char* dlsymError = dlerror();
    if (dlsymError) {
        printf("getFunction failed: name[%s], msg[%s]\n", name.cString(), dlsymError);
        return NULL;
    }
    return func;
#endif
}

bool B1LibraryLoader::load()
{
#if defined(_WIN32)
    _handle = LoadLibraryA(libraryPath().cString());
#else
    _handle = dlopen(libraryPath().cString(), RTLD_LAZY);
#endif
    if (NULL == _handle) {
        printf("load library failed: [%s]\n", libraryPath().cString());
        assert(false);
        return false;
    }
    if (onLoaded() != true) {
        unload();
        return false;
    }
    return true;
}

void B1LibraryLoader::unload()
{
    if (_handle) {
        onUnloading();
#if defined(_WIN32)
        FreeLibrary(_handle);
#else
        dlclose(_handle);
#endif
        _handle = NULL;
    }
}
