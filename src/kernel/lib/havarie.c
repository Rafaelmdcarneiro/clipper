#include "internal/havarie.h"

#include "internal/text_screen.h"
#include "stdlib.h"
#include "string.h"
#include "internal/io.h"

void capsize() {
	print_havarie_msg();

	asm("cli\n"
		"hlt");

	for(;;);
}

static inline 
uint8_t peek(void* addr) {
	uint32_t ret;
    asm ( "push %%fs\n\t"
    	  "mov  $0, %%eax\n\t"
          "mov  %%ax, %%fs\n\t"
          "mov  %%fs:(%1), %0\n\t"
          "pop  %%fs"
          : "=r"(ret) : "r"(addr) );
    return ret;
}

static
void dump_stack(uint8_t col, uint8_t row) {
	tscursor_set(col, row);
	tsputs("Stack dump", TEXT_SCREEN_BG_BLACK | TEXT_SCREEN_FG_LIGHT_BLUE);
	row++;

	tscursor_set(col, row);
	tsputs("          0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F", TEXT_SCREEN_BG_BLACK | TEXT_SCREEN_FG_GRAY);
	row++;

	int esp, ebp;
	READ_REGISTER_VALUE(esp, esp);
	READ_REGISTER_VALUE(ebp, ebp);

	int start_address = esp & ~0xF;
	char tmp[16];
	for (int table_row = 0; table_row < 16; table_row++) {
		tscursor_set(col, row);
		
		itoa(start_address, tmp, 16);
		size_t len = strlen(tmp);

		char* src = tmp + len - 1;
		char* dst = tmp + 7;

		if (len < 9) {
			do {
				*dst-- = *src--;	
			} while(src >= tmp);

			do {
				*dst-- = '0';	
			} while (src != dst);
		}

		tsputs(tmp, TEXT_SCREEN_BG_BLACK | TEXT_SCREEN_FG_GRAY);
		tsprintf("  ");

		for (int table_col = 0; table_col < 4; table_col++) {
			uint8_t x = peek((void*)(long)(start_address + 4 * table_col + 0));
			uint8_t y = peek((void*)(long)(start_address + 4 * table_col + 1));
			uint8_t z = peek((void*)(long)(start_address + 4 * table_col + 2));
			uint8_t w = peek((void*)(long)(start_address + 4 * table_col + 3));

			tsprintf("%02x %02x %02x %02x  ", x, y, z, w);
		}

		row++;

		start_address -= 0x10;
	}
}

void print_havarie_msg() {
	tsclear_screen();
	tscursor_set(0, 0);
	tsputs("Havarie - the clipper has capsized\n\n\n", TEXT_SCREEN_BG_BLACK | TEXT_SCREEN_FG_LIGHT_CYAN);

	
	int eax, ebx, ecx, edx, esi, edi, esp, ebp;
	READ_REGISTER_VALUE(eax, eax);
	READ_REGISTER_VALUE(ebx, ebx);
	READ_REGISTER_VALUE(ecx, ecx);
	READ_REGISTER_VALUE(edx, edx);
	READ_REGISTER_VALUE(esi, esi);
	READ_REGISTER_VALUE(edi, edi);
	READ_REGISTER_VALUE(esp, esp);
	READ_REGISTER_VALUE(ebp, ebp);

	tsputs("Registers\n", TEXT_SCREEN_BG_BLACK | TEXT_SCREEN_FG_LIGHT_BLUE);
	tsprintf("eax=0x%08x\nebx=0x%08x\necx=0x%08x\nedx=0x%08x\n\n", eax, ebx, ecx, edx);
	tsprintf("esi=0x%08x\nedi=0x%08x\nesp=0x%08x\nebp=0x%08x\n\n", esi, edi, esp, ebp);

	short cs, ds, es, ss, fs, gs;
	READ_REGISTER_VALUE(cs, cs);
	READ_REGISTER_VALUE(ds, ds);
	READ_REGISTER_VALUE(es, es);
	READ_REGISTER_VALUE(ss, ss);
	READ_REGISTER_VALUE(fs, fs);
	READ_REGISTER_VALUE(gs, gs);

	tsputs("\nSegments\n", TEXT_SCREEN_BG_BLACK | TEXT_SCREEN_FG_LIGHT_BLUE);
	tsprintf("cs=0x%04x\nds=0x%04x\nes=0x%04x\n\n", cs, ds, es);
	tsprintf("ss=0x%04x\nfs=0x%04x\ngs=0x%04x\n\n", ss, fs, gs);

	dump_stack(18, 3);
}
