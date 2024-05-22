#include "internal/text_screen.h"

#include <stdarg.h>
#include "internal/io.h"
#include "stdlib.h"
#include "string.h"

struct text_screen_char {
	char character;
	uint8_t attribute;
} __attribute__((packed));

static volatile struct text_screen_char* videomem = (struct text_screen_char*)0xB8000;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

enum printf_format_specifier_type {
	FORMAT_SPECIFIER_INT,
	FORMAT_SPECIFIER_UNSIGNED,
	FORMAT_SPECIFIER_HEXADECIMAL,
	FORMAT_SPEFICIER_UNKNOWN
};

struct printf_format_specifier {
	enum printf_format_specifier_type type;

	int field_size;
	char padding_char;
};

void tsinit(void) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void tsclear_screen() {
	for (uint32_t i = 0; i < ((SCREEN_WIDTH * SCREEN_HEIGHT) >> 2); i++) {
		((uint32_t*)videomem)[i] = (uint32_t)0;
	}
}

void tscursor_set(uint8_t x, uint8_t y) {
	cursor_x = x;
	cursor_y = y;
}

void tsputch(char ch, uint8_t color) {
	const char tmp[2] = { ch, 0 };
	tsputs(tmp, color);
}

static 
void handle_special_char(char c) {
	switch (c) {
		case '\n': {
			cursor_x = 0;
			cursor_y++;

			if (cursor_y >= SCREEN_HEIGHT) {
				cursor_y = 0;
			}
		} break;

		case '\r': cursor_x = 0; break;
		case '\t': cursor_x += 4; cursor_x -= cursor_x % 4; break;
	}
}

void tsputs(const char* ch, uint8_t color) {
	struct text_screen_char tsch = {
		.character = '\0',
		.attribute = (color & 0x7F)
	};

	for(const char* c = ch; *c != '\0'; c++) {
		if ((uint8_t)(*c) < 0x20) {
			handle_special_char(*c);
			continue;
		}

		tsch.character = *c;

		videomem[cursor_y * SCREEN_WIDTH + cursor_x] = tsch;

		cursor_x++;
		if (cursor_x >= SCREEN_WIDTH) {
			cursor_y++;
			cursor_x = 0;

			if (cursor_y >= SCREEN_HEIGHT) {
				cursor_y = 0;	
			}
		}
	}
}

static 
const char* decode_format_specifier_flag(const char* str, struct printf_format_specifier* data) {
	switch (*str) {
		case '0':
			data->padding_char = '0';
			return str+1;
	}

	data->padding_char = '\0';
	return str;
}

static
const char* decode_format_specifier_field_width(const char* str, struct printf_format_specifier* data) {
	char tmp[16];

	int i;
	for(i = 0; i < 15; i++) {
		if (*str < '0' || *str > '9') {
			break;
		}

		tmp[i] = *str++;
	}
	tmp[i] = '\0';

	data->field_size = atoi(tmp);
	return str;
}

static 
const char* decode_format_spefifier(const char* str, struct printf_format_specifier* data) {
	switch (*str) {
		case 'd':
		case 'i':
			data->type = FORMAT_SPECIFIER_INT;
			return str+1;

		case 'o':
		case 'u':
			data->type = FORMAT_SPECIFIER_UNSIGNED;
			return str+1;

		case 'x':
		case 'X':
			data->type = FORMAT_SPECIFIER_HEXADECIMAL;
			return str+1;
	}

	data->type = FORMAT_SPEFICIER_UNKNOWN;
	return str;
}

static 
void put_formated_string(const char* str, const struct printf_format_specifier* specifier) {
	// Padding
	if (specifier->padding_char != '\0') {
		size_t length = strlen(str);

		while ((int)length < specifier->field_size) {
			tsputch(specifier->padding_char, TEXT_SCREEN_BG_BLACK | TEXT_SCREEN_FG_WHITE);
			length++;
		}
	}

	tsputs(str, TEXT_SCREEN_BG_BLACK | TEXT_SCREEN_FG_WHITE);
}

static 
const char* handle_format_specifier(const char* str, va_list* args) {
	str++;
	char tmp[16];
	
	struct printf_format_specifier specifier;

	str = decode_format_specifier_flag(str, &specifier);
	str = decode_format_specifier_field_width(str, &specifier);

	str = decode_format_spefifier(str, &specifier);
	switch (specifier.type) {
		case FORMAT_SPECIFIER_INT: {
			int value = va_arg(*args, int);
			put_formated_string(itoa(value, tmp, 10), &specifier);
		} break;

		case FORMAT_SPECIFIER_UNSIGNED: {
			unsigned int value = va_arg(*args, unsigned int);
			put_formated_string(utoa(value, tmp, 10), &specifier);
		} break;


		case FORMAT_SPECIFIER_HEXADECIMAL: {
			unsigned int value = va_arg(*args, unsigned int);
			put_formated_string(utoa(value, tmp, 16), &specifier);
		} break;
		default: return str;
	}

	return str-1;
}

void tsprintf(const char* format, ...) {
	struct text_screen_char tsch = {
		.character = '\0',
		.attribute = TEXT_SCREEN_BG_BLACK | TEXT_SCREEN_FG_GRAY
	};

	va_list args;
	va_start(args, format);

	for (const char* c = format; *c != '\0'; c++) {
		if (*c == '%') {
			c = handle_format_specifier(c, &args);
			continue;
		}

		if ((uint8_t)(*c) < 0x20) {
			handle_special_char(*c);
			continue;
		}

		tsch.character = *c;

		videomem[cursor_y * SCREEN_WIDTH + cursor_x] = tsch;

		cursor_x++;
		if (cursor_x >= SCREEN_WIDTH) {
			cursor_y++;
			cursor_x = 0;

			if (cursor_y >= SCREEN_HEIGHT) {
				cursor_y = 0;	
			}
		}
	}

	va_end(args);
}
