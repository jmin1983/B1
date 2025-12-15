//
// B1GEMClientSession.h
//
// Library: B1GEM
// Package: Client
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_CLIENT_SESSION_H
#define _B1GEM_CLIENT_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2ClientSession.h>
#include <B1GEM/B1GEMConsts.h>

namespace BnD {
    class B1GEMClientSession : public B1SECS2ClientSession {
    public:
        B1GEMClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener);
    protected:
        B1GEMConsts::CONTROL_STATE _controlState;
    protected:
        virtual void onRecvMessageS1F1(bool wait, const std::vector<uint8>& systemBytes) override;
    protected:
        void setControlState(B1GEMConsts::CONTROL_STATE controlState);
    public:
        B1GEMConsts::COMM_STATE currentCommunicationState() const;
    };
}   //  !BnD

#endif  // !_B1GEM_CLIENT_SESSION_H
