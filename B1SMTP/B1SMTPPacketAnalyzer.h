//
// B1SMTPPacketAnalyzer.h
//
// Library: B1SMTP
// Package: B1SMTP
// Module:  B1SMTP
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SMTP_SMTP_PACKET_ANALYZER_H
#define _B1SMTP_SMTP_PACKET_ANALYZER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BasePacketAnalyzer.h>

namespace BnD {
    class B1SMTPPacketAnalyzer : public B1BasePacketAnalyzer {
    protected:
        virtual void implOnRecvSMTPResponseSuccess(B1String&& message) {}
        virtual void implOnRecvSMTPResponseStatus(B1String&& message) {}
        virtual void implOnRecvSMTPResponseHelp(B1String&& message) {}
        virtual void implOnRecvSMTPResponseServiceReady(B1String&& message) {}
        virtual void implOnRecvSMTPResponseServiceClosing(B1String&& message) {}
        virtual void implOnRecvSMTPResponseAuthComplete(B1String&& message) {}
        virtual void implOnRecvSMTPResponseActionOK(B1String&& message) {}
        virtual void implOnRecvSMTPResponseUserNotLocalWillFW(B1String&& message) {}
        virtual void implOnRecvSMTPResponseCannotVerifyButWillAccept(B1String&& message) {}
        virtual void implOnRecvSMTPResponseAuthReady(B1String&& message) {}
        virtual void implOnRecvSMTPResponseStartMailInput(B1String&& message) {}
        virtual void implOnRecvSMTPResponseServiceNotAvailable(B1String&& message) {}
        virtual void implOnRecvSMTPResponseMailBoxUnavailable(B1String&& message) {}
        virtual void implOnRecvSMTPResponseLocalError(B1String&& message) {}
        virtual void implOnRecvSMTPResponseInsufficientStorage(B1String&& message) {}
        virtual void implOnRecvSMTPResponseSyntaxErrBadCommand(B1String&& message) {}
        virtual void implOnRecvSMTPResponseSyntaxErrBadParam(B1String&& message) {}
        virtual void implOnRecvSMTPResponseCmdNotImplemented(B1String&& message) {}
        virtual void implOnRecvSMTPResponseCmdBadSequence(B1String&& message) {}
        virtual void implOnRecvSMTPResponseCmdParamNotImplemented(B1String&& message) {}
        virtual void implOnRecvSMTPResponseNotAcceptedMail(B1String&& message) {}
        virtual void implOnRecvSMTPResponseAccessDenied(B1String&& message) {}
        virtual void implOnRecvSMTPResponseAuthFailed(B1String&& message) {}
        virtual void implOnRecvSMTPResponseNoSuchUser(B1String&& message) {}
        virtual void implOnRecvSMTPResponseUserNotLocalTryFW(B1String&& message) {}
        virtual void implOnRecvSMTPResponseExceededStorage(B1String&& message) {}
        virtual void implOnRecvSMTPResponseUserAmbiguous(B1String&& message) {}
        virtual void implOnRecvSMTPResponseTransactionFailed(B1String&& message) {}
    protected:
        ANALYZE_RESULT implAnalyzeData(uint8* data, size_t size, size_t* pos) final;
    protected:
        ANALYZE_RESULT analyzeSMTPResponse(uint8* data, size_t size, size_t* pos);
    };
}   //  !BnD

#endif  // !_B1SMTP_SMTP_PACKET_ANALYZER_H
