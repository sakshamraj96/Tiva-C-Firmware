#include<stdio.h>
#include "TM4C123GH6PM.h"
#include "GPIO_Driver_Interface.h"
#include "Timer_Interface.h"
#include "ADC_Driver_Interface.h"
#include "Common_Datatype.h"
#include "Common_Macros.h"
//#include "LCD_Interface.h"
#include "I2C_Interface.h"
//#include "SPI_Interface.h"
//#include "MCP3208_Interface.h"
//#include "CAN_Interface.h"
#include "UART_Interface.h"
#include "timercnter_interface.h"
#include "DS3231_Interface.h"
#include "SSD1306_Interface.h"
#include "FLASH_Interface.h"

/******************************************************************************/
void Delay_ms(unsigned int delay_value);
unsigned char verify_apps(void);

/******************************************************************************/
//#define APPS_FLASH_START_ADDR    0x00020000
//#define APPS_STACK_PTR_ADDR      0x20004000
//#define APPS_STACK_SIZE          4096         // 4KB
#define APPS_FLASH_START_ADDR    0x00020000
#define APPS_STACK_PTR_ADDR      (*((unsigned int*)(APPS_FLASH_START_ADDR)))
#define APPS_RESET_HANDLER_ADDR  APPS_FLASH_START_ADDR + 4
#define APPS_INVALID             0xFFFFFFFF

#define apps_packet_size         1024  // no. of bytes

/******************************************************************************/
typedef void (*fun_ptr)(void);

/******************************************************************************/
uint8_t rx_data = 0x0;

/******************************************************************************/

void main()
{
//    uint32_t data = 0xa5a5a5a5;
//    FlashErase(APPS_FLASH_START_ADDR, 11396);
//    FlashProgram(APPS_FLASH_START_ADDR, &data, 1);
//    data = 0x5a5a5a5a;
//    FlashErase(APPS_FLASH_START_ADDR+4, 1);
//    FlashProgram(APPS_FLASH_START_ADDR+4, &data, 1);


    uint8_t temp1[4]   = {0};

    uint32_t rx_stack_ptr, rx_app_size , rx_cnt;
    uint8_t str1[64];

    DigitalWrite(PB0,LOW );
    DigitalWrite(PB1, HIGH);
    Delay_ms(100);

    I2C_Init(I2C_2, 500);
    ssd1306_init();
    ssd1306_setcursor(7,0);
    ssd1306_Print_String("Running Bootloader");

    UART_init(UART_0, 115200);



if(DigitalRead(PF0,PU) == 0)
{
    ssd1306_clear();
    ssd1306_refresh();
    ssd1306_setcursor(7,0);
    ssd1306_Print_String("Flash Apps Mode");

    rx_data = UART_Receive();
    if(rx_data == 0xAA) // app flasher text byte to check if receiver is ready
        UART_Transmit(0x55);
    else
    {
        while(1);
    }

    for(rx_cnt = 0 ; rx_cnt < 4 ; rx_cnt++)
    {
        temp1[rx_cnt] = UART_Receive();
    }

    rx_app_size  = (uint32_t)((temp1[3]<<24) | (temp1[2]<<16) | (temp1[1]<<8) | (temp1[0]<<0));

    sprintf(str1,"AppSize=%d\r\n", rx_app_size);
    ssd1306_setcursor(0,0);
    ssd1306_Print_String(str1);

//    UART_Transmit(0x55);


/***************************************************************************/
    uint32_t app_bin_size    = rx_app_size;
    uint8_t temp[apps_packet_size]       = {0};  //
    uint32_t no_of_blocks    = app_bin_size / apps_packet_size ;
    uint32_t no_of_bytes_rem = app_bin_size % apps_packet_size ;
    uint32_t block_cnt       = 0;
    uint32_t long_cnt        = 0;
    uint32_t long_data_cnt   = 0;
    uint32_t Apps_start_Addr = APPS_FLASH_START_ADDR;


    FlashErase(Apps_start_Addr, (rx_app_size/4));
    Delay_ms(10);

    UART_Transmit(0x55);

    for(block_cnt = 0 ; block_cnt < no_of_blocks ; block_cnt++)
    {
        for(long_data_cnt = 0 ; long_data_cnt < (apps_packet_size) ; long_data_cnt++)
        {
            temp[long_data_cnt] = UART_Receive();
        }

        FlashProgram_ByteArr(Apps_start_Addr, temp, apps_packet_size);
        Apps_start_Addr += apps_packet_size;


        if(block_cnt == 43)
        {
            ssd1306_setcursor(2,0);
            ssd1306_Print_String("Complete");
        }

        sprintf(str1,"block received =%d\r\n", block_cnt+1);
        ssd1306_setcursor(1,0);
        ssd1306_Print_String(str1);

        for(long_data_cnt = 0 ; long_data_cnt < (apps_packet_size) ; long_data_cnt++)
        {
            temp[long_data_cnt] = 0;
        }

        UART_Transmit(0x55);

    }




    for(long_cnt = 0 ; long_cnt < no_of_bytes_rem ; long_cnt++)
    {
        temp[long_cnt] = UART_Receive();
    }

    ssd1306_setcursor(2,0);
    ssd1306_Print_String("Complete!!!");

    FlashProgram_ByteArr(Apps_start_Addr, temp, no_of_bytes_rem);
}



//while(1);


    fun_ptr  jump_to_apps = (void(*)(void)) (*((unsigned int*)(APPS_RESET_HANDLER_ADDR)));

    //unsigned int *apps_stack_ptr = (APPS_STACK_PTR_ADDR + APPS_STACK_SIZE);
    unsigned int *apps_stack_ptr = APPS_STACK_PTR_ADDR;

    DigitalWrite(PF2, HIGH);
    Delay_ms(1000);
    DigitalWrite(PF2, LOW);
    ssd1306_setcursor(7,0);
    ssd1306_Print_String("Going to Apps");
    Delay_ms(500);

    if(verify_apps())
    {
        __set_MSP(apps_stack_ptr);
        jump_to_apps();
    }
    else
    {
        while(1)
            DigitalWrite(PF1,HIGH);
    }
}

/**************************************************************************************/
void Delay_ms(unsigned int delay_value)
{
    uint32_t i = 0;
    uint32_t j = 0;
    for(i=0 ; i<delay_value ; i++)
    {
        for(j=0 ; j < 3180 ; j++);
    }
}

/**************************************************************************************/
unsigned char verify_apps(void)
{
    if((*((unsigned int*)(APPS_RESET_HANDLER_ADDR))) != APPS_INVALID)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


