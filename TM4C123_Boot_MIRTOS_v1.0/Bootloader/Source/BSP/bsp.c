/* Board Support Package */
#include "TM4C123GH6PM.h"
#include "bsp.h"

#include "GPIO_Driver_Interface.h"

static uint32_t volatile l_tickCtr;

__attribute__((naked)) void assert_failed (char const *file, int line)
{
    /* TBD: damage control */
    NVIC_SystemReset(); /* reset the system */
}

void SysTick_Handler(void)
{
    static uint8_t flag = 0U;

    flag^=1;
//    GPIOF_AHB->DATA_Bits[LED_BLUE] ^= LED_BLUE;
    DigitalWrite(PF1,(GPIO_HI_LO)flag);

    ++l_tickCtr;
}

