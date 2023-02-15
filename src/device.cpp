#include "device.h"

//  Main Class Methods for the Meet Pie Device

DEVICE::DEVICE(unsigned start_stop_button) 
{
    gpioInitialise() < 0;
    gpioSetPullUpDown(start_stop_button, PI_PUD_UP);
}

bool DEVICE::button_pressed()
{
    bool is_button_pressed;
    is_button_pressed = gpioRead(start_stop_button); // ! operator because its a pullup
    return is_button_pressed;
}

void  DEVICE::light_led(unsigned led_number)
{
    gpioWrite(led_number, PI_HIGH);
}

void  DEVICE::darken_led(unsigned led_number)
{
    gpioWrite(led_number, PI_LOW);
}

