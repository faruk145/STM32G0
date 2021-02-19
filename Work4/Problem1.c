/*
 * Problem1.c
 *
 * author: Ömer Faruk Aydın
 *
 */
#include "stm32g0xx.h"
#include <stdlib.h>

int main(void);


void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
    GPIOA -> ODR ^= (1U << 6);
    TIM1 -> SR &= ~(1U << 0);
    //Update interrupt flag. This bit is set by hardware on an
    //update event. It is cleared by software.
    //0: No update occurred.1: Update interrupt pending.
}



void timer1(unsigned int x, unsigned int y){
    RCC -> APBENR2 |= (1U << 11); //TIM1 timer clock enable
    TIM1 ->CR1 = 0;
    TIM1 ->CR1 |= (1 << 7); //TIMx_ARR register is bufferedBits
    TIM1 ->CNT = 0;

    TIM1 ->PSC = x; // x==999
    //The counter clock frequency (CK_CNT) is equal to
    //fCK_PSC / (PSC[15:0] + 1).PSC contains the value to be
    //loaded in the active prescaler register at each update event .

    TIM1 ->ARR = y; // y==1600 -----> 1 seconds interrupt
    //ARR is the value to be loaded in the actual auto-reload register.

    TIM1 ->DIER |= (1 << 0); //Update interrupt disabled
    TIM1 ->CR1 |= (1 << 0);

    NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 3);
    NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}

void delay_ms(volatile uint32_t s) {
    for(; s>0; s--);
}


int main(void){
    RCC -> IOPENR |= (1U << 0); //Enable GPIOA

    //Setup PA6 for LED
    GPIOA -> MODER &= ~(3U << 2*6); // Necessary bits cleared
    GPIOA -> MODER |= (1U << 2*6); // Required pin is activated output"01

    //timer1(999,1600);	//1ms interval with systick
        while(1){
                GPIOA -> ODR |= (1U << 6);	//Turn on the LED
                delay_ms(160000);		//1ms delay
                GPIOA -> BRR |= (1U << 6);	//Turn off the LED
                delay_ms(160000);		//1ms delay
        }
    return 0;
}

