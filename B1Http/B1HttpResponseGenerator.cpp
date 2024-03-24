//
// B1HttpResponseGenerator.cpp
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpResponseGenerator.h"
#include "B1HttpMessage.h"
#include "B1HttpResponseAPIGenerator.h"

#include <boost/bind/bind.hpp>

namespace Beast = boost::beast;
namespace BeastHttp = Beast::http;

using namespace BnD;

B1HttpResponseGenerator::B1HttpResponseGenerator()
{
}

B1HttpResponseGenerator::~B1HttpResponseGenerator()
{
}

BeastHttp::message_generator B1HttpResponseGenerator::makeResponseBadRequest(const B1HttpMessage& message, const B1String& why)
{
    BeastHttp::response<BeastHttp::string_body> response(BeastHttp::status::bad_request, message.request().version());
    response.set(BeastHttp::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(BeastHttp::field::content_type, "text/html");
    response.keep_alive(message.request().keep_alive());
    response.body() = why.to_string();
    response.prepare_payload();
    return response;
}

BeastHttp::message_generator B1HttpResponseGenerator::makeResponseNotFound(const B1HttpMessage& message)
{
    BeastHttp::response<BeastHttp::string_body> response(BeastHttp::status::not_found, message.request().version());
    response.set(BeastHttp::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(BeastHttp::field::content_type, "text/html");
    response.keep_alive(message.request().keep_alive());
    response.body() = "The resource '" + std::string(message.request().target()) + "' was not found.";
    response.prepare_payload();
    return response;
}

BeastHttp::message_generator B1HttpResponseGenerator::makeResponseServerError(const B1HttpMessage& message, const B1String& what)
{
    BeastHttp::response<BeastHttp::string_body> response(BeastHttp::status::internal_server_error, message.request().version());
    response.set(BeastHttp::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(BeastHttp::field::content_type, "text/html");
    response.keep_alive(message.request().keep_alive());
    response.body() = "An error occurred: '" + what.to_string() + "'";
    response.prepare_payload();
    return response;
}

BeastHttp::message_generator B1HttpResponseGenerator::makeResponseContents(const B1HttpMessage& message, const B1String& contentsRootPath)
{
    // Build the path to the requested file.
    auto path = getRequestedContentsFullPath(contentsRootPath, message.request().target());
    if (message.request().target().back() == '/') {
        path.append("index.html");
    }

    // Attempt to open the file.
    Beast::error_code ec;
    BeastHttp::file_body::value_type body;
    body.open(path.cString(), Beast::file_mode::scan, ec);
    if (ec) {
        if (Beast::errc::no_such_file_or_directory == ec) {         // Handle the case where the file doesn't exist.
            return makeResponseNotFound(message);
        }
        else {
            return makeResponseServerError(message, ec.message());  // Handle an unknown error.
        }
    }

    // Cache the size since we need it after the move.
    const auto size = body.size();

    if (message.request().method() == BeastHttp::verb::head) {    // Respond to HEAD request.
        BeastHttp::response<BeastHttp::empty_body> response(BeastHttp::status::ok, message.request().version());
        response.set(BeastHttp::field::server, BOOST_BEAST_VERSION_STRING);
        response.set(BeastHttp::field::content_type, getMimeType(path.to_string()));
        response.content_length(size);
        response.keep_alive(message.request().keep_alive());
        return response;
    }
    else {  // Respond to GET request.
        BeastHttp::response<BeastHttp::file_body> response(std::piecewise_construct,
                                                           std::make_tuple(std::move(body)),
                                                           std::make_tuple(BeastHttp::status::ok, message.request().version()));
        response.set(BeastHttp::field::server, BOOST_BEAST_VERSION_STRING);
        response.set(BeastHttp::field::content_type, getMimeType(path.to_string()));
        response.content_length(size);
        response.keep_alive(message.request().keep_alive());
        return response;
    }
}

BeastHttp::message_generator B1HttpResponseGenerator::makeResponseAPI(const B1HttpMessage& message, const B1String& apiName)
{
    auto apiGenerator = B1HttpResponseAPIGenerator::createAPIGenerator(apiName);
    if (NULL == apiGenerator) {
        return makeResponseServerError(message, "No API name:" + apiName);
    }
    BeastHttp::response<BeastHttp::string_body> response(BeastHttp::status::ok, message.request().version());
    response.set(BeastHttp::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(BeastHttp::field::content_type, "text/html");
    response.keep_alive(message.request().keep_alive());
    response.body() = apiGenerator->makeResponse().to_string();
    response.prepare_payload();
    return response;
}

Beast::string_view B1HttpResponseGenerator::getMimeType(Beast::string_view path)
{
    using Beast::iequals;
    auto const ext = [&path]
    {
        auto const pos = path.rfind(".");
        if (pos == Beast::string_view::npos)
            return Beast::string_view{};
        return path.substr(pos);
    }();
    if (iequals(ext, ".htm"))  return "text/html";
    if (iequals(ext, ".html")) return "text/html";
    if (iequals(ext, ".php"))  return "text/html";
    if (iequals(ext, ".css"))  return "text/css";
    if (iequals(ext, ".txt"))  return "text/plain";
    if (iequals(ext, ".js"))   return "application/javascript";
    if (iequals(ext, ".json")) return "application/json";
    if (iequals(ext, ".xml"))  return "application/xml";
    if (iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if (iequals(ext, ".flv"))  return "video/x-flv";
    if (iequals(ext, ".png"))  return "image/png";
    if (iequals(ext, ".jpe"))  return "image/jpeg";
    if (iequals(ext, ".jpeg")) return "image/jpeg";
    if (iequals(ext, ".jpg"))  return "image/jpeg";
    if (iequals(ext, ".gif"))  return "image/gif";
    if (iequals(ext, ".bmp"))  return "image/bmp";
    if (iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if (iequals(ext, ".tiff")) return "image/tiff";
    if (iequals(ext, ".tif"))  return "image/tiff";
    if (iequals(ext, ".svg"))  return "image/svg+xml";
    if (iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}

B1String B1HttpResponseGenerator::getRequestedAPIName(const Beast::string_view& target) const
{
    for (int32 i = static_cast<int32>(target.size()) - 1; i > 0; --i) {
        if (target[i] == '/' && i > 3) {
            if (target[i - 3] == 'A' && target[i - 2] == 'P' && target[i - 1] == 'I') {
                return std::string(target.data() + i + 1, target.end());
            }
            break;
        }
    }
    return "";
}

B1String B1HttpResponseGenerator::getRequestedContentsFullPath(const B1String& contentsRootPath, const Beast::string_view& target)
{
    if (contentsRootPath.isEmpty()) {
        return std::string(target);
    }
    std::string result(contentsRootPath.to_string());
#ifdef BOOST_MSVC
    char constexpr pathSeparator = '\\';
    if (result.back() == pathSeparator) {
        result.resize(result.size() - 1);
    }
    result.append(target.data(), target.size());
    for (auto& c : result) {
        if (c == '/') {
            c = pathSeparator;
        }
    }
#else
    char constexpr pathSeparator = '/';
    if (result.back() == pathSeparator) {
        result.resize(result.size() - 1);
    }
    result.append(target.data(), target.size());
#endif
    return result;
}

BeastHttp::message_generator B1HttpResponseGenerator::makeResponse(const B1HttpMessage& message, const B1String& contentsRootPath)
{
    if (message.request().method() != BeastHttp::verb::get &&
        message.request().method() != BeastHttp::verb::head) {
        return makeResponseBadRequest(message, "Unknown HTTP-method");
    }

    // Request path must be absolute and not contain "..".
    if (message.request().target().empty() ||
        message.request().target()[0] != '/' ||
        message.request().target().find("..") != Beast::string_view::npos) {
        return makeResponseBadRequest(message, "Illegal request-target");
    }

    auto apiName = getRequestedAPIName(message.request().target());
    if (apiName.isEmpty() != true) {
        return makeResponseAPI(message, apiName);
    }
    else {
        return makeResponseContents(message, contentsRootPath);
    }
}
