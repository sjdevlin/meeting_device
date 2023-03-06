#include "audio.h"

AUDIO::AUDIO()
{
    for (int i = 0; i < 4;i++)
    {
        track_id[i] = 0;
        x_array[i] = 0.0;
        y_array[i] = 0.0;
        freq_array[i] = 0.0;
    
    }
}

void AUDIO::get_data()
{
//	printf("debug %d signals %d\n", debug_mode,aobjs->asnk_categories_object->snk_categories->nChannels); 	

    for (int i = 0; i < aobjs->asnk_categories_object->snk_categories->nChannels;i++)
    {
        if (track_id[i] != aobjs->asnk_categories_object->snk_categories->safe_buffer->track_id[i])
        {
        track_id[i] = aobjs->asnk_categories_object->snk_categories->safe_buffer->track_id[i];
        x_array[i] = aobjs->asnk_categories_object->snk_categories->safe_buffer->X_array[i];
        y_array[i] = aobjs->asnk_categories_object->snk_categories->safe_buffer->Y_array[i];
        }

//    if (debug_mode == 0x01)
  //      printf("Odas data [%d]: X[%1.2f],Y[%1.2f],f[%3.0f]\n", i, x_array[i], y_array[i], freq_array[i]);
    }
}

void AUDIO::stop()
{
    if (debug_mode == 0x01)
        printf("| + Stopping Threads.................  \n ");

    threads_multiple_stop(aobjs);
    threads_multiple_join(aobjs);

    if (debug_mode == 0x01)
        printf("[Done] |\n");

    if (debug_mode == 0x01)
        printf("| + Free memory......................  \n");

    aobjects_destroy(aobjs);
    configs_destroy(cfgs);
    
    if (debug_mode == 0x01)
        printf("[Done] |\n");
}

void AUDIO::start(char * file_config)
{

    if (file_config == NULL)
    {
        printf("Missing configuration file.\n");
        exit(EXIT_FAILURE);  // change to bubble this up so we can shut down ble etc.
    }

    if (debug_mode == 0x01)
        printf("| + Initializing configurations...... \n");

    cfgs = configs_construct(file_config);

    if (debug_mode == 0x01)
        printf("[Done] |\n");

    if (debug_mode == 0x01)
        printf("| + Initializing objects............. \n");

    aobjs = aobjects_construct(cfgs);

    if (debug_mode == 0x01)
        printf("[Done] |\n");

    if (debug_mode == 0x01)
        printf("| + Launch threads................... \n");

    threads_multiple_start(aobjs);
 
    if (debug_mode == 0x01)
        printf("[Done] |\n");
}
