#include <stdint.h>
#include "stm32f446xx.h"

void button_init(void);
void uart2_init(void);
void dma1_init(void);
void send_some_data (void);

#define BASE_ADDR_OF_GPIOC_PERI GPIOC

int main(void)
{
	button_init();
	uart2_init();
	send_some_data();
	dma1_init();
	
	while (1);
	return 0;
}

void send_some_data (void)
{
	char somedata[] ="Hellow World\r\n";
	
	USART_TypeDef *pUART2;
	pUART2 = USART2;
	
	//1. Make sure that TXE is set (TDR is empty)
	//if TXE is 1 put the byte
	
	uint32_t len = sizeof(somedata);
	
	for (uint32_t i = 0; i < len; i++)
	{
		//waiting for TXE became one
		while (!(pUART2->SR & (1 << 7)));
		pUART2->DR = somedata[i];
	}
	
}

void button_init(void)
{
	//button is connected to PC13. GPIOC Pin number 13.
	//1. enable the peripheral clock for the GPIOC peripheral
	//2. keep the GPIO pin in input mode
	//3. configure the edge detection on that GPIO pin
	//4. Enable the interrupt over that GPIO pin
	//5 Enable the clock for SYSCFG
	//6. Configuring the SYSCFG CR4 register
	//7. Enable the IRQ related to that GPIO in NVIC of the processor.
	
	GPIO_TypeDef *pGPIOC;
	pGPIOC = BASE_ADDR_OF_GPIOC_PERI;
	
	RCC_TypeDef *pRCC;
	pRCC = RCC;
	
	EXTI_TypeDef *pEXTI;
	pEXTI = EXTI;
	
	SYSCFG_TypeDef *pSYSCFG;
	pSYSCFG = SYSCFG;
	
	//1
	pRCC->AHB1ENR |= (1<<2);
	//2
	pGPIOC->MODER &= ~(0x3 << 26);
	//4
	pEXTI->IMR |= (1 << 13);
	//5
	pRCC->APB2ENR |= (1 << 14);
	//6
	pSYSCFG->EXTICR[3] &= ~ (0xF << 4); //Clearing
	pSYSCFG->EXTICR[3] |=   (0x2 << 4); //Setting
	//3
	pEXTI->FTSR |= (1 << 13);
	//7
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
}

void uart2_init(void)
{
	RCC_TypeDef *pRCC;
	pRCC = RCC;
	
	GPIO_TypeDef *pGPIOA;
	pGPIOA = GPIOA;
	
	USART_TypeDef *pUART2;
	pUART2 = USART2;
	
	//1. Enable the peripheral clock for the uart 2 peripheral
	pRCC->APB1ENR |= ( 1 << 17);
	
	//2. Configure the GPIO pins for uart_TX and uart_RX functionality
	//PA2 as TX, PA3 as RX
	//2.1 Enable the clock for the GPIOA peripheral
	pRCC->AHB1ENR |= (1 << 0);
	
	//2.2 Change the mode of the PA2 to alternate function mode
	pGPIOA->MODER &= ~(0x3 << 4); //clear
	pGPIOA->MODER |= (0x2 << 4);  //set
	pGPIOA->AFR[0] &= ~(0xF << 8);
	pGPIOA->AFR[0] |= (0x7 << 8);
	
	//2.3 Enable or disable Pull-up resistor of the GPIO if required
	pGPIOA->PUPDR |= (0x1 << 4);
	
	//PA3 as RX
	//2.1 Enable the clock for the GPIOA peripheral (Don't needed is configure once)
	//2.2 Change the mode of the PA3 to alternate function mode
	pGPIOA->MODER &= ~(0x3 << 6); //clear
	pGPIOA->MODER |= (0x2 << 6);  //set
	pGPIOA->AFR[0] &= ~(0xF << 12);
	pGPIOA->AFR[0] |= (0x7 << 12);
	
	//2.3 Enable or disable Pull-up resistor if required
	pGPIOA->PUPDR |= (0x1 << 6);
	
	//3. Configure the baudrate
	pUART2->BRR = 0x8B;
	
	//4. Configure the data width, no of stop bits, etc..
	//Don't need configuration since default configuration is 8 bits, 1 stop bits etc..
	
	
	//5. Enable the TX engine of the uart peripheral
	pUART2->CR1 |= (0x1 << 3);
	
	//6. Enable the UART peripheral
	pUART2->CR1 |= (0x1 << 13);
	
}
void dma1_init(void)
{
	
	RCC_TypeDef *pRCC;
	pRCC = RCC;
	
	//1. Enable the peripheral clock for the dam1
	pRCC->AHB1ENR |= ( 1 << 21);
	
	//2. Identify the stream which is suitable for your peripheral
	
	//3. Identify the channel number on which UART2 peripheral send
	
	//4. Program the source address
	
	//5. Program the destination address
	
	//6. Program number of data items to send
	
	//7. Direction of data transfer. m2p, p2m m2m
	
	//8. Program the source and destination data width
	
	//9. Direct mode or fifo mode
	
	//10. Select the fifo threshold
	
	//11. Circular mode if required
	
	//12. Single Transfer or burst transfer
	
	//13. Configure the stream priority 
	
	//14. Enable the stream
	
}
