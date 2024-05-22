#ifndef _CLIPPER_TEXT_SCREEN_H_
#define _CLIPPER_TEXT_SCREEN_H_

#include <stdint.h>

enum text_screen_attribute {
	TEXT_SCREEN_FG_BLACK = 0,
	TEXT_SCREEN_FG_BLUE,
	TEXT_SCREEN_FG_GREEN,
	TEXT_SCREEN_FG_CYAN,
	TEXT_SCREEN_FG_RED,
	TEXT_SCREEN_FG_PURPLE,
	TEXT_SCREEN_FG_BROWN,
	TEXT_SCREEN_FG_GRAY,
	TEXT_SCREEN_FG_DARK_GRAY,
	TEXT_SCREEN_FG_LIGHT_BLUE,
	TEXT_SCREEN_FG_LIGHT_GREEN,
	TEXT_SCREEN_FG_LIGHT_CYAN,
	TEXT_SCREEN_FG_LIGHT_RED,
	TEXT_SCREEN_FG_LIGHT_PURPLE,
	TEXT_SCREEN_FG_LIGHT_YELLOW,
	TEXT_SCREEN_FG_WHITE,

	TEXT_SCREEN_BG_BLACK 	=  0,
	TEXT_SCREEN_BG_BLUE  	= (1 << 4),
	TEXT_SCREEN_BG_GREEN 	= (2 << 4),
	TEXT_SCREEN_BG_CYAN  	= (3 << 4),
	TEXT_SCREEN_BG_RED   	= (4 << 4),
	TEXT_SCREEN_BG_PURPLE 	= (5 << 4),
	TEXT_SCREEN_BG_BROWN	= (6 << 4),
	TEXT_SCREEN_BG_GRAY		= (7 << 4)
};

void tsinit(void);

void tsclear_screen();
void tscursor_set(uint8_t x, uint8_t y);

void tsputch(char ch, uint8_t color);
void tsputs(const char* str, uint8_t color);

void tsprintf(const char* format, ...)
	__attribute__((format (printf, 1, 2)));

#endif // _CLIPPER_TEXT_SCREEN_H_
