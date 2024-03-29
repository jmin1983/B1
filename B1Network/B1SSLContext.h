//
// B1SSLContext.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_SSL_CONTEXT_H
#define _B1NETWORK_SSL_CONTEXT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace boost {
    namespace asio {
        namespace ssl {
            class context;
        }
    }
}

namespace BnD {
    class B1SSLContext {
    public:
        B1SSLContext(bool isServer);
        ~B1SSLContext();
    private:
        boost::asio::ssl::context* _context;
    public:
        boost::asio::ssl::context* nativeContext() const { return _context; }
        bool setServerCert(const B1String& cert, const B1String& key, const B1String& dh);
    };
}   //  !BnD

#endif  // !_B1NETWORK_SSL_CONTEXT_H
