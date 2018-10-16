/*
 * C/C++ Includes
 */

#include <stdint.h>
#include <stdarg.h>

/* Arduino Includes */

#include "Arduino.h"

/*
 * ADL Includes
 */

#include "device.h"
#include "parameter.h"
#include "adl.h"
#include "adl-logging.h"

/*
 * Private Data
 */

static const uint8_t LOG_BUFFER_SIZE = 64;


static const char s_application_name[] PROGMEM = "APP";


static char const * const s_adl_log_module_prefixes[] PROGMEM = {

    s_application_name,


};

static Print * s_p_log_stream = NULL;
static char s_buf[LOG_BUFFER_SIZE];
static char s_prefix_buf[7];

/*
 * Private Functions
 */


static void p(char const * const fmt, va_list args)
{   
    vsnprintf(s_buf, LOG_BUFFER_SIZE, fmt, args);
    s_p_log_stream->print(s_buf);
}

static void pln(char const * const fmt, va_list args)
{
    vsnprintf(s_buf, LOG_BUFFER_SIZE, fmt, args);
    s_p_log_stream->println(s_buf);
}

/*
 * Public Functions
 */

void adl_logging_setup(Print& log_printer)
{
    s_p_log_stream = &log_printer;
}

void log(ADL_LOG_MODULES module, char const * const fmt, ...)
{
    va_list args;
    adl_board_read_progmem(s_prefix_buf, s_adl_log_module_prefixes[module]);
    s_p_log_stream->print(s_prefix_buf);
    s_p_log_stream->print(": ");
    va_start(args, fmt);
    p(fmt, args);
    va_end(args);
    
}

void logln(ADL_LOG_MODULES module, char const * const fmt, ...)
{
    va_list args;
    adl_board_read_progmem(s_prefix_buf, s_adl_log_module_prefixes[module]);
    s_p_log_stream->print(s_prefix_buf);
    s_p_log_stream->print(": ");
    va_start(args, fmt);
    pln(fmt, args);
    va_end(args);
}
