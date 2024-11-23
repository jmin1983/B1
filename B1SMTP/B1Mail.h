//
// B1Mail.h
//
// Library: B1SMTP
// Package: B1SMTP
// Module:  B1SMTP
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SMTP_MAIL_H
#define _B1SMTP_MAIL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <list>

namespace BnD {
    class B1Mail {
    public:
        B1Mail();
        virtual ~B1Mail();
    public:
        struct B1Participant {
            B1Participant() {}
            B1Participant(B1Participant&& r) noexcept : _emailAddress(std::move(r._emailAddress)), _name(std::move(r._name)) {}
            B1Participant(B1String&& emailAddress) : _emailAddress(std::move(emailAddress)) {}
            B1Participant(B1String&& emailAddress, B1String&& name) : _emailAddress(std::move(emailAddress)), _name(std::move(name)) {}
            B1String _emailAddress;
            B1String _name;
            const B1String& emailAddress() { return _emailAddress; }
            const B1String& name() { return _name; }
            void setEmailAddress(B1String&& value) { _emailAddress = std::move(value); }
            void setName(B1String&& value) { _name = std::move(value); }
        };
    protected:
        B1Participant _sender;
        std::list<B1Participant> _receivers;
        std::list<B1Participant> _ccs;
        std::list<B1Participant> _bccs;
        B1String _subject;
        B1String _contents;
    public:
        const B1Participant& sender() const { return _sender; }
        const std::list<B1Participant>& receivers() const { return _receivers; }
        const std::list<B1Participant>& ccs() const { return _ccs; }
        const std::list<B1Participant>& bccs() const { return _bccs; }
        const B1String& subject() const { return _subject; }
        const B1String& contents() const { return _contents; }

        void setSender(B1String&& emailAddress, B1String&& name) { _sender.setEmailAddress(std::move(emailAddress)), _sender.setName(std::move(name)); }
        void addReceiver(B1String&& emailAddress, B1String&& name) { _receivers.push_back(B1Participant(std::move(emailAddress), std::move(name))); }
        void addCC(B1String&& emailAddress, B1String&& name) { _ccs.push_back(B1Participant(std::move(emailAddress), std::move(name))); }
        void addBCC(B1String&& emailAddress, B1String&& name) { _bccs.push_back(B1Participant(std::move(emailAddress), std::move(name))); }
        void setSubject(B1String&& value) { _subject = std::move(value); }
        void setContents(B1String&& value) { _contents = std::move(value); }
    };
}   //  !BnD

#endif  // !_B1SMTP_MAIL_H
