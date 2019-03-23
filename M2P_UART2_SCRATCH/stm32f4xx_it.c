#include "stm32f446xx.h"

void clear_exti_pending_bit(void)
{
	EXTI_TypeDef *pEXTI;
	pEXTI = EXTI;
	
	if(EXTI->PR & (1<<13))
	{
		// Clears pending bits
		EXTI->PR |= (1<<13);
	}
}


void EXTI15_10_IRQHandler(void)
{
	USART_TypeDef *pUART2;
	pUART2 = USART2;
	
	//here we will send uart2_TX_DMA request to DMA1 Controller
	pUART2->CR3 |= (1 << 7);
	
	clear_exti_pending_bit();
}

// IRQ handler for DMA1 stream6 global interrupt
void DMA1_Stream6_IRQHandler(void)
{
	
}
