#ifndef meetpie_h
#define meetpie_h

// system calls
#include <unistd.h>
// IO streams and functions

// Pi GPIO interface
#include <pigpio.h>


class DEVICE
{

public:
    DEVICE(unsigned);
    bool button_pressed();
    void light_led(unsigned);
    void darken_led(unisgned);
    void blink_led(unsigned);

private:
    unsigned start_stop_button;
};

#endif
