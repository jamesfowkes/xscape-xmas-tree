#ifndef _SETTINGS_H_
#define _SETTINGS_H_

static const uint8_t NUMBER_OF_DIGITS = 4;

static const uint8_t CODE[NUMBER_OF_DIGITS] = {
	3, 5, 2, 8
};

static const uint16_t FLASH_TIME_MS = 500;
static const uint16_t FLASH_DELAY_MS = 500;
static const uint16_t INTERDIGIT_DELAY_MS = 1000;

#define FLASH_COLOUR_1 32,0,0
#define FLASH_COLOUR_2 0,32,0
#define FLASH_COLOUR_3 0,0,32
#define FLASH_COLOUR_4 32,32,32

#endif
