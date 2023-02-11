#include "ble.h"


//  Main Class Methods for the Meet Pie Device

// These static variables have to initilized outside of the constructor
// battery level ("battery/level") reported by the server (see Server.cpp)
uint8_t BLE::server_data_battery_level = 100;
std::mutex BLE::mutex_buffer;
std::string BLE::server_data_text_String = "";


BLE::BLE()
{
}

const void * BLE::data_getter(const char *p_name)
{

	if (nullptr == p_name)
	{
		printf("NULL name sent to server data getter");
		return nullptr;
	}

	std::string str_name = p_name;

	if (str_name == "battery/level")
	{
		return &server_data_battery_level;
	}
	else if (str_name == "text/string")
	{
		return server_data_text_String.c_str();
	}

	printf((std::string("Unknown name for server data getter request: '") + p_name + "'").c_str());
	return nullptr;
}

int BLE::data_setter(const char *p_name, const void *p_data)
{

    if (nullptr == p_name)
    {
        printf("NULL name sent to server data setter");
        return 0;
    }
    if (nullptr == p_data)
    {
        printf("NULL p_data sent to server data setter");
        return 0;
    }

    std::string str_name = p_name;

    if (str_name == "mode")
    {
        std::string ble_mode = static_cast<const char *>(p_data);
        printf((std::string("Server data: mode set to '") + ble_mode + "'").c_str());
        return 1;
    }

    printf((std::string("Unknown name for server data setter request: '") + p_name + "'").c_str());

    return 0;
}

bool BLE::start()
{
        // Start Bluetooth
    if (!ggkStart("gobbledegook", "Gobbledegook", "Gobbledegook", BLE::data_getter, BLE::data_setter, BLE::max_async_init_timeout_ms))
	{
        return 0;
	}
	else
	{
	return 1;
	}

}


void BLE::stop()
{
	printf("got to ggk shut down and wait");
    ggkShutdownAndWait();

}

void BLE::update(AUDIO odas_obj) 
{
            // now create the JSON for BLE

            	mutex_buffer.lock();

			server_data_text_String = "";

			for (int i = 0; i < NUMCHANNELS; i++)
			{
				if (odas_obj.track_id[i] != 0)
				{
				int angle_in_deg = 180 - (atan2(odas_obj.y_array[i], odas_obj.x_array[i]) * 57.3);
				if (server_data_text_String != "") server_data_text_String += ",";
				server_data_text_String += std::to_string(angle_in_deg);
				}
			}

			mutex_buffer.unlock();

		    if (debug_mode == 0x01) printf ("Bluetooth data: %s\n",server_data_text_String.c_str());

    		// now the output string is ready and_swe should call notify
			ggkNofifyUpdatedCharacteristic("/com/gobbledegook/text/string");

}
