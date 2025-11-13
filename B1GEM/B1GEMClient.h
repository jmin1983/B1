//
// B1GEMClient.h
//
// Library: B1GEM
// Package: Client
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_CLIENT_H
#define _B1GEM_CLIENT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1GEM/B1GEMConsts.h>
#include <B1SECS2/B1SECS2Client.h>

namespace BnD {
    class B1GEMClient : public B1SECS2Client {
    public:
        B1GEMClient();
        virtual ~B1GEMClient();
    protected:
        std::shared_ptr<class B1GEMClientSession> getSession(uint16 deviceID) const;
    public:
        B1GEMConsts::COMM_STATE currentCommunicationState(uint16 deviceID) const;
    };
}   //  !BnD

#endif  // !_B1GEM_CLIENT_H
