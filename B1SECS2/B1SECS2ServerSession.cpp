//
// B1SECS2ServerSession.cpp
//
// Library: B1SECS2
// Package: Server
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2ServerSession.h"
#include "B1SECS2Server.h"
#include "B1SECS2MessageWritable.h"

#include <B1Base/B1Endian.h>

#include <stdarg.h>

using namespace BnD;
using namespace B1SECS2Consts;

B1SECS2ServerSession::B1SECS2ServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener, uint16 secs2SessionID, B1SECS2Server* owner)
    : B1ArrayBufferServerSession(serverSocket, listener)
    , _initialSystemByte(0)
    , _secs2SessionID(secs2SessionID)
    , _owner(owner)
    , _t6Checker()
    , _t7Checker()
    , _t8Checker()
    , _selected(false)
    , _lastMessage()
{
}

B1SECS2ServerSession::~B1SECS2ServerSession()
{
}

void B1SECS2ServerSession::recvHSMSMessage(const std::vector<uint8>& data)
{
    _t8Checker.stop();
    const size_t headerSize = 10;
    if (data.size() < headerSize) {
        writeLog("[ERROR]header length is too short");
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
            writeLog("Unknown Device ID: expected[%u], real[%u]", _secs2SessionID, sessionID);
            sendMessageS9F1(sessionID, makeNewSystemBytes(), messageHeader);
        }
        else {
            if (data.size() > headerSize)
                recvHSMSData(sessionID, stream, function, wait, systemBytes, &data[headerSize], static_cast<uint32>(data.size() - headerSize), messageHeader);
            else
                recvHSMSData(sessionID, stream, function, wait, systemBytes, NULL, 0, messageHeader);
        }
    }
}

void B1SECS2ServerSession::recvHSMSControl(uint16 sessionID, const std::vector<uint8>& systemBytes, CONTROL_MESSAGE controlMessage)
{
    writeLog("recvHSMSControl: controlMessage[%d]", controlMessage);
    if (CONTROL_MESSAGE_SELECT_REQ == controlMessage) {
        _t7Checker.stop();
        _selected = true;
        sendControlMessage(sessionID, systemBytes, CONTROL_MESSAGE_SELECT_RSP);
        onSelectCompleted(sessionID, systemBytes);
        return;
    }
    if (_selected != true) {
        writeLog("not selected: request[%d]", controlMessage);
        disconnect();
        return;
    }
    switch (controlMessage) {
        case CONTROL_MESSAGE_DESELECT_REQ:
            _selected = false;
            _t7Checker.start(TIME_OUT_T7);
            sendControlMessage(sessionID, systemBytes, CONTROL_MESSAGE_DESELECT_RSP);
            onDeselectCompleted();
            break;
        case CONTROL_MESSAGE_LINK_TEST_REQ:
            sendControlMessage(sessionID, systemBytes, CONTROL_MESSAGE_LINK_TEST_RSP);
            break;
        case CONTROL_MESSAGE_REJECT_REQ:
            break;
        case CONTROL_MESSAGE_SEPERATE_REQ:
            writeLog("recv CONTROL_MESSAGE_SEPERATE_REQ -> disconnect");
            disconnect();
            break;
        default:
            break;
    }
}

void B1SECS2ServerSession::recvHSMSData(uint16 sessionID, uint8 stream, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength, const std::vector<uint8>& messageHeader)
{
    writeLog("recvHSMSData: S[%u]F[%u]", stream, function);
    if (_selected != true) {
        writeLog("not selected: S[%u]F[%u]", stream, function);
        disconnect();
        return;
    }
    if (onRecvHSMSData(stream, function) != true) {
        writeLog("can not proceed: S[%u]F[%u] -> sendMessageF0", stream, function);
        sendMessageF0(stream, sessionID, systemBytes);
        return;
    }
    switch (stream) {
        case 1:
            if (recvHSMSDataStream1(sessionID, function, wait, systemBytes, data, dataLength) != true) {
                writeLog("recvHSMSDataStream1 failed: S[%u]F[%u]", stream, function);
                sendMessageS9F5(sessionID, makeNewSystemBytes(), messageHeader);
            }
            break;
        case 2:
            if (recvHSMSDataStream2(sessionID, function, wait, systemBytes, data, dataLength) != true) {
                writeLog("recvHSMSDataStream2 failed: S[%u]F[%u]", stream, function);
                sendMessageS9F5(sessionID, makeNewSystemBytes(), messageHeader);
            }
            break;
        case 5:
            if (recvHSMSDataStream5(sessionID, function, wait, systemBytes, data, dataLength) != true) {
                writeLog("recvHSMSDataStream5 failed: S[%u]F[%u]", stream, function);
                sendMessageS9F5(sessionID, makeNewSystemBytes(), messageHeader);
            }
            break;
        case 6:
            if (recvHSMSDataStream6(sessionID, function, wait, systemBytes, data, dataLength) != true) {
                writeLog("recvHSMSDataStream6 failed: S[%u]F[%u]", stream, function);
                sendMessageS9F5(sessionID, makeNewSystemBytes(), messageHeader);
            }
            break;
        case 64:
            if (recvHSMSDataStream64(sessionID, function, wait, systemBytes, data, dataLength) != true) {
                writeLog("recvHSMSDataStream64 failed: S[%u]F[%u]", stream, function);
                sendMessageS9F5(sessionID, makeNewSystemBytes(), messageHeader);
            }
            break;
        default:
            writeLog("recvMessageUndefined: S[%u]F[%u]", stream, function);
            sendMessageS9F3(sessionID, makeNewSystemBytes(), messageHeader);
            break;
    }
}

