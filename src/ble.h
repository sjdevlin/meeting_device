#ifndef ble_h
#define ble_h

#include <signal.h>
#include <iostream>
#include <thread>
#include <sstream>
#include <mutex>
#include <fstream>

#include "audio.h"
#include "ggk.h"

class BLE
{

public:
    BLE();
    bool start();
    void stop();
    void update(AUDIO);
    static const void * data_getter(const char *);
    static int data_setter(const char *, const void *);

    //mutex so that we can multi-thread
    static std::mutex mutex_buffer;
    // The battery level ("battery/level") reported by the server (see Server.cpp)
    static uint8_t server_data_battery_level;
    // The text string ("text/string") used by our custom text string service (see Server.cpp)
    static std::string server_data_text_String;
    static const int max_async_init_timeout_ms = 5000;

private:
    // Maximum time to wait for any single async process to timeout during initialization
};

#endif
