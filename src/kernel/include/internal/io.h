#ifndef _CLIPPER_IO_H_
#define _CLIPPER_IO_H_

#include <stdint.h>

#define READ_REGISTER_VALUE(var, reg) asm ("mov %%" #reg ", %[" #var "]" : [var]"=m" (var))

inline void outb(uint16_t port, uint8_t val) {
	asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

#endif // _CLIPPER_IO_H_
