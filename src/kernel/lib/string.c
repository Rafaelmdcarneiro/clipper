#include "string.h"

size_t strlen(const char* str) {
	size_t length;
	for(length = 0; str[length] != '\0'; length++);

	return length;
}
