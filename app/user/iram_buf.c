/*
 * iram_buf.c
 *
 *      Author: pvvx
 */
#include "user_config.h"
#include "esp_common.h"
#include "iram_buf.h"

#ifndef DEBUGSOO
#define DEBUGSOO 2
#endif

#ifndef ICACHE_RODATA_ATTR
#define ICACHE_RODATA_ATTR __attribute__((section(".irom.text")))
#endif
#ifndef ICACHE_RAM_ATTR
#define ICACHE_RAM_ATTR
#endif

#define flash_read spi_flash_read

extern uint32 _text_start[]; // start addr IRAM
extern uint32 _lit4_start[]; // addr data buf IRAM

/* dport (io1) section */
extern volatile uint32 dport_[64];		// 0x3ff00000
#define DPORT_BASE	dport_		// 0x3ff00000

int ICACHE_FLASH_ATTR eRam_init(void)
{
	 uint32 * end = &_text_start[((((DPORT_BASE[9]>>3)&3)==3)? (0x08000 >> 2) : (0x0C000 >> 2))];
	 eraminfo.size = (int32)((uint32)(end) - (uint32)eraminfo.base);
	 if(eraminfo.size > 0) {
		 uint32 * ptr = _lit4_start;
		 while(ptr < end) *ptr++ = 0;
	 }
	 else printf("No free IRAM!");
	 return eraminfo.size;
}

void ICACHE_RAM_ATTR copy_s4d1(unsigned char * pd, void * ps, unsigned int len)
{
	union {
		unsigned char uc[4];
		unsigned int ud;
	}tmp;
	unsigned int *p = (unsigned int *)((unsigned int)ps & (~3));

	unsigned int xlen = (unsigned int)ps & 3;
	if(xlen) {
		tmp.ud = *p++;
		while (len)  {
			len--;
			*pd++ = tmp.uc[xlen++];
			if(xlen & 4) break;
		}
	}
	xlen = len >> 2;
	while(xlen) {
		tmp.ud = *p++;
		*pd++ = tmp.uc[0];
		*pd++ = tmp.uc[1];
		*pd++ = tmp.uc[2];
		*pd++ = tmp.uc[3];
		xlen--;
	}
	if(len & 3) {
		tmp.ud = *p;
		pd[0] = tmp.uc[0];
		if(len & 2) {
			pd[1] = tmp.uc[1];
			if(len & 1) {
				pd[2] = tmp.uc[2];
			}
		}
	}
}


void ICACHE_RAM_ATTR copy_s1d4(void * pd, unsigned char * ps, unsigned int len)
{
	union {
		unsigned char uc[4];
		unsigned int ud;
	}tmp;
	unsigned int *p = (unsigned int *)(((unsigned int)pd) & (~3));
	unsigned int xlen = (unsigned int)pd & 3;
	if(xlen) {
		tmp.ud = *p;
		while (len)  {
			len--;
			tmp.uc[xlen++] = *ps++;
			if(xlen & 4) break;
		}
		*p++ = tmp.ud;
	}
	xlen = len >> 2;
	while(xlen) {
		tmp.uc[0] = *ps++;
		tmp.uc[1] = *ps++;
		tmp.uc[2] = *ps++;
		tmp.uc[3] = *ps++;
		*p++ = tmp.ud;
		xlen--;
	}
	if(len & 3) {
		tmp.ud = *p;
		tmp.uc[0] = ps[0];
		if(len & 2) {
			tmp.uc[1] = ps[1];
			if(len & 1) {
				tmp.uc[2] = ps[2];
			}
		}
		*p = tmp.ud;
	}
}

//extern void copy_s4d1(uint8 * pd, void * ps, uint32 len);

bool ICACHE_RAM_ATTR eRamRead(uint32 addr, uint8 *pd, uint32 len)
{
	if (addr + len > eraminfo.size) return false;
	copy_s4d1(pd, (void *)((uint32)eraminfo.base + addr), len);
	return true;
}

//extern void copy_s1d4(void * pd, uint8 * ps, uint32 len);

bool ICACHE_RAM_ATTR eRamWrite(uint32 addr, uint8 *ps, uint32 len)
{
	if (addr + len > eraminfo.size) return false;
	copy_s1d4((void *)((uint32)eraminfo.base + addr), ps, len);
	return true;
}

