#include "device.h"

//  Main Class Methods for the Meet Pie Device

DEVICE::DEVICE(unsigned start_stop_button, unsigned power_led, unsigned run_led) 
{
    int status = gpioInitialise();
    if (status < 0 && debug_mode == 0x01) printf ("Error initialising %d\n", status);

    status = gpioSetPullUpDown(start_stop_button, PI_PUD_UP);
    status = gpioSetMode(start_stop_button, PI_INPUT);
    if (debug_mode == 0x01) printf ("Button %d", status);
    status = gpioSetPullUpDown(power_led, PI_PUD_DOWN);
    status = gpioSetMode(power_led, PI_OUTPUT);
    if (debug_mode == 0x01) printf ("Power LED %d\n", status);
    status = gpioSetPullUpDown(run_led, PI_PUD_DOWN);
    status = gpioSetMode(run_led, PI_OUTPUT);
    if (debug_mode == 0x01) printf ("Run LED %d\n", status);

}

bool DEVICE::button_pressed(unsigned button)
{
    int button_status;
    button_status = gpioRead(button); // ! operator because its a pullup
    if (button_status == 1) 
	{
	return false;
	}
	else
	{
	return true;
	}
}

void  DEVICE::light_led(unsigned led_number)
{
    gpioWrite(led_number, PI_HIGH);
}

void  DEVICE::darken_led(unsigned led_number)
{
    gpioWrite(led_number, PI_LOW);
}

