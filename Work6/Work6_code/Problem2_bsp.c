#include "bsp.h"
#include <stdlib.h>
#include <stdio.h>

void Setup_Pins(void){
	RCC->IOPENR |= (3U << 0);	//Enable GPIOA and GPIOB ports

	/*Setup SSD*/
	GPIOA->MODER &= ~(3U << 0);	//PA0 for Decimal Point in SSD
	GPIOA->MODER |= (1U << 0);

	GPIOA->MODER &= ~(3U << 2*1);	//PA1 for A in SSD
	GPIOA->MODER |= (1U << 2*1);

	GPIOA->MODER &= ~(3U << 2*4);	//PA4 for B in SSD
	GPIOA->MODER |= (1U << 2*4);

	GPIOA->MODER &= ~(3U << 2*5);	//PA5 for C in SSD
	GPIOA->MODER |= (1U << 2*5);

	GPIOA->MODER &= ~(3U << 2*12);	//PA12 for D in SSD
	GPIOA->MODER |= (1U << 2*12);

	GPIOA->MODER &= ~(3U << 2*11);	//PA11 for E in SSD
	GPIOA->MODER |= (1U << 2*11);

	GPIOA->MODER &= ~(3U << 2*8);	//PA8 for F in SSD
	GPIOA->MODER |= (1U << 2*8);

	GPIOA->MODER &= ~(3U << 2*7);	//PA7 for G in SSD
	GPIOA->MODER |= (1U << 2*7);

	GPIOB->MODER &= ~(3U << 2*4);	//PB4 for D1 in SSD
	GPIOB->MODER |= (1U << 2*4);

	GPIOB->MODER &= ~(3U << 2*5);	//PB5 for D2 in SSD
	GPIOB->MODER |= (1U << 2*5);

	GPIOB->MODER &= ~(3U << 2*9);	//PB9 for D3 in SSD
	GPIOB->MODER |= (1U << 2*9);

	GPIOB->MODER &= ~(3U << 2*8);	//PB8 for D4 in SSD
	GPIOB->MODER |= (1U << 2*8);

	/*Setup Membrane 4x4 Keypad*/
	GPIOA->MODER &= ~(3U << 2*10);	//PA10 for R1 in 4x4 Keypad
	GPIOA->MODER |= (1U << 2*10);

	GPIOA->MODER &= ~(3U << 2*9);	//PA9 for R2 in 4x4 Keypad
	GPIOA->MODER |= (1U << 2*9);

	GPIOB->MODER &= ~(3U << 0);		//PB0 for R3 in 4x4 Keypad
	GPIOB->MODER |= (1U << 0);

	GPIOB->MODER &= ~(3U << 2*2);	//PB2 for R4 in 4x4 Keypad
	GPIOB->MODER |= (1U << 2*2);

	GPIOB->MODER &= ~(3U << 2*6);	//PB6 for C1 in 4x4 Keypad
	GPIOB->PUPDR |= (2U << 2*6);	//Pull-down mode

	GPIOB->MODER &= ~(3U << 2*7);	//PB7 for C2 in 4x4 Keypad
	GPIOB->PUPDR |= (2U << 2*7);	//Pull-down mode

	GPIOA->MODER &= ~(3U << 2*15);	//PA15 for C3 in 4x4 Keypad
	GPIOA->PUPDR |= (2U << 2*15);	//Pull-down mode

	GPIOB->MODER &= ~(3U << 2*1);	//PB1 for C4 in 4x4 Keypad
	GPIOB->PUPDR |= (2U << 2*1);	//Pull-down mode

	/*Setup interrupts for inputs*/
	EXTI->EXTICR[1] |= (1U << 8*2);	//PB6
	EXTI->EXTICR[1] |= (1U << 8*3);	//PB7
	EXTI->EXTICR[3] |= (0U << 8*3);	//PA15
	EXTI->EXTICR[0] |= (1U << 8*1);	//PB1

	/*Rising Edge*/
	EXTI->RTSR1 |= (1U << 6);	//6th pin
	EXTI->RTSR1 |= (1U << 7);	//7th pin
	EXTI->RTSR1 |= (1U << 15);	//15th pin
	EXTI->RTSR1 |= (1U << 1);	//1st pin
	/*Mask*/
	EXTI->IMR1 |= (1U << 6);	//6th pin
	EXTI->IMR1 |= (1U << 7);	//7th pin
	EXTI->IMR1 |= (1U << 15);	//15th pin
	EXTI->IMR1 |= (1U << 1);	//1st pin
	/*NVIC*/
	NVIC_SetPriority(EXTI0_1_IRQn, 0);
	NVIC_EnableIRQ(EXTI0_1_IRQn);

	NVIC_SetPriority(EXTI4_15_IRQn, 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);

	/*Set all rows for 5V*/
	GPIOA->ODR |= (1U << 10);	//PA10
	GPIOA->ODR |= (1U << 9);	//PA9
	GPIOB->ODR |= (1U << 0);	//PB0
	GPIOB->ODR |= (1U << 2);	//PB2

}

static volatile uint32_t tick;

void SysTick_Handler(){
	if (tick > 0)
		--tick;
}

void delay_ms(uint32_t s){
	tick = s;
	while(tick)
		SysTick_Handler();
}

void USART2_IRQHandler(void){
	uint8_t data = (uint8_t)(USART2->RDR);
	//RXNE is automatically cleared when read
	printChar(data);
}

void printChar(uint8_t c){
	USART2->TDR = (uint16_t)c;
	while (!(USART2->ISR & (1 << 6)));
}

int _write(int fd, char* ptr, int len){
	(void)fd;
	for (int i = 0; i <= len; ++i){
		printChar(ptr[i]);
	}
	return len;
}

void print(char* buf){
	int len = 0;
	while(buf[len++] != '\0');
	_write(0, buf, len);
}
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
	USART2->CR1 |= (1 << 5); //RXNEIE for interrupt

	USART2->BRR = (uint16_t)(SystemCoreClock / baud);

	USART2->CR1 |= (1 << 0); //USART enable

	NVIC_SetPriority(USART2_IRQn, 1);
	NVIC_EnableIRQ(USART2_IRQn);
}

