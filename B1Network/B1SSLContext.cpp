//
// B1SSLContext.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//
#include "B1Network.h"
#include "B1SSLContext.h"

#include <boost/asio/ssl.hpp>

using namespace BnD;

B1SSLContext::B1SSLContext(bool isServer)
    : _context(new boost::asio::ssl::context(isServer ? boost::asio::ssl::context::tlsv12 : boost::asio::ssl::context::tls_client))
{
}

B1SSLContext::~B1SSLContext()
{
    delete _context;
}

bool B1SSLContext::setServerCert(const B1String& cert, const B1String& key, const B1String& dh)
{
    if (cert.isEmpty() || key.isEmpty() || dh.isEmpty()) {
        return false;
    }
    _context->set_options(boost::asio::ssl::context::default_workarounds |
                          boost::asio::ssl::context::no_sslv2 |
                          boost::asio::ssl::context::single_dh_use);
    _context->use_certificate_chain(boost::asio::buffer(cert.to_string().data(), cert.to_string().size()));
    _context->use_private_key(boost::asio::buffer(key.to_string().data(), key.to_string().size()), boost::asio::ssl::context::file_format::pem);
    _context->use_tmp_dh(boost::asio::buffer(dh.to_string().data(), dh.to_string().size()));
    return true;
}
