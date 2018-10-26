















/*
app
Created on 2018-10-26
By the Arduino Description Language tool.
 

		Application for the Xscape xmas tree puzzle
	
*/





#include <stdint.h>

#include "adl-logging.h"
#include "adl-defs.h"
#include "adl-nv.h"
#include "device.h"
#include "parameter.h"
#include "adl.h"
#include "adl-callbacks.h"


#include <Adafruit_NeoPixel.h>





#include "adafruit-neopixel-adl.h"

#include "debounced-input.h"





static DebouncedInput s_button = DebouncedInput(4, 50, true);

static AdafruitNeoPixelADL s_neopixels = AdafruitNeoPixelADL(3, 100, NEO_RGB + NEO_KHZ800);


static DeviceBase * s_devices[] = 
{
    
    &s_button
    
    ,
    
    
    &s_neopixels
    
    
};



static ParameterBase * s_params[] = 
{
    
};






int handle_device1_command(char const * const command, char * reply)
{
    return s_button.command_handler(command, reply);
}

int handle_device2_command(char const * const command, char * reply)
{
    return s_neopixels.command_handler(command, reply);
}


static COMMAND_HANDLER adl_devices[] = {
    
    handle_device1_command,
    
    handle_device2_command,
    
};

COMMAND_HANDLER& adl_get_device_cmd_handler(DEVICE_ADDRESS address)
{
    return adl_devices[address-1];
}

DeviceBase& adl_get_device(DEVICE_ADDRESS address)
{
    return *s_devices[address-1];
}



static COMMAND_HANDLER adl_params[] = {
    
};

COMMAND_HANDLER& adl_get_param_cmd_handler(PARAM_ADDRESS address)
{
    return adl_params[address-1];
}

ParameterBase& adl_get_param(PARAM_ADDRESS address)
{
    return *s_params[address-1];
}



void adl_board_send(char * to_send)
{
    Serial.println(to_send);
}


void setup()
{
    adl_on_setup_start();

    Serial.begin(115200);
    
    adl_nonvolatile_setup();

    adl_logging_setup(Serial);

    
    // Setup for Button
    s_button.setup();
    // END Button setup

    
    // Setup for NeoPixels
    s_neopixels.setup();
    // END NeoPixels setup

    

    adl_custom_setup(s_devices, ADL_DEVICE_COUNT, s_params, ADL_PARAM_COUNT);

    adl_on_setup_complete();
    
    if (0)
    {
        adl_delay_start( 0 );
    }
}



void loop()
{
    adl_handle_any_pending_commands();
    adl_service_timer();
    adl_custom_loop(s_devices, ADL_DEVICE_COUNT, s_params, ADL_PARAM_COUNT);
}



        void serialEvent()
        {
            while (Serial.available())
            {
                adl_add_char((char)Serial.read());
            }
        }



