//
// B1SystemUtil.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1SystemUtil.h"

#ifdef _WIN32
#include "B1SystemUtil_windows.cpp"
#else 
#include "B1SystemUtil_linux.cpp"
#endif // _WIN32
