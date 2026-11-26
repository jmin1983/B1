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

B1SECS2ClientSession::B1SECS2ClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener)
    : B1ArrayBufferClientSession(clientSocket, listener)
    , _initialSystemByte(0)
    , _clientSecs2DataManager()
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
        if (sessionID != deviceID()) {
            B1LOG("Unknown Device ID: expected[%u], real[%u]", deviceID(), sessionID);
        }
        else {
            if (data.size() > headerSize) {
                recvHSMSData(stream, function, wait, systemBytes, &data[headerSize], static_cast<uint32>(data.size() - headerSize), messageHeader);
            }
            else {
                recvHSMSData(stream, function, wait, systemBytes, nullptr, 0, messageHeader);
            }
        }
    }
}

void B1SECS2ClientSession::recvHSMSControl(uint16 sessionID, const std::vector<uint8>& systemBytes, CONTROL_MESSAGE controlMessage)
{
    B1LOG("recvHSMSControl: controlMessage[%d]", controlMessage);
    if (deviceID() != sessionID && 0xFFFF != sessionID) {  //  0xFFFF: session_id of Linktest in E37.
        B1LOG("invalid sessionID received -> disconnect: deviceID[%d], recved_sessionID[%d]", deviceID(), sessionID);
        disconnect();
        return;
    }
    switch (controlMessage) {
        case CONTROL_MESSAGE_SELECT_RSP:
            _t7Checker.stop();
            _aliveChecker.start(CONSTS_ALIVE_TIME_OUT);
            onSelectCompleted();
            break;
        case CONTROL_MESSAGE_DESELECT_RSP:
            _aliveChecker.stop();
            _t7Checker.restart();
            onDeselectCompleted();
            break;
        case CONTROL_MESSAGE_LINK_TEST_REQ:
            sendControlMessage(systemBytes, CONTROL_MESSAGE_LINK_TEST_RSP);
            break;
        case CONTROL_MESSAGE_LINK_TEST_RSP:
            _aliveChecker.reset();
            break;
        case CONTROL_MESSAGE_SEPERATE_REQ:
            B1LOG("recv CONTROL_MESSAGE_SEPERATE_REQ -> disconnect: deviceID:[%d]", deviceID());
            disconnect();
            break;
        default:
            break;
    }
}

void B1SECS2ClientSession::recvHSMSData(uint8 stream, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength, const std::vector<uint8>& messageHeader)
{
    {
        int32 systemByte = 0;
        if (toUint32FromSystemBytes(systemBytes, &systemByte)) {
            B1LOG("recvHSMSData: S[%u]F[%u], systemByte[0x%X]", stream, function, systemByte);
        }
        else {
            B1LOG("recvHSMSData: S[%u]F[%u]", stream, function);
        }
    }

    if (onRecvHSMSData(stream, function) != true) {
        B1LOG("can not proceed: S[%u]F[%u] -> sendMessageF0", stream, function);
        sendMessageF0(stream, systemBytes);
        return;
    }
    switch (stream) {
        case 1:
            if (recvHSMSDataStream1(function, wait, systemBytes, data, dataLength) != true) {
                B1LOG("recvHSMSDataStream1 failed: S[%u]F[%u]", stream, function);
            }
            break;
        case 2:
            if (recvHSMSDataStream2(function, wait, systemBytes, data, dataLength) != true) {
                B1LOG("recvHSMSDataStream2 failed: S[%u]F[%u]", stream, function);
            }
            break;
        case 3:
            if (recvHSMSDataStream3(function, wait, systemBytes, data, dataLength) != true) {
                B1LOG("recvHSMSDataStream3 failed: S[%u]F[%u]", stream, function);
            }
            break;
        case 5:
            if (recvHSMSDataStream5(function, wait, systemBytes, data, dataLength) != true) {
                B1LOG("recvHSMSDataStream5 failed: S[%u]F[%u]", stream, function);
            }
            break;
        case 6:
            if (recvHSMSDataStream6(function, wait, systemBytes, data, dataLength) != true) {
                B1LOG("recvHSMSDataStream6 failed: S[%u]F[%u]", stream, function);
            }
            break;
        case 10:
            if (recvHSMSDataStream10(function, wait, systemBytes, data, dataLength) != true) {
                B1LOG("recvHSMSDataStream10 failed: S[%u]F[%u]", stream, function);
            }
            break;
        case 64:
            if (recvHSMSDataStream64(function, wait, systemBytes, data, dataLength) != true) {
                B1LOG("recvHSMSDataStream64 failed: S[%u]F[%u]", stream, function);
            }
            break;
        default:
            B1LOG("recvMessageUndefined: S[%u]F[%u]", stream, function);
            break;
    }
}

