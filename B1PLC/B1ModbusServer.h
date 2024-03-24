//
// B1ModbusServer.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_SERVER_H
#define _B1PLC_MODBUS_SERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseServer.h>

namespace BnD {
    class B1ModbusServerMemoryManager;
    class B1ModbusServer : protected B1BaseServer {
    public:
        B1ModbusServer();
        virtual ~B1ModbusServer();
    protected:
        std::shared_ptr<B1ModbusServerMemoryManager> _memoryManager;
    protected:
        virtual B1BaseServerSession* createSession(B1ServerSocket* serverSocket) override;
    public:
        bool initialize(uint16 port, uint16 coilSize, uint16 holdingRegisterSize);
        void finalize();

    };
}   //  !BnD

#endif  // !_B1PLC_MODBUS_SERVER_H
