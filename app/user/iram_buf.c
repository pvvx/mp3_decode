/*
 * iram_buf.c
 *
 *      Author: pvvx
 */
#include "user_config.h"
#include "esp_common.h"
#include "bios/spiflash.h"
#include "iram_buf.h"

#ifndef DEBUGSOO
#define DEBUGSOO 2
#endif

#ifndef ICACHE_RODATA_ATTR
#define ICACHE_RODATA_ATTR __attribute__((section(".irom.text")))
#endif

#if DEBUGSOO > 0
const char *txtSPIFlashInterface[] = {"QIO","QOUT","DIO","DOUT"};
const char *txtFSize[] = {"512K", "256K", "1M", "2M", "4M"};
const char *txtSFreq[] = {"40MHz","26MHz","20MHz","80MHz"};
#endif


#define flash_read spi_flash_read

ERAMInfo eraminfo;

bool ICACHE_FLASH_ATTR get_eram_size(ERAMInfo *einfo) {
	struct SPIFlashHeader x;
	int i = 1;
	uint32 faddr = 0x000;
//	einfo->use = false;
	einfo->base = NULL;
	einfo->size = 0;
	if (flash_read(faddr, (uint32 *)&x, 8) != 0)
		return false;
	faddr += 8;
#if DEBUGSOO > 0
	printf("Flash Header:\n");
#endif
	if (x.head.id != 0xE9) {
#if DEBUGSOO > 0
		printf(" Bad Header!\n");
#endif
		return false;
	} else {
#if DEBUGSOO > 0
/*		uint32 speed = x.head.hsz.spi_freg;
		if(speed > 2) {
			if(speed == 15) speed = 3;
			else speed = 0;
		} */
		printf(
				" Number of segments: %u\n SPI Flash Interface: %s\n SPI CLK: %s\n Flash size: %s\n Entry point: %p\n",
				x.head.number_segs, txtSPIFlashInterface[x.head.spi_interface & 3], txtSFreq[x.head.hsz.spi_freg & 3],
				txtFSize[x.head.hsz.flash_size&3], x.entry_point);
#endif
		while (x.head.number_segs) {
			if (flash_read(faddr, (uint32 *)&x.seg, 8) != 0) {
#if DEBUGSOO > 0
				printf("flash read error!");
#endif
				return false;
			};
			if ((x.seg.memory_offset >= IRAM1_BASE)
					&& (x.seg.memory_offset < (IRAM1_BASE + IRAM1_SIZE))) {
				if (((x.seg.memory_offset + x.seg.segment_size) > (uint32)einfo->base)
						&& ((x.seg.memory_offset + x.seg.segment_size)
								< (IRAM1_BASE + IRAM1_SIZE))) {
					einfo->size = IRAM1_SIZE - x.seg.segment_size;
					einfo->base = (uint32 *)(x.seg.memory_offset + x.seg.segment_size);
				};
			};
#if DEBUGSOO > 0
			printf(" Segment %u: offset: %p, size: %u\n", i, x.seg.memory_offset,
					x.seg.segment_size);
#endif
			x.head.number_segs--;
			i++;
			faddr += x.seg.segment_size + 8;
		};
	};
	if((eraminfo.base)&&(eraminfo.size > MIN_GET_IRAM)) return true;
	return false;
}

void ICACHE_FLASH_ATTR eram_init(void) {
	if(get_eram_size(&eraminfo)) {
#if DEBUGSOO > 0
		printf("Found free IRAM: base:%p, size:%u bytes\n", eraminfo.base,  eraminfo.size);
#endif
		memset(eraminfo.base, 0, eraminfo.size);
	}
}


bool ICACHE_FLASH_ATTR eRamRead(uint32 addr, uint8 *pd, uint32 len)
{
	union {
		uint8 uc[4];
		uint32 ud;
	}tmp;
	if (addr + len > eraminfo.size) return false;
	uint32 *p = (uint32 *)(((uint32)eraminfo.base + addr) & (~3));
	uint32 xlen = addr & 3;
	if(xlen) {
		tmp.ud = *p++;
		while (len)  {
			*pd++ = tmp.uc[xlen++];
			len--;
			if(xlen >= 4) break;
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
	len &= 3;
	if(len) {
		tmp.ud = *p;
		uint8 * ptmp = tmp.uc;
		while (len--)  *pd++ = *ptmp++;
	}
	return true;
}

bool ICACHE_FLASH_ATTR eRamWrite(uint32 addr, uint8 *pd, uint32 len)
{
	union {
		uint8 uc[4];
		uint32 ud;
	}tmp;
	if (addr + len > eraminfo.size) return false;
	uint32 *p = (uint32 *)(((uint32)eraminfo.base + addr) & (~3));
	uint32 xlen = addr & 3;
	if(xlen) {
		tmp.ud = *p;
		while (len)  {
			tmp.uc[xlen++] = *pd++;
			len--;
			if(xlen >= 4) break;
		}
		*p++ = tmp.ud;
	}
	xlen = len >> 2;
	while(xlen) {
		tmp.uc[0] = *pd++;
		tmp.uc[1] = *pd++;
		tmp.uc[2] = *pd++;
		tmp.uc[3] = *pd++;
		*p++ = tmp.ud;
		xlen--;
	}
	len &= 3;
	if(len) {
		tmp.ud = *p;
		uint8 * ptmp = tmp.uc;
		while (len--)  *ptmp++ = *pd++;
		*p = tmp.ud;
	}
	return true;
}

