//
// B1SECS2DataTypes.h
//
// Library: B1SECS2
// Package: B1SECS2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_DATA_TYPES_H
#define _B1SECS2_DATA_TYPES_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <list>

namespace BnD {
    class B1SECS2Data;
    class B1SECS2DataManager;
    class B1SECS2DataBase {
    public:
        enum INDEX {
            INDEX_ACKC5 = 0,
            INDEX_ACKC6,
            INDEX_ALCD,
            INDEX_ALED,
            INDEX_ALID,
            INDEX_ALTX,
            INDEX_CEED,
            INDEX_CEID,
            INDEX_CENAME,
            INDEX_CEPACK,
            INDEX_CEPVAL,
            INDEX_COMMACK,
            INDEX_CPACK,
            INDEX_CPNAME,
            INDEX_CPVAL,    //  CPVAL is just B1SECS2Data.
            INDEX_DATAID,
            INDEX_DRACK,
            INDEX_DVVALNAME,
            INDEX_EAC,
            INDEX_ECDEF,    //  ECDEF is just B1SECS2Data.
            INDEX_ECID,
            INDEX_ECMAX,    //  ECMAX is just B1SECS2Data.
            INDEX_ECMIN,    //  ECMIN is just B1SECS2Data.
            INDEX_ECNAME,
            INDEX_ECV,      //  ECV is just B1SECS2Data.
            INDEX_ERACK,
            INDEX_HCACK,
            INDEX_LRACK,
            INDEX_MDLN,
            INDEX_OBJSPEC,
            INDEX_OFLACK,
            INDEX_ONLACK,
            INDEX_RCMD,
            INDEX_RPTID,
            INDEX_SOFTREV,
            INDEX_SV,       //  SV: B1SECS2Data + list
            INDEX_SVID,
            INDEX_SVNAME,
            INDEX_TIACK,
            INDEX_TIME,
            INDEX_UNITS,
            INDEX_VID,
            INDEX_V,

            INDEX_SEPERATOR_END,
            INDEX_SEPERATOR_BEGIN = INDEX_ACKC5,
        };
    protected:
        B1SECS2DataBase(INDEX index, const B1SECS2DataManager* manager);
        B1SECS2DataBase(INDEX index, std::shared_ptr<B1SECS2Data> data);
        virtual ~B1SECS2DataBase();
    protected:
        const INDEX _index;
        std::shared_ptr<B1SECS2Data> _data;
    public:
        bool operator<(const B1SECS2DataBase& a) const;
              B1SECS2Data* operator->()       { return _data.get(); }
        const B1SECS2Data* operator->() const { return _data.get(); }
    public:
              B1SECS2Data* get()       { return _data.get(); }
        const B1SECS2Data* get() const { return _data.get(); }
        std::shared_ptr<B1SECS2Data> data() const { return _data; }
    public:
        virtual B1SECS2DataBase* clone() const;
    };
#define SECS2_DATA_TYPE_IMPL(d)\
    class B1SECS2Data##d : public B1SECS2DataBase { public:\
        B1SECS2Data##d(const B1SECS2DataManager* manager) : B1SECS2DataBase(INDEX_##d, manager) {}\
        B1SECS2Data##d(std::shared_ptr<B1SECS2Data> data) : B1SECS2DataBase(INDEX_##d, data) {}\
    }
#define SECS2_DATA_VAR_TYPE_IMPL(d) \
    class B1SECS2Data##d : public B1SECS2DataBase { public:\
        B1SECS2Data##d(std::shared_ptr<B1SECS2Data> data) : B1SECS2DataBase(INDEX_##d, data) {}\
    }
#define SECS2_DATA_LIST_TYPE_IMPL(d) \
    class B1SECS2Data##d : public B1SECS2DataBase { public:\
        B1SECS2Data##d(std::shared_ptr<B1SECS2Data> data) : B1SECS2DataBase(INDEX_##d, data) {}\
        std::list<std::shared_ptr<B1SECS2Data> > _list; /*중간에 LIST가 나와도 최종 개수는 변경 없다. LIST 존재 여부에 상관 없이 모든 아이템 전부 합친게 여기 들어감.*/ \
    }

    SECS2_DATA_TYPE_IMPL(ACKC5);
    SECS2_DATA_TYPE_IMPL(ACKC6);
    SECS2_DATA_TYPE_IMPL(ALCD);
    SECS2_DATA_TYPE_IMPL(ALED);
    SECS2_DATA_TYPE_IMPL(ALID);
    SECS2_DATA_TYPE_IMPL(ALTX);
    SECS2_DATA_TYPE_IMPL(CEED);
    SECS2_DATA_TYPE_IMPL(CEID);
    SECS2_DATA_TYPE_IMPL(CENAME);
    SECS2_DATA_TYPE_IMPL(CEPACK);
    SECS2_DATA_LIST_TYPE_IMPL(CEPVAL);  //  may be a scalar of any type, a list of values of the same type, or a list of possibly nested {L:2 CPNAME CEPVAL}.
    SECS2_DATA_TYPE_IMPL(COMMACK);
    SECS2_DATA_TYPE_IMPL(CPACK);
    SECS2_DATA_TYPE_IMPL(CPNAME);
    SECS2_DATA_VAR_TYPE_IMPL(CPVAL);
    SECS2_DATA_TYPE_IMPL(DATAID);
    SECS2_DATA_TYPE_IMPL(DRACK);
    SECS2_DATA_TYPE_IMPL(DVVALNAME);
    SECS2_DATA_TYPE_IMPL(EAC);
    SECS2_DATA_VAR_TYPE_IMPL(ECDEF);
    SECS2_DATA_TYPE_IMPL(ECID);
    SECS2_DATA_VAR_TYPE_IMPL(ECMAX);
    SECS2_DATA_VAR_TYPE_IMPL(ECMIN);
    SECS2_DATA_TYPE_IMPL(ECNAME);
    SECS2_DATA_VAR_TYPE_IMPL(ECV);
    SECS2_DATA_TYPE_IMPL(ERACK);
    SECS2_DATA_TYPE_IMPL(HCACK);
    SECS2_DATA_TYPE_IMPL(LRACK);
    SECS2_DATA_TYPE_IMPL(MDLN);
    SECS2_DATA_TYPE_IMPL(OBJSPEC);
    SECS2_DATA_TYPE_IMPL(OFLACK);
    SECS2_DATA_TYPE_IMPL(ONLACK);
    SECS2_DATA_TYPE_IMPL(RCMD);
    SECS2_DATA_TYPE_IMPL(RPTID);
    SECS2_DATA_TYPE_IMPL(SOFTREV);
    SECS2_DATA_LIST_TYPE_IMPL(SV);
    SECS2_DATA_TYPE_IMPL(SVID);
    SECS2_DATA_TYPE_IMPL(SVNAME);
    SECS2_DATA_TYPE_IMPL(TIACK);
    SECS2_DATA_TYPE_IMPL(TIME);
    SECS2_DATA_TYPE_IMPL(UNITS);
    SECS2_DATA_TYPE_IMPL(VID);
    SECS2_DATA_LIST_TYPE_IMPL(V);
}   //  !BnD

#endif  // !_B1SECS2_DATA_TYPES_H
