#ifndef _ADL_LOGGING_H_
#define _ADL_LOGGING_H_

typedef enum adl_log_modules
{

	LOG_APP,

} ADL_LOG_MODULES;

void adl_logging_setup(Print& log_printer);
void log(ADL_LOG_MODULES module, char const * const fmt, ...);
void logln(ADL_LOG_MODULES module, char const * const fmt, ...);

#endif