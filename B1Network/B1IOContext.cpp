//
// B1IOContext.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//
#include "B1Network.h"
#include "B1IOContext.h"

#include <B1Base/B1Thread.h>

using namespace BnD;

B1IOContext::B1IOContext(uint8 threadCountHint)
    : _context(new boost::asio::io_context(threadCountHint))
{
}

B1IOContext::~B1IOContext()
{
    delete _context;
}

void B1IOContext::stop()
{
    _context->stop();
}

bool B1IOContext::isStopped() const
{
    return _context->stopped();
}

void B1IOContext::runNativeContext()
{
    _context->run();
}

void B1IOContext::stopNativeContext()
{
    _context->stop();
}

void B1IOContext::restartNativeContext()
{
    _context->restart();
}

bool B1IOContext::isNativeContextStopped()
{
    return _context->stopped();
}
