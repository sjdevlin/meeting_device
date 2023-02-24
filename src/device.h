#ifndef device_h
#define device_h

// system calls
#include <unistd.h>
#include <cstdio>

extern "C" {
#include <pigpio.h>
}
// IO streams and functions

// Pi GPIO interface
//extern "C" {
//#include "mini_gpio.h"
//}

extern char debug_mode;

class DEVICE
{

public:
    DEVICE(unsigned, unsigned, unsigned);
    bool button_pressed(unsigned);
    void light_led(unsigned);
    void darken_led(unsigned);
//    void blink_led(unsigned);

};

#endif
