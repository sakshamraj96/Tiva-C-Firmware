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
#include "bsp.h"
#include "mirtos.h"

/******************************************************************************/
void led1();
void led2();
void led3();
void delay_ms(uint32_t val);

/**********************************************************/
uint32_t stack_led1[40];
OSThread led1_thread;

uint32_t stack_led2[40];
OSThread led2_thread;

uint32_t stack_led3[40];
OSThread led3_thread;

/*********************************************************************/
void led1()
{
    OSThread_start(&led2_thread,
                         &led2,
                         stack_led2,
                         sizeof(stack_led2));
  while(1)
  {
      DigitalWrite(PF1, HIGH);
      BSP_delay(BSP_TICK_PER_SEC / 2U);
      DigitalWrite(PF1, LOW);
      BSP_delay((BSP_TICK_PER_SEC * 3U) / 1U);


  }
}

/**********************************************************/


void led2()
{
    OSThread_start(&led3_thread,
                   &led3,
                   stack_led3,
                   sizeof(stack_led3));

  while(1)
  {
      DigitalWrite(PF2, HIGH);
      BSP_delay(BSP_TICK_PER_SEC / 4U);
      DigitalWrite(PF2, LOW);
      BSP_delay((BSP_TICK_PER_SEC * 3U) / 4U);
  }
}

/**********************************************************/


void led3()
{
  while(1)
  {
      DigitalWrite(PF3, HIGH);
      BSP_delay(BSP_TICK_PER_SEC / 3U);
      DigitalWrite(PF3, LOW);
      BSP_delay((BSP_TICK_PER_SEC * 3U) / 3U);
  }
}
  
/**********************************************************/
uint32_t volatile* intreg;
void main()
{
    BSP_init();
    OS_init();
    
    OSThread_start(&led1_thread,
                   &led1,
                   stack_led1,
                   sizeof(stack_led1));
                   

    
    /* transfer control to the RTOS to run the threads */
    OS_run();
    
}


void delay_ms(uint32_t val)
{
    for(uint32_t i = 0 ; i<val ; i++)
    {
      for(int j = 0 ; j < 1000 ; j++)
      {
        ;
      }
    }
}

/**********************************************************/


























//#include<stdio.h>
//#include "TM4C123GH6PM.h"
//#include "GPIO_Driver_Interface.h"
//#include "Timer_Interface.h"
//#include "ADC_Driver_Interface.h"
//#include "Common_Datatype.h"
//#include "Common_Macros.h"
////#include "LCD_Interface.h"
//#include "I2C_Interface.h"
////#include "SPI_Interface.h"
////#include "MCP3208_Interface.h"
////#include "CAN_Interface.h"
//#include "UART_Interface.h"
//#include "timercnter_interface.h"
//#include "DS3231_Interface.h"
//#include "SSD1306_Interface.h"
//
//
///* system clock setting [Hz] */
//#define SYS_CLOCK_HZ 16000000U
//
//
//void main()
//{
//    SysTick->LOAD = (SYS_CLOCK_HZ / 2U) - 1U ;
//    SysTick->VAL  = 0U;
//    SysTick->CTRL = (1U << 2) | (1U << 1) | (1U << 0);
//
//    __enable_irq();
//
//   // SysTick_Handler();
//
//    while(1)
//    {
//        static uint8_t flag = 0U;
//
//        flag^=1;
//    //    GPIOF_AHB->DATA_Bits[LED_BLUE] ^= LED_BLUE;
////        DigitalWrite(PF3,(GPIO_HI_LO)flag);
//    }
//}
