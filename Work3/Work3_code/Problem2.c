/*
 * 
 *
 * author: Omer Faruk Aydin 171024002 Problem2
 *
 */

#include "stm32g0xx.h"
#include <stdio.h>
#include <stdlib.h>

#define LEDDELAY    1600000

int b = 0;
int a;

void HardFault_Handler(void);
void delay(volatile uint32_t);
void fault1(void);
void fault2(void);
void fault3(void);

int main(void) {

	/* Enable GPIOC and GPIOB clock */
	RCC->IOPENR |= (3U << 1);

	/* Setup PC6 as output */
	GPIOC->MODER &= ~(3U << 2 * 6);
	GPIOC->MODER |= (1U << 2 * 6);

	/* Setup PB3 as input for button */
	GPIOB->MODER &= ~(3U << 2 * 3);

	/* Setup PB3 as interrupt button */
	EXTI -> RTSR1 |= (1U << 3);
	EXTI -> EXTICR[0] |= (1U << 8*3);
	EXTI -> IMR1 |= (1U << 3);

	NVIC_SetPriority(EXTI2_3_IRQn, 0);
	NVIC_EnableIRQ(EXTI2_3_IRQn);

	while (1) {

	}

	return 0;
}

void HardFault_Handler(void) {
	__asm("ldr r0, =_estack"); /* Set stack pointer */
	__asm("mov sp, r0");

	/* Toggle LED */
	delay(LEDDELAY);
    GPIOC->ODR ^= (1U << 6);
    delay(LEDDELAY);

	NVIC_SystemReset();  /* for resume after HardFaultHandling many times */

	__asm("b Reset_Handler");
}

void EXTI2_3_IRQHandler(void) {

	a =rand()%3;

	if (a == 0) {
		fault1();
	} else if (a == 1) {
		fault2();
	} else if (a == 2) {
		fault3();
	}

	EXTI -> RPR1 |= (1U << 3); // Pending
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}

void fault1(void) {
	__asm("ldr r0, =0x80000001");
	__asm("ldr r1, [r0]");
}

void fault2(void) {
	__asm("ldr r0, =0x81000000");
	__asm("ldr r1, [r0]");
}

void fault3(void) {
	b = *(int*)0xFEEDABBA;
}

