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

#define STARTSTOPBUTTON 18
#define RUNLED 17
#define POWERLED 22

#define POLLINGFREQ 1000000  // also this into a parameter file
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

    // Create objects

    if (debug_mode == 0x01) printf("creating odas object obj\n");
    AUDIO audio_obj;
    if (debug_mode == 0x01) printf("creating ble object obj\n");
    BLE ble_obj;
    if (debug_mode == 0x01) printf("creating device object\n");
    DEVICE device_obj(STARTSTOPBUTTON, POWERLED, RUNLED);
    device_obj.darken_led(POWERLED);
    device_obj.darken_led(RUNLED);
    fflush(stdout);

    // JJ remember to add error codes that bubble up

    // Setup our signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    char status = STOPPED; // make sure main loop starts in right mode
//    char status = START; // make sure main loop starts in right mode

    // start up the bluetooth server
       
	int attempt = 0;
	int ble_status = 99;

	while (ble_status != 0 && attempt < 4)
	   	{
		ble_status = ble_obj.start();
		printf("starting bluetooth. returned (%d) attempt number : %d\n", ble_status, attempt);
	    	attempt++;
		fflush(stdout);
            	device_obj.light_led(RUNLED);
            	device_obj.darken_led(POWERLED);
	    	sleep(1);
            	device_obj.darken_led(RUNLED);
            	device_obj.light_led(POWERLED);
	    	sleep(1);
	    	}
	    
        if (ble_status != 0){
		if (debug_mode == 0x01) printf("BLE fucked up again");
		if (debug_mode == 0x01) fflush(stdout);
	 	exit(0);    
		}

    printf ("shutdown request %d", shutdown_request);
    fflush(stdout);

    while (shutdown_request != 0x01)
    {
        switch (status)
        {

        case STOPPED:
            device_obj.light_led(POWERLED);
            device_obj.darken_led(RUNLED);
      	    usleep (BUTTONFREQ);

            if (device_obj.button_pressed(STARTSTOPBUTTON)) 
            {
	    if (debug_mode == 0x01) printf("button pressed \n");
   	    if (debug_mode == 0x01) fflush(stdout);
                status = START;
                device_obj.darken_led(POWERLED);
            }
            break;


        case START:

            audio_obj.start(file_config);
            device_obj.light_led(RUNLED);
            status = RUNNING;
            break;

        case RUNNING:

            audio_obj.get_data();                       // Get Audio source data
            ble_obj.update(audio_obj);

            usleep(POLLINGFREQ); // This is the crucial delay that determines frequnecy of polling

            if (device_obj.button_pressed(STARTSTOPBUTTON)) {
			device_obj.darken_led(RUNLED);
                            status = STOP;

            }

            break;

        case STOP:

            audio_obj.stop();
            status = STOPPED;
            usleep (BUTTONFREQ); // just avoiding bounce

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
