//
// B1SECS2ClientSession.cpp
//
// Library: B1SECS2
// Package: Client
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2ClientSession.h"
#include "B1SECS2MessageWritable.h"

#include <B1Base/B1Endian.h>
#include <B1Base/B1TickUtil.h>

using namespace BnD;
using namespace B1SECS2Consts;

B1SECS2ClientSession::B1SECS2ClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, uint16 secs2SessionID, const B1SECS2DataManager* secs2DataManager)
    : B1ArrayBufferClientSession(clientSocket, listener)
    , _initialSystemByte(0)
    , _secs2SessionID(secs2SessionID)
    , _secs2DataManager(secs2DataManager)
    , _lastMessage()
    , _t7Checker()
    , _aliveChecker()
    , _lastAliveRequestTick(0)
{
}

void B1SECS2ClientSession::recvHSMSMessage(const std::vector<uint8>& data)
{
    const size_t headerSize = 10;
    if (data.size() < headerSize) {
        B1LOG("[HSMS]header length is too short");
        disconnect();
        return;
    }
    uint16 sessionID = 0;
    memcpy(&sessionID, &data[0], sizeof(sessionID));
    sessionID = TO_UINT16_FOR_NETWORK(sessionID);   //  15bit integer
    bool wait = data[2] & (0x01 << 7);
    uint8 stream = data[2] & (0xFF >> 1);
    uint8 function = data[3];
    //uint8 ptype = data[4];  //  0 only in HSMS;
    uint8 stype = data[5];
    std::vector<uint8> systemBytes(4, 0);
    memcpy(&systemBytes[0], &data[6], systemBytes.size());
    if (stype > 0) {
        recvHSMSControl(sessionID, systemBytes, static_cast<CONTROL_MESSAGE>(stype));
    }
    else {
        std::vector<uint8> messageHeader(headerSize, 0);
        memcpy(&messageHeader[0], &data[0], headerSize);
        if (sessionID != _secs2SessionID) {
            B1LOG("Unknown Device ID: expected[%u], real[%u]", _secs2SessionID, sessionID);
        }
        else {
            if (data.size() > headerSize)
                recvHSMSData(sessionID, stream, function, wait, systemBytes, &data[headerSize], static_cast<uint32>(data.size() - headerSize), messageHeader);
            else
                recvHSMSData(sessionID, stream, function, wait, systemBytes, NULL, 0, messageHeader);
        }
    }
}

void B1SECS2ClientSession::recvHSMSControl(uint16 sessionID, const std::vector<uint8>& systemBytes, CONTROL_MESSAGE controlMessage)
{
    B1LOG("recvHSMSControl: controlMessage[%d]", controlMessage);
    if (_secs2SessionID != sessionID) {
        B1LOG("invalid sessionID received -> disconnect: _secs2SessionID[%d], recved_sessionID[%d]", _secs2SessionID, sessionID);
        disconnect();
        return;
    }
    switch (controlMessage) {
        case CONTROL_MESSAGE_SELECT_RSP:
            _t7Checker.stop();
            _aliveChecker.start(TIME_OUT_ALIVE);
            onSelectCompleted();
            break;
        case CONTROL_MESSAGE_DESELECT_RSP:
            _aliveChecker.stop();
            _t7Checker.restart();
            onDeselectCompleted();
            break;
        case CONTROL_MESSAGE_LINK_TEST_RSP:
            _aliveChecker.reset();
            break;
        case CONTROL_MESSAGE_SEPERATE_REQ:
            B1LOG("recv CONTROL_MESSAGE_SEPERATE_REQ -> disconnect: _secs2SessionID:[%d]", _secs2SessionID);
            disconnect();
            break;
        default:
            break;
    }
}

