#include "TM4C123GH6PM.h"
#include "UART.h"

/********************* Structure Defination*** ************************/
UART_config_type UART_config[] = 
{
  {
    .UART_Perif_Addr     = (UART0_Type*)UART0,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOA,
    .GPIO_Perif_Index    = 0,
    .RX_Pin              = 0,
    .TX_Pin              = 1,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART1,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOB,
    .GPIO_Perif_Index    = 1,
    .RX_Pin              = 0,
    .TX_Pin              = 1,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART2,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOD,
    .GPIO_Perif_Index    = 3,
    .RX_Pin              = 6,
    .TX_Pin              = 7,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART3,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOC,
    .GPIO_Perif_Index    = 2,
    .RX_Pin              = 6,
    .TX_Pin              = 7,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART4,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOC,
    .GPIO_Perif_Index    = 2,
    .RX_Pin              = 4,
    .TX_Pin              = 5,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART5,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOE,
    .GPIO_Perif_Index    = 4,
    .RX_Pin              = 4,
    .TX_Pin              = 5,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART6,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOD,
    .GPIO_Perif_Index    = 3,
    .RX_Pin              = 4,
    .TX_Pin              = 5,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART7,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOE,
    .GPIO_Perif_Index    = 4,
    .RX_Pin              = 0,
    .TX_Pin              = 1,
    .GPIOPCTL_Index      = 1
  }
};

/********************************************************************************/
static uint8_t UART_Init_Flag = 0U;
static uint32_t UART_Index = 0U;
static uint32_t BaudRate_Integer = 0;
static float   BaudRate_Float   = 0.0;
uint32_t temp;

/*********************************************************************************/

void UART_init(UART_Channel UART_Ch_Index, uint32_t UART_BaudRate)
{
    UART_Index = (uint32_t)UART_Ch_Index;

    SYSCTL->RCGCGPIO |= (1<<(UART_config[UART_Index].GPIO_Perif_Index));                /* enable clock to GPIO PORT*/
    SYSCTL->RCGCUART |= (1<<(UART_Index));                                              /* enable clock to UART */
    Delay(1);
    /* UART initialization */
    UART_config[UART_Index].UART_Perif_Addr->CTL = 0;                                   /* UART module disbable */
    
    BaudRate_Integer = ((1000000)/UART_BaudRate);                                       /* (16000000/16)/UART_BaudRate */
    UART_config[UART_Index].UART_Perif_Addr->IBRD = BaudRate_Integer;                   /* for 9600 baud rate, integer = 104 */
    
    BaudRate_Float     = (1000000.0/(float)UART_BaudRate) - (float)BaudRate_Integer;
    BaudRate_Integer   = (uint32_t)((BaudRate_Float*(float)64.0) + (float)0.5);
    UART_config[UART_Index].UART_Perif_Addr->FBRD = BaudRate_Integer;                    /* for 9600 baud rate, fractional = 11*/
    
    UART_config[UART_Index].UART_Perif_Addr->CC = 0;                                    /*select system clock*/
    UART_config[UART_Index].UART_Perif_Addr->LCRH = 0x60;                               /* data lenght 8-bit, not parity bit, no FIFO */
    UART_config[UART_Index].UART_Perif_Addr->CTL = 0x301;                               /* Enable UART module, Rx and Tx */

    /* Configure them digital and enable alternate function */
    UART_config[UART_Index].GPIO_Perif_Addr->DEN   |= ((1<<(UART_config[UART_Index].RX_Pin)) | (1<<(UART_config[UART_Index].TX_Pin)));    /* set PE4 and PE5 as digital */
    UART_config[UART_Index].GPIO_Perif_Addr->AFSEL |= ((1<<(UART_config[UART_Index].RX_Pin)) | (1<<(UART_config[UART_Index].TX_Pin)));    /* Use PE4,PE5 alternate function */
    UART_config[UART_Index].GPIO_Perif_Addr->AMSEL &= ~((1<<(UART_config[UART_Index].RX_Pin)) | (1<<(UART_config[UART_Index].TX_Pin)));    /* Turn off analg function*/
    UART_config[UART_Index].GPIO_Perif_Addr->PCTL  |= ((UART_config[UART_Index].GPIOPCTL_Index<<((UART_config[UART_Index].RX_Pin)*4)) | (UART_config[UART_Index].GPIOPCTL_Index<<((UART_config[UART_Index].TX_Pin)*4)));      /* configure PE4 and PE5 for UART */
	
    UART_Init_Flag = 1;
}

void UART_print(uint8_t *data)
{
	if(UART_Init_Flag == 0)
	{
		UART_init(UART_0, 9600);
	}
	
	else
	{
		while(*data)
		{
			UART_Transmitter(*(data++));
		}
	}
}


void UART_Transmitter(char data)  
{
    while((UART_config[UART_Index].UART_Perif_Addr->FR & 0x20) != 0);           /* wait until Tx buffer not full */
    UART_config[UART_Index].UART_Perif_Addr->DR = data;                         /* before giving it another byte */
}

void Delay(unsigned long counter)
{
	unsigned long i = 0;
	
	for(i=0; i< counter; i++);
}





uint8_t UART_Receive() 
{
    uint8_t rx_data = 0xff;

    /* Wait until there is data available to receive */
    while(!UART_RxReady());

    /* Read the received data */
    rx_data = UART_config[UART_Index].UART_Perif_Addr->DR;

    return rx_data;
}


uint8_t UART_RxReady()
{
    return ((UART_config[(uint32_t)UART_Index].UART_Perif_Addr->FR & UART_FR_RXFE) == 0);
}
