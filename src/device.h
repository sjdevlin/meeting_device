#ifndef device_h
#define device_h

// system calls
#include <unistd.h>
// IO streams and functions

// Pi GPIO interface
extern "C" {
#include "mini_gpio.h"
}

class DEVICE
{

public:
    DEVICE(unsigned);
    bool button_pressed();
    void light_led(unsigned);
    void darken_led(unsigned);
//    void blink_led(unsigned);

private:
    unsigned start_stop_button;
};

#endif