void B1SECS2ClientSession::recvHSMSData(uint16 sessionID, uint8 stream, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength, const std::vector<uint8>& messageHeader)
{
    B1LOG("recvHSMSData: S[%u]F[%u]", stream, function);
    if (onRecvHSMSData(stream, function) != true) {
        B1LOG("can not proceed: S[%u]F[%u] -> sendMessageF0", stream, function);
        sendMessageF0(stream, sessionID, systemBytes);
        return;
    }
    switch (stream) {
        case 1:
            if (recvHSMSDataStream1(sessionID, function, wait, systemBytes, data, dataLength) != true) {
                B1LOG("recvHSMSDataStream1 failed: S[%u]F[%u]", stream, function);
            }
            break;
        case 2:
            if (recvHSMSDataStream2(sessionID, function, wait, systemBytes, data, dataLength) != true) {
                B1LOG("recvHSMSDataStream2 failed: S[%u]F[%u]", stream, function);
            }
            break;
        case 5:
            if (recvHSMSDataStream5(sessionID, function, wait, systemBytes, data, dataLength) != true) {
                B1LOG("recvHSMSDataStream5 failed: S[%u]F[%u]", stream, function);
            }
            break;
        case 6:
            if (recvHSMSDataStream6(sessionID, function, wait, systemBytes, data, dataLength) != true) {
                B1LOG("recvHSMSDataStream6 failed: S[%u]F[%u]", stream, function);
            }
            break;
        case 64:
            if (recvHSMSDataStream64(sessionID, function, wait, systemBytes, data, dataLength) != true) {
                B1LOG("recvHSMSDataStream64 failed: S[%u]F[%u]", stream, function);
            }
            break;
        default:
            B1LOG("recvMessageUndefined: S[%u]F[%u]", stream, function);
            break;
    }
}

