#ifndef audio_h
#define audio_h

extern "C" {
#include "../../include/odas/odas.h"

#include "parameters.h"
#include "configs.h"
#include "objects.h"
#include "threads.h"
}

#include <time.h>
#include <signal.h>

// for cpp
#include <string>

#define NUMCHANNELS 3

extern char debug_mode;

class AUDIO
{

public:
    AUDIO();
    void start(char *);
    void stop();
    void get_data();

    unsigned long long track_id[4];
    float x_array[4];
    float y_array[4];
    float freq_array[4];
    objects *objs;
    aobjects *aobjs;
    configs *cfgs;
};

#endif