bool B1SECS2ClientSession::recvHSMSDataStream1(uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 0:
            recvMessageS1F0(systemBytes);
            break;
        case 1:
            recvMessageS1F1(wait, systemBytes, data, dataLength);
            break;
        case 2:
            recvMessageS1F2(wait, systemBytes, data, dataLength);
            break;
        case 4:
            recvMessageS1F4(wait, systemBytes, data, dataLength);
            break;
        case 14:
            recvMessageS1F14(wait, systemBytes, data, dataLength);
            break;
        case 16:
            recvMessageS1F16(wait, systemBytes, data, dataLength);
            break;
        case 18:
            recvMessageS1F18(wait, systemBytes, data, dataLength);
            break;
        default:
            B1LOG("not implemented yet: stream[1], function[%d]", function);
            assert(false);
            return false;
    };
    return true;
}

bool B1SECS2ClientSession::recvHSMSDataStream2(uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 0:
            recvMessageS2F0(systemBytes);
            break;
        case 32:
            recvMessageS2F32(wait, systemBytes, data, dataLength);
            break;
        case 34:
            recvMessageS2F34(wait, systemBytes, data, dataLength);
            break;
        case 36:
            recvMessageS2F36(wait, systemBytes, data, dataLength);
            break;
        case 38:
            recvMessageS2F38(wait, systemBytes, data, dataLength);
            break;
        case 42:
            recvMessageS2F42(wait, systemBytes, data, dataLength);
            break;
        case 50:
            recvMessageS2F50(wait, systemBytes, data, dataLength);
            break;
        default:
            B1LOG("not implemented yet: stream[2], function[%d]", function);
            assert(false);
            return false;
    }
    return true;
}

bool B1SECS2ClientSession::recvHSMSDataStream3(uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 0:
            recvMessageS3F0(systemBytes);
            break;
        case 18:
            recvMessageS3F18(wait, systemBytes, data, dataLength);
            break;
        default:
            B1LOG("not implemented yet: stream[3], function[%d]", function);
            assert(false);
            return false;
    }
    return true;
}

bool B1SECS2ClientSession::recvHSMSDataStream5(uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 1:
            recvMessageS5F1(wait, systemBytes, data, dataLength);
            break;
        case 4:
            recvMessageS5F4(wait, systemBytes, data, dataLength);
            break;
        default:
            B1LOG("not implemented yet: stream[64], function[%d]", function);
            return false;
    }
    return true;
}

bool B1SECS2ClientSession::recvHSMSDataStream6(uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 11:
            recvMessageS6F11(wait, systemBytes, data, dataLength);
            break;
        default:
            B1LOG("not implemented yet: stream[6], function[%d]", function);
            assert(false);
            return false;
    };
    return true;
}

bool B1SECS2ClientSession::recvHSMSDataStream10(uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 1:
            recvMessageS10F1(wait, systemBytes, data, dataLength);
            break;
        case 4:
            recvMessageS10F4(systemBytes, data, dataLength);
            break;
        case 6:
            recvMessageS10F6(systemBytes, data, dataLength);
            break;
        default:
            B1LOG("not implemented yet: stream[10], function[%d]", function);
            return false;
    };
    return true;
}

bool B1SECS2ClientSession::recvHSMSDataStream64(uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    switch (function) {
        case 2:
            recvMessageS64F2(wait, systemBytes, data, dataLength);
            break;
        default:
            B1LOG("not implemented yet: stream[64], function[%d]", function);
            return false;
    };
    return true;
}

void B1SECS2ClientSession::makeMessageHeader(uint8 stream, uint8 function, bool wait, uint8 stype, const std::vector<uint8>& systemBytes, std::vector<uint8>* result)
{
    uint16 sessionID = TO_UINT16_FOR_NETWORK(deviceID());   //  15bit integer
    result->insert(result->end(), (uint8*)&sessionID, (uint8*)&sessionID + sizeof(sessionID));
    result->push_back((wait ? 0x01 << 7 : 0) | stream);
    result->push_back(function);
    result->push_back(0);   //  ptype
    result->push_back(stype);
    result->insert(result->end(), systemBytes.begin(), systemBytes.end());
}

bool B1SECS2ClientSession::implInitializeSession()
{
    if (B1ArrayBufferClientSession::implInitializeSession() != true) {
        return false;
    }
    _clientSecs2DataManager = createClientSECS2DataManager();
    if (_clientSecs2DataManager->initialize() != true) {
        return false;
    }
    return true;
}

