#include <Arduino.h>
#include <EEPROM.h>

#include "adl-logging.h"
#include "adl-defs.h"
#include "adl-nv.h"
#include "device.h"
#include "parameter.h"
#include "adl.h"
#include "adl-callbacks.h"



int adl_board_read_progmem(char * const dst, char const * const src)
{
    return(strlen(strcpy_P(dst, src)));
}