//
// B1GEMEvent.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_EVENT_H
#define _B1GEM_EVENT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2DataTypes.h>

namespace BnD {
    class B1GEMEvent {
    protected:
        B1GEMEvent(const B1GEMEvent& evt, bool deepCopy);
    public:
        B1GEMEvent(const B1SECS2DataCEID& ceID, const B1SECS2DataCENAME& name);
        virtual ~B1GEMEvent();
    protected:
        std::shared_ptr<const B1SECS2DataCEID> _ceID;
        std::shared_ptr<const B1SECS2DataCENAME> _ceName;
        std::vector<std::shared_ptr<B1SECS2DataRPTID> > _reportIDs;
    public:
        bool operator<(const B1GEMEvent& e) const { return *_ceID < *e._ceID; }
        virtual B1GEMEvent* clone() { return new B1GEMEvent(*this, true); }
    public:
        const B1SECS2DataCEID& ceID() const { return *_ceID; }
        const B1SECS2DataCENAME& ceName() const { return *_ceName; }
              std::vector<std::shared_ptr<B1SECS2DataRPTID> >& reportIDs()       { return _reportIDs; }
        const std::vector<std::shared_ptr<B1SECS2DataRPTID> >& reportIDs() const { return _reportIDs; }
    };

    typedef std::shared_ptr<B1GEMEvent> SPGEMEvent;
}   //  !BnD

#endif  // !_B1GEM_EVENT_H
