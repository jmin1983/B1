//
// B1BaseComponent.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1BaseComponent.h"
#include "B1BaseSessionManager.h"

using namespace BnD;

B1BaseComponent::B1BaseComponent()
{
}

B1BaseComponent::~B1BaseComponent()
{
}

B1BaseSessionManager* B1BaseComponent::createSessionManager()
{
    return new B1BaseSessionManager();
}

bool B1BaseComponent::startup()
{
    _sessionManager.reset(createSessionManager());
    if (_sessionManager->startup() != true) {
        return false;
    }
    return implStartup();
}

void B1BaseComponent::shutdown()
{
    if (_sessionManager) {
        _sessionManager->shutdown();
    }
    implShutdown();
}
