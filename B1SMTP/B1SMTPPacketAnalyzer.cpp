//
// B1SMTPPacketAnalyzer.cpp
//
// Library: B1SMTP
// Package: B1SMTP
// Module:  B1SMTP
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SMTP.h"
#include "B1SMTPPacketAnalyzer.h"
#include "B1SMTPProtocol.h"

using namespace BnD;

auto B1SMTPPacketAnalyzer::implAnalyzeData(uint8* data, size_t size, size_t* pos) -> ANALYZE_RESULT
{
    return analyzeSMTPResponse(data, size, pos);
}

auto B1SMTPPacketAnalyzer::analyzeSMTPResponse(uint8* data, size_t size, size_t* pos) -> ANALYZE_RESULT
{
    const int32 responseValueSize = 3;
    if (size < responseValueSize) {
        return ANALYZE_RESULT_NOT_ENOUTH_DATA;
    }
    auto response(B1SMTPProtocol::RESPONSE_UNKNOWN);
    try {
        std::vector<char> responseValue(responseValueSize + 1, 0);
        memcpy(&responseValue[0], data, responseValueSize);
        B1String responseString(&responseValue[0]);
        response = static_cast<B1SMTPProtocol::RESPONSE>(responseString.toInt32());
        (*pos) += (responseValueSize);
    }
    catch (...) {
        B1LOG("unknown response value: value[%c%c%c]", data[0], data[1], data[2]);
        return ANALYZE_RESULT_FAIL;
    }
    B1String message;
    try {
        int32 messageLength = -1;
        for (size_t i = responseValueSize + 1; i < size; ++i) {
            if (data[i - 1] == '\r' && data[i] == '\n') {
                messageLength = static_cast<int32>(i) - responseValueSize - 1;
                break;
            }
        }
        if (messageLength < 0) {
            return ANALYZE_RESULT_NOT_ENOUTH_DATA;
        }
        std::vector<char> messageValue(messageLength + 1, 0);
        memcpy(&messageValue[0], data + responseValueSize, messageLength);
        message.from(&messageValue[0]);
        (*pos) += (messageLength + 2);
    }
    catch (...) {
        B1LOG("unknown message value");
        return ANALYZE_RESULT_FAIL;
    }

    switch (response) {
        case B1SMTPProtocol::RESPONSE_SUCCESS:
            implOnRecvSMTPResponseSuccess(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_STATUS:
            implOnRecvSMTPResponseStatus(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_HELP:
            implOnRecvSMTPResponseHelp(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_SERVICE_READY:
            implOnRecvSMTPResponseServiceReady(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_SERVICE_CLOSING:
            implOnRecvSMTPResponseServiceClosing(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_AUTH_COMPLETE:
            implOnRecvSMTPResponseAuthComplete(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_ACTION_OK:
            implOnRecvSMTPResponseActionOK(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_USER_NOT_LOCAL_WILL_FW:
            implOnRecvSMTPResponseUserNotLocalWillFW(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_CANNOT_VERIFY_BUT_WILL_ACCEPT:
            implOnRecvSMTPResponseCannotVerifyButWillAccept(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_AUTH_READY:
            implOnRecvSMTPResponseAuthReady(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_START_MAIL_INPUT:
            implOnRecvSMTPResponseStartMailInput(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_SERVICE_NOT_AVAILABLE:
            implOnRecvSMTPResponseServiceNotAvailable(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_MAIL_BOX_UNAVAILABLE:
            implOnRecvSMTPResponseMailBoxUnavailable(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_LOCAL_ERROR:
            implOnRecvSMTPResponseLocalError(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_INSUFFICIENT_STORAGE:
            implOnRecvSMTPResponseInsufficientStorage(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_SYNTAX_ERR_BAD_COMMAND:
            implOnRecvSMTPResponseSyntaxErrBadCommand(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_SYNTAX_ERR_BAD_PARAM:
            implOnRecvSMTPResponseSyntaxErrBadParam(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_CMD_NOT_IMPLEMENTED:
            implOnRecvSMTPResponseCmdNotImplemented(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_CMD_BAD_SEQUENCE:
            implOnRecvSMTPResponseCmdBadSequence(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_CMD_PARAM_NOT_IMPLEMENTED:
            implOnRecvSMTPResponseCmdParamNotImplemented(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_NOT_ACCEPTED_MAIL:
            implOnRecvSMTPResponseNotAcceptedMail(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_ACCESS_DENIED:
            implOnRecvSMTPResponseAccessDenied(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_AUTH_FAILED:
            implOnRecvSMTPResponseAuthFailed(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_NO_SUCH_USER:
            implOnRecvSMTPResponseNoSuchUser(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_USER_NOT_LOCAL_TRY_FW:
            implOnRecvSMTPResponseUserNotLocalTryFW(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_EXCEEDED_STORAGE:
            implOnRecvSMTPResponseExceededStorage(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_USER_AMBIGUOUS:
            implOnRecvSMTPResponseUserAmbiguous(std::move(message));
            break;
        case B1SMTPProtocol::RESPONSE_TRANSACTION_FAILED:
            implOnRecvSMTPResponseTransactionFailed(std::move(message));
            break;
        default:
            B1LOG("Unknown type: response[%d], size[%d], recvdBuffer_size[%d]", response, size, _recvdBuffer.size());
            assert(false);
            return ANALYZE_RESULT_FAIL;
    }
    return ANALYZE_RESULT_SUCCESS;
}
