/*
 * iram_buf.h
 *
 *      Author: pvvx
 */
#ifndef __EXRTA_RAM_H_
#define __EXRTA_RAM_H_

#define IRAM1_BASE	0x40100000
#define IRAM1_SIZE	0x00008000
#define MIN_GET_IRAM 4096

typedef struct t_eraminfo
{
	uint32 *base;
	uint32 size;
}ERAMInfo;

//#define eram_base (*(volatile uint32*)(IRAM1_BASE + IRAM1_SIZE - 8))
//#define eram_size (*(volatile uint32*)(IRAM1_BASE + IRAM1_SIZE - 4))

void eram_init(void) ICACHE_FLASH_ATTR;
extern ERAMInfo eraminfo;

bool eRamRead(uint32 addr, uint8 *pd, uint32 len) ICACHE_FLASH_ATTR;
bool eRamWrite(uint32 addr, uint8 *pd, uint32 len) ICACHE_FLASH_ATTR;


#endif /* __EXRTA_RAM_H_ */
