//
// B1Object.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1Object.h"
#include "B1Archive.h"
#include "B1String.h"

using namespace BnD;

bool B1Object::archiveToString(B1String* result, bool pretty) const
{
    B1Archive archive;
    archive.fromObject(*this);
    return archive.toString(result, pretty);
}
