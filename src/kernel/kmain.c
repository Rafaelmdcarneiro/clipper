#include "include/internal/text_screen.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "internal/havarie.h"

#if defined(__linux__)
#	error "Please use a platform agnostic compiler"
#endif

#if !defined(__i386__)
#	error "Please use an ix86-elf compiler"
#endif

#include "internal/text_screen.h"

void kmain(void) {
	tsinit();

	const char* msg = "Clipper set sail!\n~~~~~~~~~~~~~~~~~\n";
	tsputs(msg, TEXT_SCREEN_BG_BLACK | TEXT_SCREEN_FG_LIGHT_CYAN);

	tsprintf("Test | Integers: %d, Unsigned: %u, Hexadecimal: %x\n", -48, 28493, 28493);

	capsize();

	return;
}
