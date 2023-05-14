#ifndef _UART_
#define _UART_

//#define SYSCTL_RCGCFLASH_R       (*((volatile uint32_t *)0x400FEA08))
#define FLASH_FMA              (*((volatile uint32_t *)0x400FD000))
#define FLASH_FMD              (*((volatile uint32_t *)0x400FD004))
#define FLASH_FMC              (*((volatile uint32_t *)0x400FD008))
#define FLASH_FCRIS_R            (*((volatile uint32_t *)0x400FEA08))
#define FLASH_FMC_WRKEY          0xA4420000
#define FLASH_FMC_WRITE          0x1
#define FLASH_FMC_ERASE          0x2
#define FLASH_FMC_MERASE         0x4
#define FLASH_FMC_COMT           0x8

#define FLASH_FMC_RDY            0x00000001

#define FLASH_FMC_START          0x00000001
#define FLASH_FMC_MERASE         0x00000010

#define FLASH_BASE_ADDRESS       0x00000000
#define FLASH_BLOCK_SIZE         1024  // 1KB flash block size for TM4C123

#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))
#define SYSCTL_RCGC2_FLASH      0x00000020

#define FLASH_FMC2_R          (*((volatile uint32_t *)0x400FC020))
#define FLASH_FMC2_WRBUF      0x00000001  // Write Buffer Clear
#define FLASH_FMC2_WRKEY      0xA4420000  // Write Key



void FlashProgram(uint32_t address, uint32_t *data, uint32_t count);
void FlashProgram_ByteArr(uint32_t address, uint8_t *data, uint32_t count);
void FlashErase(uint32_t address, uint32_t count);


#endif
