//
// B1BaseSocketImpl.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_SOCKET_IMPL_H
#define _B1NETWORK_BASE_SOCKET_IMPL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1Lock;
    class B1BaseSocketImpl {
    public:
        B1BaseSocketImpl();
        virtual ~B1BaseSocketImpl() {}
    private:
        std::shared_ptr<B1Lock> _closeLock;
    protected:
        virtual void implUpdateSocket(std::shared_ptr<boost::asio::ip::tcp::socket>* pAsioSocket) = 0;
        virtual auto implRollbackSocket() -> std::shared_ptr<boost::asio::ip::tcp::socket> = 0;
        virtual void implClose() = 0;
        virtual B1String implPeerAddress() const = 0;
        virtual uint16 implPeerPort() const = 0;
        virtual uint16 implLocalPort() const = 0;
        virtual bool implIsOpen() const = 0;
        virtual boost::asio::ip::tcp::socket* implGetASIOSocket() const = 0;
    public:
        void updateSocket(std::shared_ptr<boost::asio::ip::tcp::socket>* pAsioSocket) { implUpdateSocket(pAsioSocket); }
        auto rollbackSocket() -> std::shared_ptr<boost::asio::ip::tcp::socket> { return implRollbackSocket(); }
        void close();
        B1String peerAddress() const { return implPeerAddress(); }
        uint16 peerPort() const { return implPeerPort(); }
        uint16 localPort() const { return implLocalPort(); }
        bool isOpen() const { return implIsOpen(); }
        boost::asio::ip::tcp::socket* getASIOSocket() const { return implGetASIOSocket(); }
    };

    class B1ASIOSocketImpl : public B1BaseSocketImpl {
    public:
        B1ASIOSocketImpl();
    protected:
        boost::asio::ip::tcp::socket* _asioSocket;
        
    protected:
        void implUpdateSocket(std::shared_ptr<boost::asio::ip::tcp::socket>* pAsioSocket) final;
        auto implRollbackSocket() -> std::shared_ptr<boost::asio::ip::tcp::socket> final;
        void implClose() final;
        B1String implPeerAddress() const final;
        uint16 implPeerPort() const final;
        uint16 implLocalPort() const final;
        bool implIsOpen() const final;
        boost::asio::ip::tcp::socket* implGetASIOSocket() const final { return _asioSocket; }
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_SOCKET_IMPL_H
