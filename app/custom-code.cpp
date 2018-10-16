#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "device.h"
#include "parameter.h"
#include "adl.h"

#include "adl-logging.h"

#include "debounced-input.h"
#include "adafruit-neopixel-adl.h"
#include "adl-oneshot-timer.h"

#include "settings.h"

static const uint8_t NUMBER_OF_PIXELS = 6;
static const uint8_t CODE_COLOURS[NUMBER_OF_DIGITS][3] = {
	{FLASH_COLOUR_1},
	{FLASH_COLOUR_2},
	{FLASH_COLOUR_3},
	{FLASH_COLOUR_4}
};

typedef enum _eState
{	
	eState_Idle,
	eState_Flashing,
	eState_FlashDelay,
	eState_InterDigitDelay,
} eState;

static eState s_eState = eState_Idle;

static DebouncedInput * pInput;
static AdafruitNeoPixelADL * pNeopixels;

static int8_t s_current_digit = -1;
static uint8_t s_flash_number = 1;

static ADLOneShotTimer s_timer(FLASH_TIME_MS);

static void start_flash(uint8_t digit)
{
	if (digit < NUMBER_OF_DIGITS)
	{
		pNeopixels->pixels().clear();
		pNeopixels->set_pixels(0, NUMBER_OF_PIXELS, CODE_COLOURS[digit]);
	}
}

static void stop_flash(uint8_t digit)
{
	if (digit < NUMBER_OF_DIGITS)
	{
		pNeopixels->pixels().clear();
		pNeopixels->pixels().show();
	}
}

static void set_digit(uint8_t digit)
{
	s_current_digit = digit;
	s_flash_number = 1;
}

static void start_flash_with_delay(uint8_t digit, ADLOneShotTimer& timer, uint16_t delay_ms)
{
	start_flash(digit);
	timer.start(delay_ms);
}

static bool flashes_complete(uint8_t current_flash_number, uint8_t total_flashes)
{
	return current_flash_number >= total_flashes;
}

static bool code_complete(uint8_t current_digit, uint8_t total_digits)
{
	return current_digit >= total_digits;
}

static void play_intro()
{
	uint8_t i;
	for (i=0; i<NUMBER_OF_DIGITS; i++)
	{
		start_flash_with_delay(i, s_timer, 250);
		while (!s_timer.check_and_reset()) {}
		stop_flash(i);
		delay(250);
	}
}

void adl_custom_setup(DeviceBase * pdevices[], int ndevices, ParameterBase * pparams[], int nparams)
{
	(void)ndevices; (void)pparams; (void)nparams;

	pInput = (DebouncedInput*)pdevices[0];
	pNeopixels = (AdafruitNeoPixelADL*)pdevices[1];

	play_intro();
}

void adl_custom_loop(DeviceBase * pdevices[], int ndevices, ParameterBase * pparams[], int nparams)
{
	(void)pdevices; (void)pparams; (void)ndevices; (void)nparams;

	switch(s_eState)
	{
	case eState_Idle:
		if (pInput->check_low_and_clear())
		{
			logln(LOG_APP, "Starting flash...");
			set_digit(0);
			logln(LOG_APP, "Start digit %d, flash %d", s_current_digit, s_flash_number);
			start_flash_with_delay(s_current_digit, s_timer, FLASH_TIME_MS);
			s_eState = eState_Flashing;
		}
		break;
	case eState_Flashing:
		if (s_timer.check_and_reset())
		{
			stop_flash(s_current_digit);

			if (!flashes_complete(s_flash_number, CODE[s_current_digit]))
			{
				s_flash_number++;
				s_timer.start(FLASH_DELAY_MS);
				s_eState = eState_FlashDelay;
			}
			else
			{
				set_digit(s_current_digit+1);
				if (!code_complete(s_current_digit, NUMBER_OF_DIGITS))
				{
					logln(LOG_APP, "Finished digit %d", s_current_digit);
					s_timer.start(INTERDIGIT_DELAY_MS);
					s_eState = eState_InterDigitDelay;
				}
				else
				{
					logln(LOG_APP, "Finished all flashes", s_current_digit);
					s_eState = eState_Idle;
				}
			}
		}
		break;
	case eState_FlashDelay:
	case eState_InterDigitDelay:
		if (s_timer.check_and_reset())
		{
			logln(LOG_APP, "Start digit %d, flash %d", s_current_digit, s_flash_number);
			start_flash_with_delay(s_current_digit, s_timer, FLASH_TIME_MS);
			s_eState = eState_Flashing;
		}
		break;
	}
}