void B1SECS2ClientSession::implFinalizeSession()
{
    if (_clientSecs2DataManager) {
        _clientSecs2DataManager->finalize();
        _clientSecs2DataManager.reset();
    }
    B1ArrayBufferClientSession::implFinalizeSession();
}

void B1SECS2ClientSession::implOnConnect()
{
    B1ArrayBufferClientSession::implOnConnect();
    srand((unsigned int)time(0));
    _initialSystemByte = rand();
    _lastMessage.reset();
    _t7Checker.start(B1HSMSMessage::TIME_OUT_DEFAULT_T7);
    _aliveChecker.stop();
    _lastAliveRequestTick = B1TickUtil::currentTick();
}

void B1SECS2ClientSession::onReadComplete(uint8* data, size_t dataSize)
{
    const size_t messageLengthSize = 4;
    auto& buffer = _lastMessage._data;
    buffer.insert(buffer.end(), data, data + dataSize);
    size_t pos = 0;
    while (buffer.size() - pos >= messageLengthSize) {
        uint32 length = 0;
        memcpy(&length, &buffer[pos], messageLengthSize);
        length = TO_UINT32_FOR_NETWORK(length);
        if (buffer.size() - pos - messageLengthSize < length) {
            break;  //  not enough data.
        }
        std::vector<uint8> message(buffer.begin() + pos + messageLengthSize, buffer.begin() + pos + messageLengthSize + length);
        pos += messageLengthSize + length;
        recvHSMSMessage(message);
    }
    buffer.erase(buffer.begin(), buffer.begin() + pos);
}

void B1SECS2ClientSession::implProcessConnected(bool firstConnectedProcess)
{
    if (_t7Checker.isRunning() && _t7Checker.isTimeover(false)) {
        B1LOG("SELECT timedout: deviceID[%d]", deviceID());
        disconnect();
        return;
    }
    if (_aliveChecker.isRunning() && _aliveChecker.isTimeover(false)) {
        B1LOG("NETWORK timedout: deviceID[%d]", deviceID());
        disconnect();
        return;
    }
    if (firstConnectedProcess) {
        sendControlMessage(makeNewSystemBytes(), CONTROL_MESSAGE_SELECT_REQ);
    }
    else {
        uint64 now = B1TickUtil::currentTick();
        if (B1TickUtil::diffTick(_lastAliveRequestTick, now) > CONSTS_REQ_ALIVE_INTERVAL) {
            _lastAliveRequestTick = now;
            sendControlMessage(makeNewSystemBytes(), CONTROL_MESSAGE_LINK_TEST_REQ);
        }
    }
    B1ArrayBufferClientSession::implProcessConnected(firstConnectedProcess);
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

void B1SECS2ClientSession::sendControlMessage(const std::vector<uint8>& systemBytes, CONTROL_MESSAGE controlMessage, bool wait)
{
    B1LOG("sendControlMessage: controlMessage[%d]", controlMessage);
    uint32 messageLenth = 10;
    std::vector<uint8> message;
    message.reserve(messageLenth + sizeof(messageLenth));
    messageLenth = TO_UINT32_FOR_NETWORK(messageLenth);
    message.insert(message.end(), (uint8*)&messageLenth, (uint8*)&messageLenth + sizeof(messageLenth));
    makeMessageHeader(0, 0, wait, controlMessage, systemBytes, &message);
    writeData(message);
}

void B1SECS2ClientSession::sendDataMessage(const std::vector<uint8>& systemBytes, uint8 stream, uint8 function, bool wait)
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
    makeMessageHeader(stream, function, wait, 0, systemBytes, &message);
    writeData(message);
}

void B1SECS2ClientSession::sendDataMessage(const std::vector<uint8>& systemBytes, uint8 stream, uint8 function, const std::vector<std::vector<uint8> >& items, bool wait)
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
    makeMessageHeader(stream, function, wait, 0, systemBytes, &message);
    for (const auto& item : items) {
        message.insert(message.end(), item.begin(), item.end());
    }
    writeData(message);
}

void B1SECS2ClientSession::sendDataMessage(const std::vector<uint8>& systemBytes, const B1SECS2MessageWritable& message)
{
    if (message.items().empty()) {
        sendDataMessage(systemBytes, message.stream(), message.function(), message.wait());
    }
    else {
        sendDataMessage(systemBytes, message.stream(), message.function(), message.items(), message.wait());
    }
}

void B1SECS2ClientSession::sendMessageF0(uint8 stream, const std::vector<uint8>& systemBytes)
{
    sendDataMessage(systemBytes, stream, 0, false);
}
