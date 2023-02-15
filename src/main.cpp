#include <signal.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>

#include "audio.h"
#include "ble.h"
#include "ggk.h"
#include "device.h"

// These macro definitions are for main loop logic
#define STOPPED 0x00
#define STOP 0x01
#define START 0x02
#define RUNNING 0x03

#define STARTSTOPBUTTON 17
#define RUNLED 18
#define POWERLED 19


#define POLLINGFREQ 500000  // also this into a parameter file
#define BUTTONFREQ 250000   // also this into a parameter file
#define TWO_SECONDS 2000000 // also this into a parameter file

char debug_mode = 0x00;       // global variable for debugging flag
char shutdown_request = 0x00; // Used in signal handler for orderly shut down on Ctrl C

void signal_handler(int signum)
{
    switch (signum)
    {
    case SIGINT:
        printf("SIGINT recieved, shutting down");
        shutdown_request = 0x01; // JJ check this is OK
        break;
    case SIGTERM:
        printf("SIGTERM recieved, shutting down");
        shutdown_request = 0x01;
        break;
    }
}

int main(int argc, char *argv[])
{
    // Create objects
    AUDIO audio_obj;
    BLE ble_obj;
    DEVICE device_obj(STARTSTOPBUTTON);

    // Command-line parser  // need to add error condition
    int c;
    char *file_config;

    while ((c = getopt(argc, argv, "adc:")) != -1)
    {
        switch (c)
        {
        case 'c':
            file_config = (char *)malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(file_config, optarg);
            break;
        case 'd':
            debug_mode = 0x01;
            break;
        }
    }

    // JJ remember to add error codes that bubble up

    // Setup our signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    char status = STOPPED; // make sure main loop starts in right mode

    // start up the bluetooth server

        if (!ble_obj.start())  // fatal error if bluetooth is unable to start
        {
            printf("error starting bluetooth");
            exit(0);
        }

    while (shutdown_request != 0x01)
    {
        switch (status)
        {

        case STOPPED:
            device_obj.darken_led(POWERLED);
	    usleep (50000);
            device_obj.light_led(POWERLED);
      	    usleep (50000);
            if (device_obj.button_pressed()) 
            {
                status = START;
                device_obj.darken_led(POWERLED);
            }
            break;


        case START:

	    device_obj.light_led(RUNLED);
	    usleep (500000);
            device_obj.darken_led(RUNLED);

            audio_obj.start(file_config);
            device_obj.light_led(RUNLED);
            status = RUNNING;
            break;

        case RUNNING:

            audio_obj.get_data();                       // Get Audio source data
            ble_obj.update(audio_obj);

            usleep(POLLINGFREQ); // This is the crucial delay that determines frequnecy of polling

            if (device_obj.button_pressed()) {
			device_obj.darken_led(RUNLED);
            		device_obj.light_led(POWERLED);
	    		usleep (50000);
            		device_obj.darken_led(POWERLED);
                            status = STOP;

            }

            break;

        case STOP:

            audio_obj.stop();
            device_obj.light_led(POWERLED);
            status = STOPPED;

            break;

        default:
            break;
        }
    }

    if (status == RUNNING) // this means it was a shutdown request from ctrl C
    {
        audio_obj.stop();
    }

    free((void *)file_config);

  //  ble_obj.stop();  // Stop the ggk server

    exit(0);
}
