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

void delay(uint8_t count);

//#define UART_LOG_EN 1

void main()
{ 
  int cnt = 0;
  unsigned int curr_time = 0;
  unsigned int temp_time = 0;
  unsigned int prev_time = 0;
  unsigned char str[100] = {0};
  unsigned char str1[100] = {0};
  int fps = 0;
  
  
  DigitalWrite(PB0,LOW );
  DigitalWrite(PB1, HIGH);
  delay(10);
  
  init_Timer_Counter();
  I2C_Init(I2C_2, 500);
  UART_init(UART_0, 115200);
  ssd1306_init();
  
  Start_Timer_counter();
  
    while (1) 
    { 

#ifdef UART_LOG_EN
        curr_time = 0;
        curr_time = millis(); 
#endif        
        cnt+=3;
        ssd1306_clear();
        ssd1306_DrawCircle(cnt, 32, 10);
        if(cnt >= 127)
          cnt = 0;
        ssd1306_draw_line(0, 43, 127, 43, 1); 

#ifdef UART_LOG_EN
        temp_time = curr_time;
        curr_time = millis();
        temp_time = curr_time - temp_time;
        sprintf(str,"ssd1306 driver buffer update time = %d\r\n", temp_time);
        UART_print(str);
#endif 
        
        ssd1306_refresh();    /* max time consuming */
        
#ifdef UART_LOG_EN        
        temp_time = curr_time;
        curr_time = millis();
        temp_time = curr_time - temp_time;
        sprintf(str,"ssd1306 oled buffer time = %d\r\n", temp_time);
        UART_print(str);
#endif         

        curr_time = millis();
        fps = 1000/curr_time;
        sprintf(str1,"FPS = %d\r\n", fps);
        ssd1306_setcursor(7,5);
        ssd1306_Print_String(str1);

        
        
#ifdef UART_LOG_EN        
        temp_time = curr_time;
        curr_time = millis();
        temp_time = curr_time - temp_time;
        sprintf(str,"ssd1306 oled fps update time = %d\r\n", temp_time);
        UART_print(str);
#endif  
        
        Refresh_Timer_counter();
    }
}

void delay(uint8_t count)
{
	uint32_t i = 0;
	uint32_t j = 0;
	for(i=0 ; i<count ; i++)
	{
		for(j=0 ; j < 3180 ; j++);
	}
}
