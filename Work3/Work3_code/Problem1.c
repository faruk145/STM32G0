/*
 * Author: Omer Faruk Aydin
 * 171024002 Problem1
*/
#include "stm32g0xx.h"
#include <stdio.h>
#include <stdlib.h>


int main(void);
void delay(volatile uint32_t);
void button(int);

extern unsigned long _estack;
int number = 0;
int a;
int b;
int x;
volatile uint32_t LEDDELAY;

void HardFault_Handler(void) {
delay(LEDDELAY);
GPIOC -> ODR &= (0U << 6);
delay(LEDDELAY);
__asm ("ldr r0, = _estack");
__asm ("movs sp, r0");
NVIC_SystemReset();
__asm ("b Reset_Handler");

}

void Reset_Handler(void) {
__asm ("ldr r0, = _estack");
__asm ("movs sp, r0");
main();
for(;;);
}


int main(void){
RCC -> IOPENR |= (1U << 2); //Enable GPIOC
RCC -> IOPENR |= (1U << 1); //Enable GPIOB

//Setup PC6 for LED
GPIOC -> MODER &= ~(3U << 2*6); // Gerekli bitler temizlendi
GPIOC -> MODER |= (1U << 2*6); // Gerekli pin aktif edildi output olduğu için "01"

//Setup PB3 for Button
GPIOB -> MODER &= ~(3U << 2*3); // Gerekli bitler temizlendi input olduğu için "00"
while (1) {
	++number;
	x = 2 * number + 5;
	button(x);

}

return 0;
}

void delay(volatile uint32_t s) {
for(; s>0; s--);
}

void fault1(void){
	__asm ("ldr r6, =0x80000001");
	__asm ("ldr r5, [r6]");
}

void fault2(void){
	__asm ("ldr r6, =0x81000000");
	__asm ("ldr r5, [r6]");
}

void fault3(void){
	b = *(int*)0xFEEDABBA;
}
void button(int i){
	a = i % 3;
	if (a == 0){
		if((GPIOB -> IDR >> 3) & 1){
			fault1();
		}
		else{
		LEDDELAY = 700000;
		GPIOC -> ODR |= (1U << 6);
		}
	}
	else if (a == 1 || a == -1){
		if((GPIOB -> IDR >> 3) & 1){
			fault2();
		}
		else{
			LEDDELAY = 100000;
			GPIOC -> ODR |= (1U << 6);
		}
	}
	else{
		if((GPIOB -> IDR >> 3) & 1){
			fault3();
		}
		else{
			LEDDELAY = 900000;
			GPIOC -> ODR |= (1U << 6);
		}
	}
}
