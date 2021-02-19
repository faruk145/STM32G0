/*
 * Problem2
 *
 * author: Ömer Faruk Aydın 171024002
 *
 */
#include "stm32g0xx.h"
#include <stdlib.h>

int main(void);
void delay(volatile uint32_t);

void TIM1_BRK_UP_TRG_COM_IRQHandler(void){

    GPIOA -> ODR ^= (1U << 6);
    TIM1 -> SR &= ~(1U << 0);
}

void timer1(unsigned int x, unsigned int y){
    RCC -> APBENR2 |= (1U << 11);

    TIM1 ->CR1 = 0;
    TIM1 ->CR1 |= (1U << 7);
    TIM1 ->CNT = 0;

    TIM1 ->PSC = x; // x==999
    TIM1 ->ARR = y; // y==1600 -----> 1 seconds interrupt

    TIM1 ->DIER |= (1U << 0);
    TIM1 ->CR1 |= (1U << 0);

    NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 1);
    NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);

}

void short_delay(int x){
	for (int i = 0; i < x; ++i);
}

int main(void){

    unsigned int a;
    unsigned int b;
    static int button_press_counter;

    SystemCoreClockUpdate();

    RCC -> IOPENR |= (3U << 0); //Enable GPIOA, GPIOB
    //Setup PA6 for LED
    GPIOA -> MODER &= ~(3U << 2*6); // Necessary bits cleared
    GPIOA -> MODER |= (1U << 2*6); // Required pin is activated output"01
    //Setup PB3 for Button
    GPIOB -> MODER &= ~(3U << 2*3); // Necessary bits cleared input "00"
    GPIOB -> PUPDR |= (2U << 2*3);
    while(1){

        if((((GPIOB -> IDR >> 3) & 1) && button_press_counter % 10 == 0)){
            a=9999;
            b = 1600;	//1 second interval
            timer1(a,b);
            short_delay(3000000);
            ++button_press_counter;
        }
        if((((GPIOB -> IDR >> 3) & 1) && button_press_counter % 10 == 1)){
        	a=9999;
        	b = 3200;	//2 seconds interval
        	timer1(a,b);
        	short_delay(3000000);
        	++button_press_counter;
        }
        if((((GPIOB -> IDR >> 3) & 1) && button_press_counter % 10 == 2)){
        	a=9999;
        	b = 4800;	//3 seconds interval
        	timer1(a,b);
        	short_delay(3000000);
        	++button_press_counter;
        }

        if((((GPIOB -> IDR >> 3) & 1) && button_press_counter % 10  == 3)){
        	a=9999;
        	b = 6400;	//4 seconds interval
        	timer1(a,b);
        	short_delay(3000000);
        	++button_press_counter;
                }
        if((((GPIOB -> IDR >> 3) & 1) && button_press_counter % 10 == 4)){
        	a=9999;
        	b = 8000;	//5 seconds interval
        	timer1(a,b);
        	short_delay(3000000);
        	++button_press_counter;
          }

        if((((GPIOB -> IDR >> 3) & 1) && button_press_counter % 10 == 5)){
        	a=9999;
        	b = 9600;	//6 seconds interval
        	timer1(a,b);
        	short_delay(3000000);
        	++button_press_counter;
         }
        if((((GPIOB -> IDR >> 3) & 1) && button_press_counter % 10 == 6)){
        	a=9999;
        	b = 11200;	//7 seconds interval
        	timer1(a,b);
        	short_delay(3000000);
        	++button_press_counter;
        }
        if((((GPIOB -> IDR >> 3) & 1) && button_press_counter % 10 == 7)){
        	a=9999;
        	b = 12800;	//8 seconds interval
        	timer1(a,b);
        	short_delay(3000000);
        	++button_press_counter;
        }
        if((((GPIOB -> IDR >> 3) & 1) && button_press_counter % 10 == 8)){
        	a=9999;
        	b = 14400;	//9 seconds interval
        	timer1(a,b);
        	short_delay(3000000);
        	++button_press_counter;
        }
        if((((GPIOB -> IDR >> 3) & 1) && button_press_counter % 10 == 9)){
        	a=9999;
        	b = 16000;	//10 seconds interval
        	timer1(a,b);
        	short_delay(3000000);
        	++button_press_counter;
        }

    }
    return 0;
}
