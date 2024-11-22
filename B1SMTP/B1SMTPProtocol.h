//
// B1SMTPProtocol.h
//
// Library: B1SMTP
// Package: B1SMTP
// Module:  B1SMTP
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SMTP_SMTP_PROTOCOL_H
#define _B1SMTP_SMTP_PROTOCOL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1SMTPProtocol {
        enum RESPONSE {
            RESPONSE_UNKNOWN = 0,   //  for initialization.
            RESPONSE_SUCCESS = 200,
            RESPONSE_STATUS = 211,
            RESPONSE_HELP = 214,
            RESPONSE_SERVICE_READY = 220,
            RESPONSE_SERVICE_CLOSING = 221,
            RESPONSE_AUTH_COMPLETE = 235,
            RESPONSE_ACTION_OK = 250,
            RESPONSE_USER_NOT_LOCAL_WILL_FW = 251,
            RESPONSE_CANNOT_VERIFY_BUT_WILL_ACCEPT = 252,
            RESPONSE_AUTH_READY = 334,
            RESPONSE_START_MAIL_INPUT = 354,
            RESPONSE_SERVICE_NOT_AVAILABLE = 421,
            RESPONSE_MAIL_BOX_UNAVAILABLE = 450,
            RESPONSE_LOCAL_ERROR = 451,
            RESPONSE_INSUFFICIENT_STORAGE = 452,
            RESPONSE_SYNTAX_ERR_BAD_COMMAND = 500,
            RESPONSE_SYNTAX_ERR_BAD_PARAM = 501,
            RESPONSE_CMD_NOT_IMPLEMENTED = 502,
            RESPONSE_CMD_BAD_SEQUENCE = 503,
            RESPONSE_CMD_PARAM_NOT_IMPLEMENTED = 504,
            RESPONSE_NOT_ACCEPTED_MAIL = 521,
            RESPONSE_ACCESS_DENIED = 530,
            RESPONSE_AUTH_FAILED = 535,
            RESPONSE_NO_SUCH_USER = 550,
            RESPONSE_USER_NOT_LOCAL_TRY_FW = 551,
            RESPONSE_EXCEEDED_STORAGE = 552,
            RESPONSE_USER_AMBIGUOUS = 553,
            RESPONSE_TRANSACTION_FAILED = 554,
        };
    };
}   //  !BnD

#endif  // !_B1SMTP_SMTP_PROTOCOL_H
