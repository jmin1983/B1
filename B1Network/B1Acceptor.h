//
// B1Acceptor.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_ACCEPTOR_H
#define _B1NETWORK_ACCEPTOR_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace boost {
    namespace system {
        class error_code;
    };
};

namespace BnD {
    class B1AcceptorListener;
    class B1BaseSocket;
    class B1IOContext;
    class B1ServerSocket;
    class B1Acceptor {
    public:
        B1Acceptor(const B1IOContext* context);
        virtual ~B1Acceptor();
    private:
        const B1IOContext* _context;
    protected:
        std::shared_ptr<boost::asio::ip::tcp::acceptor> _acceptor;
        bool _closed;
        B1AcceptorListener* _listener;
    private:
        void acceptComplete(B1ServerSocket* serverSocket, const boost::system::error_code& error);
    protected:
        void accept();
    public:
        bool initialize(uint16 port, B1AcceptorListener* listener);
        void finalize();
    };
}   //  !BnD

#endif  // !_B1NETWORK_ACCEPTOR_H
