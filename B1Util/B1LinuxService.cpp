//
// B1LinuxService.cpp
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Util.h"
#include "B1LinuxService.h"

#if defined(__linux__)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <systemd/sd-bus.h>

using namespace BnD;

struct sd_bus_data {
    sd_bus_data() : _bus(NULL), _error(SD_BUS_ERROR_NULL), _reply(NULL) {}
    ~sd_bus_data()
    {
        sd_bus_error_free(&_error);
        sd_bus_message_unref(_reply);
        sd_bus_unref(_bus);
    }
    sd_bus* _bus;
    sd_bus_error _error;
    sd_bus_message* _reply;
};

bool B1LinuxService::startService(const B1String& serviceName)
{
    sd_bus_data bus_data;

    int r = sd_bus_open_system(&bus_data._bus);
    if (r < 0) {
        b1log("unable to start_service. failed to connect to system bus: %s", strerror(-r));
        return false;
    }

    r = sd_bus_call_method(bus_data._bus,
        "org.freedesktop.systemd1",          // service
        "/org/freedesktop/systemd1",         // path
        "org.freedesktop.systemd1.Manager",  // interface
        "StartUnit",                         // method
        &bus_data._error,                    // error object
        &bus_data._reply,                    // reply message
        "ss",                                // input signature (string-string)
        serviceName.cString(),               // unit name
        "replace");                          // mode (e.g., "replace", "fail", "isolate")
    if (r < 0) {
        b1log("unable to start_service. failed to call StartUnit: %s", bus_data._error.message);
        return false;
    }
    return true;
}

bool B1LinuxService::stopService(const B1String& serviceName)
{
    sd_bus_data bus_data;

    int r = sd_bus_open_system(&bus_data._bus);
    if (r < 0) {
        b1log("unable to stop_service. failed to connect to system bus: %s", strerror(-r));
        return false;
    }

    r = sd_bus_call_method(bus_data._bus,
        "org.freedesktop.systemd1",
        "/org/freedesktop/systemd1",
        "org.freedesktop.systemd1.Manager",
        "StopUnit",
        &bus_data._error,
        &bus_data._reply,
        "ss",
        serviceName.cString(),
        "replace");
    if (r < 0) {
        b1log("unable to stop_service. failed to call StopUnit: %s", bus_data._error.message);
        return false;
    }
    return true;
}

bool B1LinuxService::getServiceActive(const B1String& serviceName, bool* isActive)
{
    sd_bus_data bus_data;

    int r = sd_bus_open_system(&bus_data._bus);
    if (r < 0) {
        b1log("unable to get_service_active. failed to connect to system bus: %s", strerror(-r));
        return false;
    }

    std::vector<B1String> split;
    serviceName.toknize(".", std::back_inserter(split));
    B1String path;
    for (const auto& s : split) {
        if (path.isEmpty() != true) {
            path.append("_2e");
        }
        path += s;
    }
    char* msg = 0;
    r = sd_bus_get_property_string(
        bus_data._bus,
        "org.freedesktop.systemd1",
        ("/org/freedesktop/systemd1/unit/" + path).cString(),
        "org.freedesktop.systemd1.Unit",
        "ActiveState",
        &bus_data._error,
        &msg);
    B1String result(msg);
    free(msg);
    if (r < 0) {
        b1log("unable to get_service_state. failed to connect to system bus: %s", strerror(-r));
        return false;
    }
    if (result == "active") {
        *isActive = true;
    }
    else if (result == "inactive") {
        *isActive = false;
    }
    else {
        return false;
    }
    return true;
}

#endif  //  _WIN32
