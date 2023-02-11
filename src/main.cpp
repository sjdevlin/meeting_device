#include <signal.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>

#include "audio.h"
#include "ble.h"
#include "ggk.h"

// These macro definitions are for main loop logic
#define STOPPED 0x00
#define STOP 0x01
#define START 0x02
#define RUNNING 0x03
#define SETMODE 0x04

#define SHORTPRESS 1
#define LONGPRESS 6

#define NUMLEDS 35          // eventually this will go into a parameter file
#define POLLINGFREQ 500000  // also this into a parameter file
#define BUTTONFREQ 250000   // also this into a parameter file
#define TWO_SECONDS 2000000 // also this into a parameter file

char debug_mode = 0x00;       // global variable for debugging flag
char auto_mode = 0x01;        // global variable for auto run without button - used in development only
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
        case 'a':
            auto_mode = 0x01;
            break;
        }
    }

    // JJ remember to add error codes that bubble up

    // Setup our signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    char status = STOPPED; // make sure main loop starts in right mode
    int time_button_pressed = 0;
    unsigned int blink_counter = 0;

    // start up the bluetooth server

        if (!ble_obj.start())  // fatal error if bluetooth is unable to start
        {
            printf("error starting bluetooth");
            exit(0);
        }

    // only for development
    if (auto_mode == 0x01)
        status = START;

    while (shutdown_request != 0x01)
    {
        switch (status)
        {

        case START:

            audio_obj.start(file_config);

            status = RUNNING;
            break;

        case RUNNING:

            audio_obj.get_data();                       // Get Audio source data
            ble_obj.update(audio_obj);

            usleep(POLLINGFREQ); // This is the crucial delay that determines frequnecy of polling

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
