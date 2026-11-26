//
// B1Acceptor.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1Acceptor.h"
#include "B1AcceptorListener.h"
#include "B1IOContext.h"
#include "B1ServerSocket.h"

#include <B1Base/B1Thread.h>

using namespace BnD;

B1Acceptor::B1Acceptor(const B1IOContext* context)
    : _context(context)
    , _acceptor()
    , _closed(true)
    , _listenerLock()
    , _listener(nullptr)
{
}

B1Acceptor::~B1Acceptor()
{
}

void B1Acceptor::accept()
{
    if (_acceptor == nullptr || _acceptor->is_open() != true) {
        _closed = true;
        return;
    }
    auto serverSocket = std::make_shared<B1ServerSocket>(_context);
    auto self = shared_from_this();     //  keep this object alive until the handler is called(or destroyed with io context).
    _acceptor->async_accept(
        *serverSocket->asioSocket(),
        [self, serverSocket](const boost::system::error_code& error) {
            self->acceptComplete(serverSocket, error);
        }
    );
}

void B1Acceptor::acceptComplete(std::shared_ptr<B1ServerSocket> serverSocket, const boost::system::error_code& error)
{
    {
        B1AutoLock al(_listenerLock);
        if (nullptr == _listener) {    //  finalized.
            _closed = true;
            return;
        }
        if (error) {
            _listener->onAcceptFailed(serverSocket, error.value());
        }
        else {
            _listener->onAcceptComplete(serverSocket);
        }
    }
    accept();
}

bool B1Acceptor::initialize(uint16 port, B1AcceptorListener* listener)
{
    if (_acceptor != nullptr) {
        return false;
    }
    _acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(*_context->nativeContext(), boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    _closed = false;
    _listener = listener;
    accept();
    return true;
}

void B1Acceptor::finalize()
{
    bool inIOThread = false;
    if (_acceptor) {
        B1LOG("closing acceptor");
        auto acceptor = _acceptor;
        boost::asio::post(acceptor->get_executor(), [acceptor]() {  //  acceptor is not thread-safe: close in the io thread(same thread as async_accept).
            try {
                acceptor->close();
            }
            catch (...) {}
        });
        inIOThread = _context->nativeContext()->get_executor().running_in_this_thread();
        for (int32 i = 0; i < 300 && inIOThread != true; ++i) {   //  in io thread: the posted close can not run while waiting.
            if (_closed) {
                break;
            }
            B1Thread::sleep(100);
        }
    }
    {
        B1AutoLock al(_listenerLock);
        _listener = nullptr;
    }
    if (_closed) {
        B1LOG("acceptor closed");
        _acceptor.reset();
    }
    else if (inIOThread) {
        //  called in io thread(ex: in a listener callback). the pending handler(holding this object) closes it later.
        B1LOG("called in io thread -> leave it to the pending handler");
    }
    else {
        //  io thread is busy. the pending handler(holding this object) closes it later.
        B1LOG("acceptor not closed in time -> leave it to the pending handler");
    }
}
