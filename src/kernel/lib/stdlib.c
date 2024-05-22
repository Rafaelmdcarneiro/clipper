#include "stdlib.h"

int atoi(const char* str) {
	// Blled away whitespace
	while(*str++ == ' ');
	str--;

	int val = 0;
	int factor = 1;

	if (*str == '+') { 
		str++; 
	} else if (*str == '-') {
		factor = -1;
		str++;
	}

	const char* start = str;
	while (*str >= '0' && *str <= '9') {
		str++;
	}

	int power_of_ten = 1;
	while (start < str) {
		val += power_of_ten * ((*(--str)) - '0');
		power_of_ten *= 10;
	}

	return factor * val;
}

char* itoa(int value, char* buffer, int base) {
	char* ptr;
	char* value_start;

	if (base < 2 || base > 35) {
		*buffer = '\0';
		return buffer;
	}

	ptr = buffer;

	if (value < 0 && base == 10) {
		*ptr++ = '-';
	}

	value_start = ptr;

	do {
		short remainder = value % base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + remainder];

		value /= base;
	} while(value);

	*ptr-- = '\0';

	while (value_start < ptr) {
		char tmp = *ptr;
		*ptr-- = *value_start;
		*value_start++ = tmp;
	}

	return buffer;
}

char* utoa(unsigned int value, char* buffer, int base) {
	char* ptr;
	char* value_start;

	if (base < 2 || base > 35) {
		*buffer = '\0';
		return buffer;
	}

	ptr = value_start = buffer;

	do {
		short remainder = value % base;
		*ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[remainder];

		value /= base;
	} while(value != 0);

	*ptr-- = '\0';

	while (value_start < ptr) {
		char tmp = *ptr;
		*ptr-- = *value_start;
		*value_start++ = tmp;
	}

	return buffer;
}

