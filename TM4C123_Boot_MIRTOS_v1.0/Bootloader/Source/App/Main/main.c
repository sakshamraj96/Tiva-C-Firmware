/******************************************************************************/
#include "main.h"
/******************************************************************************/


void main()
{

    fun_ptr  jump_to_apps = (void(*)(void)) (*((unsigned int*)(APPS_RESET_HANDLER_ADDR)));
    unsigned int apps_stack_ptr = APPS_STACK_PTR_ADDR;

    if(verify_apps())
    {

        SCB->VTOR = APPS_FLASH_START_ADDR;  // change in Interrupt Vector Table address to Apps IntVtrTble
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


