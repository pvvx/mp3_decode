/*
 * iram_buf.h
 *
 *      Author: pvvx
 */
#ifndef __EXRTA_RAM_H_
#define __EXRTA_RAM_H_

#define IRAM1_BASE	0x40100000
#define IRAM1_SIZE	0x0000C000
#define MIN_GET_IRAM 4096

typedef struct t_eraminfo // info buffer iram
{
	uint32 *base;
	int32 size;
}ERAMInfo;

extern ERAMInfo eraminfo;

int eRam_init(void) ICACHE_FLASH_ATTR;
bool eRamRead(uint32 addr, uint8 *pd, uint32 len) ICACHE_FLASH_ATTR;
bool eRamWrite(uint32 addr, uint8 *pd, uint32 len) ICACHE_FLASH_ATTR;


#endif /* __EXRTA_RAM_H_ */
