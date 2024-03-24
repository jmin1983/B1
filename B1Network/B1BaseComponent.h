//
// B1BaseComponent.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_COMPONENT_H
#define _B1NETWORK_BASE_COMPONENT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1BaseSessionManager;
    class B1BaseComponent {
    public:
        B1BaseComponent();
        virtual ~B1BaseComponent();
    protected:
        std::shared_ptr<B1BaseSessionManager> _sessionManager;
    protected:
        virtual bool implStartup() { return true; }
        virtual void implShutdown() {}
    protected:
        virtual B1BaseSessionManager* createSessionManager();
    protected:
        B1BaseSessionManager* sessionManager() const { return _sessionManager.get(); }
    public:
        bool startup();
        void shutdown();
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_COMPONENT_H