bool B1SECS2ClientSession::recvHSMSDataStream1(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 0:
            recvMessageS1F0(sessionID, systemBytes);
            break;
        case 1:
            recvMessageS1F1(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 2:
            recvMessageS1F2(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 4:
            recvMessageS1F4(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 14:
            recvMessageS1F14(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 16:
            recvMessageS1F16(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 18:
            recvMessageS1F18(sessionID, wait, systemBytes, data, dataLength);
            break;
        default:
            B1LOG("not implemented yet: stream[1], function[%d]", function);
            assert(false);
            return false;
    };
    return true;
}

bool B1SECS2ClientSession::recvHSMSDataStream2(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 0:
            recvMessageS2F0(sessionID, systemBytes);
            break;
        case 32:
            recvMessageS2F32(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 34:
            recvMessageS2F34(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 36:
            recvMessageS2F36(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 38:
            recvMessageS2F38(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 42:
            recvMessageS2F42(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 50:
            recvMessageS2F50(sessionID, wait, systemBytes, data, dataLength);
            break;
        default:
            B1LOG("not implemented yet: stream[2], function[%d]", function);
            assert(false);
            return false;
    }
    return true;
}

bool B1SECS2ClientSession::recvHSMSDataStream5(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 1:
            recvMessageS5F1(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 4:
            recvMessageS5F4(sessionID, wait, systemBytes, data, dataLength);
            break;
        default:
            B1LOG("not implemented yet: stream[64], function[%d]", function);
            return false;
    }
    return true;
}

bool B1SECS2ClientSession::recvHSMSDataStream6(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 11:
            recvMessageS6F11(sessionID, wait, systemBytes, data, dataLength);
            break;
        default:
            B1LOG("not implemented yet: stream[6], function[%d]", function);
            assert(false);
            return false;
    };
    return true;
}

bool B1SECS2ClientSession::recvHSMSDataStream64(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 2:
            recvMessageS64F2(sessionID, wait, systemBytes, data, dataLength);
            break;
        default:
            B1LOG("not implemented yet: stream[64], function[%d]", function);
            return false;
    };
    return true;
}

void B1SECS2ClientSession::makeMessageHeader(uint16 sessionID, uint8 stream, uint8 function, bool wait, uint8 stype, const std::vector<uint8>& systemBytes, std::vector<uint8>* result)
{
    sessionID = TO_UINT16_FOR_NETWORK(sessionID);   //  15bit integer
    result->insert(result->end(), (uint8*)&sessionID, (uint8*)&sessionID + sizeof(sessionID));
    result->push_back((wait ? 0x01 << 7 : 0) | stream);
    result->push_back(function);
    result->push_back(0);   //  ptype
    result->push_back(stype);
    result->insert(result->end(), systemBytes.begin(), systemBytes.end());
}

std::vector<uint8> B1SECS2ClientSession::makeNewSystemBytes()
{
    ++_initialSystemByte;
    size_t size = sizeof(_initialSystemByte);
    return std::vector<uint8>((uint8*)&_initialSystemByte, (uint8*)&_initialSystemByte + size);
}

bool B1SECS2ClientSession::toUint32FromSystemBytes(const std::vector<uint8>& systemBytes, int32* result) const
{
    const size_t systemByteSize = 4;
    if (systemBytes.size() != systemByteSize) {
        return false;
    }
    int32 systemByte = 0;
    memcpy(&systemByte, &systemBytes[0], systemByteSize);
    systemByte = TO_UINT32_FOR_NETWORK(systemByte);
    *result = systemByte;
    return true;
}

void B1SECS2ClientSession::sendControlMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, CONTROL_MESSAGE controlMessage, bool wait)
{
    B1LOG("sendControlMessage: controlMessage[%d]", controlMessage);
    uint32 messageLenth = 10;
    std::vector<uint8> message;
    message.reserve(messageLenth + sizeof(messageLenth));
    messageLenth = TO_UINT32_FOR_NETWORK(messageLenth);
    message.insert(message.end(), (uint8*)&messageLenth, (uint8*)&messageLenth + sizeof(messageLenth));
    makeMessageHeader(sessionID, 0, 0, wait, controlMessage, systemBytes, &message);
    writeData(message);
}

void B1SECS2ClientSession::sendDataMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, uint8 stream, uint8 function, bool wait)
{
    int32 systemByte = 0;
    if (toUint32FromSystemBytes(systemBytes, &systemByte)) {
        B1LOG("sendDataMessage: S[%u]F[%u], systemByte[0x%X]", stream, function, systemByte);
    }
    else {
        B1LOG("sendDataMessage: S[%u]F[%u]", stream, function);
    }

    uint32 messageLenth = 10;
    std::vector<uint8> message;
    message.reserve(messageLenth + sizeof(messageLenth));
    messageLenth = TO_UINT32_FOR_NETWORK(messageLenth);
    message.insert(message.end(), (uint8*)&messageLenth, (uint8*)&messageLenth + sizeof(messageLenth));
    makeMessageHeader(sessionID, stream, function, wait, 0, systemBytes, &message);
    writeData(message);
}

void B1SECS2ClientSession::sendDataMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, uint8 stream, uint8 function, const std::vector<std::vector<uint8> >& items, bool wait)
{
    int32 systemByte = 0;
    if (toUint32FromSystemBytes(systemBytes, &systemByte)) {
        B1LOG("sendDataMessage: S[%u]F[%u], systemByte[0x%X], items[%u]", stream, function, systemByte, static_cast<uint32>(items.size()));
    }
    else {
        B1LOG("sendDataMessage: S[%u]F[%u], items[%u]", stream, function, static_cast<uint32>(items.size()));
    }
 
    uint32 messageLenth = 10;
    for (const auto& item : items) {
        messageLenth += static_cast<uint32>(item.size());
    }

    std::vector<uint8> message;
    message.reserve(messageLenth + sizeof(messageLenth));
    messageLenth = TO_UINT32_FOR_NETWORK(messageLenth);
    message.insert(message.end(), (uint8*)&messageLenth, (uint8*)&messageLenth + sizeof(messageLenth));
    makeMessageHeader(sessionID, stream, function, wait, 0, systemBytes, &message);
    for (const auto& item : items) {
        message.insert(message.end(), item.begin(), item.end());
    }
    writeData(message);
}

void B1SECS2ClientSession::sendDataMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2MessageWritable& message)
{
    if (message.items().empty())
        sendDataMessage(sessionID, systemBytes, message.stream(), message.function(), message.wait());
    else
        sendDataMessage(sessionID, systemBytes, message.stream(), message.function(), message.items(), message.wait());
}

void B1SECS2ClientSession::sendMessageF0(uint8 stream, uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    sendDataMessage(sessionID, systemBytes, stream, 0, false);
}

void B1SECS2ClientSession::implOnConnect()
{
    B1ArrayBufferClientSession::implOnConnect();
    srand((unsigned int)time(0));
    _initialSystemByte = rand();
    _lastMessage.reset();
    _t7Checker.start(TIME_OUT_T7);
    _aliveChecker.stop();
    _lastAliveRequestTick = B1TickUtil::currentTick();
}

void B1SECS2ClientSession::onReadComplete(uint8* data, size_t dataSize)
{
    if (0 == _lastMessage._recved) {
        const size_t messageLengthSize = 4;
        if (dataSize < messageLengthSize) {
            B1LOG("[HSMS]message length is too short:[%s]", peerAddress().cString());
            return;
        }
        memcpy(&_lastMessage._length, data, messageLengthSize);
        _lastMessage._length = TO_UINT32_FOR_NETWORK(_lastMessage._length);
        std::vector<uint8> buffer(dataSize - messageLengthSize, 0);
        memcpy(&buffer[0], (uint8*)data + messageLengthSize, dataSize - messageLengthSize);
        if (_lastMessage._length == buffer.size()) {
            recvHSMSMessage(buffer);
            _lastMessage.reset();
            return;
        }
        else if (_lastMessage._length > buffer.size()) {
            _lastMessage._data.insert(_lastMessage._data.end(), buffer.begin(), buffer.end());
            _lastMessage._recved += static_cast<uint32>(dataSize);
            return;
        }
        else if (_lastMessage._length < buffer.size()) {
            std::vector<uint8> temp(_lastMessage._length, 0);
            memcpy(&temp[0], &buffer[0], _lastMessage._length);
            recvHSMSMessage(temp);
            uint32 remainIndex = _lastMessage._length;
            _lastMessage.reset();
            onReadComplete(&buffer[remainIndex], buffer.size() - remainIndex);
            return;
        }
    }
    else {
        _lastMessage._data.insert(_lastMessage._data.end(), data, data + dataSize);
        if (_lastMessage._length == _lastMessage._data.size()) {
            recvHSMSMessage(_lastMessage._data);
            _lastMessage.reset();
            return;
        }
        else if (_lastMessage._length > _lastMessage._data.size()) {
            _lastMessage._recved += static_cast<uint32>(dataSize);
            return;
        }
        else if (_lastMessage._length < _lastMessage._data.size()) {
            std::vector<uint8> temp;
            temp.reserve(_lastMessage._length);
            temp.insert(temp.end(), _lastMessage._data.begin(), _lastMessage._data.begin() + _lastMessage._length);
            recvHSMSMessage(temp);
            temp.clear();
            temp.insert(temp.end(), _lastMessage._data.begin() + _lastMessage._length, _lastMessage._data.end());
            _lastMessage.reset();
            onReadComplete(&temp[0], temp.size());
            return;
        }
    }
}

void B1SECS2ClientSession::implProcessConnected(bool firstConnectedProcess)
{
    if (_t7Checker.isRunning() && _t7Checker.isTimeover(false)) {
        B1LOG("SELECT timedout: _secs2SessionID[%d]", _secs2SessionID);
        disconnect();
        return;
    }
    if (_aliveChecker.isRunning() && _aliveChecker.isTimeover(false)) {
        B1LOG("NETWORK timedout: _secs2SessionID[%d]", _secs2SessionID);
        disconnect();
        return;
    }
    if (firstConnectedProcess) {
        sendControlMessage(_secs2SessionID, makeNewSystemBytes(), CONTROL_MESSAGE_SELECT_REQ);
    }
    else {
        uint64 now = B1TickUtil::currentTick();
        if (B1TickUtil::diffTick(_lastAliveRequestTick, now) > CONSTS_REQ_ALIVE_INTERVAL) {
            _lastAliveRequestTick = now;
            sendControlMessage(_secs2SessionID, makeNewSystemBytes(), CONTROL_MESSAGE_LINK_TEST_REQ);
        }
    }
    B1ArrayBufferClientSession::implProcessConnected(firstConnectedProcess);
}
