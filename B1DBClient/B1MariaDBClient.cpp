//
// B1MariaDBClient.cpp
//
// Library: B1DBClient
// Package: B1DBClient
// Module:  B1DBClient
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1DBClient.h"
#include "B1MariaDBClient.h"
#include "B1MariaDBHandle.h"
#include "B1MariaDBResult.h"

using namespace BnD;

B1MariaDBClient::B1MariaDBClient()
{
}

B1MariaDBClient::~B1MariaDBClient()
{
}

bool B1MariaDBClient::initialize(const B1String& address, uint16 port, const B1String& dbName, const B1String& user, const B1String& password, bool useSSL)
{
    if (_handle) {
        return false;
    }
    _handle.reset(new B1MariaDBHandle());
    if (_handle->initialize(useSSL) != true) {
        _handle->finalize();
        _handle.reset();
        return false;
    }
    try {
        boost::asio::ip::tcp::resolver resolver(_handle->context()->get_executor());
        char portString[10] = {0};
        sprintf(portString, "%d", port);
        auto endpoints = resolver.resolve(address.cString(), portString);
        boost::mysql::handshake_params params(user.cString(), password.cString(), dbName.cString());
        if (useSSL) {
            _handle->sslConnection()->connect(*endpoints.begin(), params);
        }
        else {
            _handle->connection()->connect(*endpoints.begin(), params);
        }
    }
    catch (const boost::mysql::error_with_diagnostics& err) {
        B1LOG("boost_my_sql_error: code[%d], msg[%s]", err.code().value(), err.what());
        _handle->finalize();
        _handle.reset();
        return false;
    }
    catch (...) {
        B1LOG("unknown error in boost_my_sql");
        _handle->finalize();
        _handle.reset();
        return false;
    }
    return true;
}

void B1MariaDBClient::finalize()
{
    if (_handle) {
        _handle->finalize();
        _handle.reset();
    }
}

bool B1MariaDBClient::execute(const B1String& sql, B1MariaDBResult* result) const
{
    try {
        if (auto connection = _handle->connection()) {
            boost::mysql::results temp;
            connection->execute(sql.cString(), temp);
            if (result) {
                result->readDBResult(temp.rows());
            }
        }
        else if (auto connection = _handle->sslConnection()) {
            boost::mysql::results temp;
            connection->execute(sql.cString(), temp);
            if (result) {
                result->readDBResult(temp.rows());
            }
        }
        else {
            return false;
        }
    }
    catch (const boost::mysql::error_with_diagnostics& err) {
        B1LOG("boost_my_sql_execute error: code[%d], server_msg[%s], client_msg[%s], what[%s], sql[%s]",
              err.code().value(), std::string(err.get_diagnostics().server_message()).c_str(), std::string(err.get_diagnostics().client_message()).c_str(), err.what(), sql.cString());
        return false;
    }
    catch (...) {
        B1LOG("unknown execute error in boost_my_sql: sql[%s]", sql.cString());
        return false;
    }
    return true;
}

bool B1MariaDBClient::executeBatch(const B1String& sql, B1MariaDBResult* result) const
{
    boost::mysql::execution_state state;
    try {
        if (auto connection = _handle->connection()) {
            connection->start_execution(sql.cString(), state);
            while (state.complete() != true) {
                auto rows = connection->read_some_rows(state);
                if (result) {
                    result->readDBResult(rows);
                }
            }
        }
        else if (auto connection = _handle->sslConnection()) {
            connection->start_execution(sql.cString(), state);
            while (state.complete() != true) {
                auto rows = connection->read_some_rows(state);
                if (result) {
                    result->readDBResult(rows);
                }
            }
        }
        else {
            return false;
        }
    }
    catch (const boost::mysql::error_with_diagnostics& err) {
        B1LOG("boost_my_sql_execute_batch error: code[%d], server_msg[%s], client_msg[%s], what[%s], sql[%s]",
              err.code().value(), std::string(err.get_diagnostics().server_message()).c_str(), std::string(err.get_diagnostics().client_message()).c_str(), err.what(), sql.cString());
        return false;
    }
    catch (...) {
        B1LOG("unknown execute_batch error in boost_my_sql: sql[%s]", sql.cString());
        return false;
    }
    return true;
}

bool B1MariaDBClient::executePrepared(const B1String& stmt,
                                      const std::vector<B1String>& bind0, const std::vector<B1String>& bind1,
                                      const std::vector<std::vector<uint8> >& bind2, const std::vector<std::vector<uint8> >& bind3, const std::vector<std::vector<uint8> >& bind4) const
{
    const size_t bindSize = bind0.size();
    if (bindSize != bind1.size() ||
        bindSize != bind2.size() ||
        bindSize != bind3.size() ||
        bindSize != bind4.size()) {
        return false;
    }
    try {
        if (auto connection = _handle->connection()) {
            auto statement = connection->prepare_statement(stmt.cString());
            for (size_t i = 0; i < bindSize; ++i) {
                boost::mysql::results temp;
                connection->execute(statement.bind(bind0[i].cString(), bind1[i].cString(), bind2[i], bind3[i], bind4[i]), temp);
            }
        }
        else if (auto connection = _handle->sslConnection()) {
            auto statement = connection->prepare_statement(stmt.cString());
            for (size_t i = 0; i < bindSize; ++i) {
                boost::mysql::results temp;
                connection->execute(statement.bind(bind0[i].cString(), bind1[i].cString(), bind2[i], bind3[i], bind4[i]), temp);
            }
        }
        else {
            return false;
        }
    }
    catch (const boost::mysql::error_with_diagnostics& err) {
        B1LOG("boost_my_sql_execute_prepared_statement error: code[%d], server_msg[%s], client_msg[%s], what[%s], stmt[%s]",
              err.code().value(), std::string(err.get_diagnostics().server_message()).c_str(), std::string(err.get_diagnostics().client_message()).c_str(), err.what(), stmt.cString());
        return false;
    }
    catch (...) {
        B1LOG("unknown execute prepared statement error in boost_my_sql: stmt[%s]", stmt.cString());
        return false;
    }
    return true;
}
