/*
author: Ömer Faruk Aydın
Problem1
171024002
*/
#include "bsp.h"
/*
 * PA2 PA3 are connected to the Virtual COM port
 * USART2 module
 * */
void BSP_UART_init(uint32_t baud){
	RCC->IOPENR |= (1U << 0);
	RCC->APBENR1 |= (1U << 17); //or RCC->APBENR1 |= (1U << RCC_APBENR1_USART2EN);

	/*Setup PA2 as AF1*/
	GPIOA->MODER &= ~(3U << 2*2);
	GPIOA->MODER |= (2U << 2*2);
	/*Choose AF1 from mux*/
	GPIOA->AFR[0] &= ~(0xFU << 4*2);
	GPIOA->AFR[0] |= (1U << 4*2);

	/*Setup PA3 as AF1*/
	GPIOA->MODER &= ~(3U << 2*3);
	GPIOA->MODER |= (2U << 2*3);
	/*Choose AF1 from mux*/
	GPIOA->AFR[0] &= ~(0xFU << 4*3);
	GPIOA->AFR[0] |= (1U << 4*3);

	//setup uart2
	//reset uart cr1
	USART2->CR1 = 0;
	USART2->CR1 |= (1 << 3); //Transmit enable
	USART2->CR1 |= (1 << 2); //Receiver enable

	USART2->BRR = (uint16_t)(SystemCoreClock / baud);

	USART2->CR1 |= (1 << 0); //USART enable

}
void printChar(uint8_t c){
	USART2->TDR = (uint16_t)c;
	while (!(USART2->ISR & (1 << 6)));
}

int _print(int f, char *ptr, int len)
{
	(void)f;
	for (int i = 0 ; i < len; ++i){
		printChar(*ptr++);
	}
return len;
}

void print(char *s) {
	int counter;
	for (counter = 0; s[counter]!= '\0'; ++counter);

	_print(0, s, counter);
}


int main(void) {

    //BSP_system_init();
    BSP_UART_init(9600);
    print("HelloBro\n");
    for(;;){

    }

    return 0;
}