bool B1SECS2ServerSession::recvHSMSDataStream1(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
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
        case 3:
            recvMessageS1F3(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 11:
            recvMessageS1F11(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 13:
            recvMessageS1F13(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 14:
            recvMessageS1F14(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 15:
            recvMessageS1F15(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 17:
            recvMessageS1F17(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 21:
            recvMessageS1F21(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 23:
            recvMessageS1F23(sessionID, wait, systemBytes, data, dataLength);
            break;
        default:
            return false;
    };
    return true;
}

bool B1SECS2ServerSession::recvHSMSDataStream2(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 0:
            recvMessageS2F0(sessionID, systemBytes);
            break;
        case 13:
            recvMessageS2F13(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 15:
            recvMessageS2F15(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 17:
            recvMessageS2F17(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 29:
            recvMessageS2F29(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 31:
            recvMessageS2F31(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 33:
            recvMessageS2F33(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 35:
            recvMessageS2F35(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 37:
            recvMessageS2F37(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 41:
            recvMessageS2F41(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 49:
            recvMessageS2F49(sessionID, wait, systemBytes, data, dataLength);
            break;
        default:
            return false;
    }
    return true;
}

bool B1SECS2ServerSession::recvHSMSDataStream5(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 0:
            recvMessageS5F0(sessionID, systemBytes);
            break;
        case 2:
            recvMessageS5F2(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 3:
            recvMessageS5F3(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 5:
            recvMessageS5F5(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 7:
            recvMessageS5F7(sessionID, wait, systemBytes, data, dataLength);
            break;
        default:
            return false;
    }
    return true;
}

bool B1SECS2ServerSession::recvHSMSDataStream6(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 0:
            recvMessageS6F0(sessionID, systemBytes);
            break;
        case 12:
            recvMessageS6F12(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 15:
            recvMessageS6F15(sessionID, wait, systemBytes, data, dataLength);
            break;
        case 19:
            recvMessageS6F19(sessionID, wait, systemBytes, data, dataLength);
            break;
        default:
            return false;
    };
    return true;
}

bool B1SECS2ServerSession::recvHSMSDataStream64(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 0:
            recvMessageS64F0(sessionID, systemBytes);
            break;
        case 1:
            recvMessageS64F1(sessionID, wait, systemBytes, data, dataLength);
            break;
        default:
            return false;
    };
    return true;
}

void B1SECS2ServerSession::makeMessageHeader(uint16 sessionID, uint8 stream, uint8 function, bool wait, uint8 stype, const std::vector<uint8>& systemBytes, std::vector<uint8>* result)
{
    sessionID = TO_UINT16_FOR_NETWORK(sessionID);   //  15bit integer
    result->insert(result->end(), (uint8*)&sessionID, (uint8*)&sessionID + sizeof(sessionID));
    result->push_back((wait ? 0x01 << 7 : 0) | stream);
    result->push_back(function);
    result->push_back(0);   //  ptype
    result->push_back(stype);
    result->insert(result->end(), systemBytes.begin(), systemBytes.end());
}

void B1SECS2ServerSession::writeLog(const char* format, ...)
{
    B1String s;
    s.format("%s: [%s:%d] ", (__CLASS_NAME__).c_str(), peerAddress().cString(), localPort());
    va_list args;
    va_start(args, format);
    s.appendvf(format, args);
    va_end(args);
    b1log(s);
}

const B1SECS2DataManager* B1SECS2ServerSession::secs2DataManager() const
{
    return _owner->secs2DataManager();
}

std::vector<uint8> B1SECS2ServerSession::makeNewSystemBytes()
{
    ++_initialSystemByte;
    size_t size = sizeof(_initialSystemByte);
    return std::vector<uint8>((uint8*)&_initialSystemByte, (uint8*)&_initialSystemByte + size);
}

bool B1SECS2ServerSession::toUint32FromSystemBytes(const std::vector<uint8>& systemBytes, int32* result) const
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

void B1SECS2ServerSession::sendControlMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, CONTROL_MESSAGE controlMessage, bool wait)
{
    writeLog("sendControlMessage: controlMessage[%d]", controlMessage);
    uint32 messageLenth = 10;
    std::vector<uint8> message;
    message.reserve(messageLenth + sizeof(messageLenth));
    messageLenth = TO_UINT32_FOR_NETWORK(messageLenth);
    message.insert(message.end(), (uint8*)&messageLenth, (uint8*)&messageLenth + sizeof(messageLenth));
    makeMessageHeader(sessionID, 0, 0, wait, controlMessage, systemBytes, &message);
    writeData(message);
}

void B1SECS2ServerSession::sendDataMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, uint8 stream, uint8 function, bool wait)
{
    int32 systemByte = 0;
    if (toUint32FromSystemBytes(systemBytes, &systemByte)) {
        writeLog("sendDataMessage: S[%u]F[%u], systemByte[0x%X]", stream, function, systemByte);
    }
    else {
        writeLog("sendDataMessage: S[%u]F[%u]", stream, function);
    }

    uint32 messageLenth = 10;
    std::vector<uint8> message;
    message.reserve(messageLenth + sizeof(messageLenth));
    messageLenth = TO_UINT32_FOR_NETWORK(messageLenth);
    message.insert(message.end(), (uint8*)&messageLenth, (uint8*)&messageLenth + sizeof(messageLenth));
    makeMessageHeader(sessionID, stream, function, wait, 0, systemBytes, &message);
    writeData(message);
}

void B1SECS2ServerSession::sendDataMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, uint8 stream, uint8 function, const std::vector<std::vector<uint8> >& items, bool wait)
{
    int32 systemByte = 0;
    if (toUint32FromSystemBytes(systemBytes, &systemByte)) {
        writeLog("sendDataMessage: S[%u]F[%u], systemByte[0x%X], items[%u]", stream, function, systemByte, static_cast<uint32>(items.size()));
    }
    else {
        writeLog("sendDataMessage: S[%u]F[%u], items[%u]", stream, function, static_cast<uint32>(items.size()));
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

void B1SECS2ServerSession::sendDataMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2MessageWritable& message)
{
    if (message.items().empty()) {
        sendDataMessage(sessionID, systemBytes, message.stream(), message.function(), message.wait());
    }
    else {
        sendDataMessage(sessionID, systemBytes, message.stream(), message.function(), message.items(), message.wait());
    }
}

void B1SECS2ServerSession::sendMessageF0(uint8 stream, uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    sendDataMessage(sessionID, systemBytes, stream, 0, false);
}

void B1SECS2ServerSession::implOnConnect()
{
    B1ArrayBufferServerSession::implOnConnect();
    srand((unsigned int)time(0));
    _initialSystemByte = rand();
    _lastMessage.reset();
    _selected = false;
    _t6Checker.start(TIME_OUT_T6);
    _t6Checker.stop();
    _t7Checker.start(TIME_OUT_T7);
    _t8Checker.start(TIME_OUT_T8);
    _t8Checker.stop();
}

void B1SECS2ServerSession::onReadComplete(uint8* data, size_t dataSize)
{
    if (0 == _lastMessage._recved) {
        _t8Checker.reset();
        const size_t messageLengthSize = 4;
        if (dataSize < messageLengthSize) {
            writeLog("[HSMS]message length is too short:[%s]", peerAddress().cString());
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

void B1SECS2ServerSession::implProcessConnected(bool firstConnectedProcess)
{
    B1ArrayBufferServerSession::implProcessConnected(firstConnectedProcess);
    if (_selected != true) {
        if (_t7Checker.isTimeover(false)) {
            writeLog("T7 Timed out");
            disconnect();
            return;
        }
    }
    if (_t6Checker.isTimeover(false)) {
        writeLog("T6 Timed out");
        disconnect();
        return;
    }
    if (_t8Checker.isTimeover(false)) {
        writeLog("T8 Timed out");
        disconnect();
        return;
    }
}
