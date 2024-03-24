//
// B1HttpResponseAPIGenerator.cpp
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpResponseAPIGenerator.h"
#include "B1HttpResponseAPICurrentTime.h"

using namespace BnD;

B1HttpResponseAPIGenerator::B1HttpResponseAPIGenerator()
{
}

B1HttpResponseAPIGenerator::~B1HttpResponseAPIGenerator()
{
}

B1String B1HttpResponseAPIGenerator::makeResponse()
{
    return implMakeResponse();
}

std::unique_ptr<B1HttpResponseAPIGenerator> B1HttpResponseAPIGenerator::createAPIGenerator(const B1String& apiName)
{
    if ("CurrentTime" == apiName) {
        return std::make_unique<B1HttpResponseAPICurrentTime>();
    }
    return std::unique_ptr<B1HttpResponseAPIGenerator>();
}
