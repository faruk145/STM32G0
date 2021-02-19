/*
 * Problem5
 *
 * author: Ömer Faruk Aydın 171024002
 *
 */
#include "stm32g0xx.h"
#include <stdlib.h>

int main(void);


void short_delay(int x){
	for (int i = 0; i < x; ++i);
}

void clear_leds(void){
	GPIOA -> ODR |= (0x18F2U);
}
void clear_digit1(void){
	GPIOB -> ODR &= (0U << 4);
}

void clear_digit2(void){
	GPIOB -> ODR &= (0U << 5);
}


void clear_digit3(void){
	GPIOB -> ODR &= (0U << 9);
}


void clear_digit4(void){
	GPIOB -> ODR &= (0U << 8);
}


void light0(void){
	clear_leds();
	GPIOA -> ODR &= ~(0x18F0U);
}

void light1(void){
	clear_leds();
	GPIOA -> ODR &= ~(0x0810U);
}

void light2(void){
	clear_leds();
	GPIOA -> ODR &= ~(0x18A2U);
}

void light3(void){
	clear_leds();
	GPIOA -> ODR &= ~(0x08B2U);
}

void light4(void){
	clear_leds();
	GPIOA -> ODR &= ~(0x0852U);
}

void light5(void){
	clear_leds();
	GPIOA -> ODR &= ~(0x00F2U);
}

void light6(void){
	clear_leds();
	GPIOA -> ODR &= ~(0x10F2U);
}

void light7(void){
	clear_leds();
	GPIOA -> ODR &= ~(0x0890U);
}

void light8(void){
	clear_leds();
	GPIOA -> ODR &= ~(0x18F2U);
}

void light9(void){
	clear_leds();
	GPIOA -> ODR &= ~(0x08F2U);
}

static int count;

int light_1st_digit(void){
	clear_digit4();
	GPIOB -> ODR |= (1U << 4);
	if ((count / 1000) % 10 == 0){
		light0();
	}
	else if ((count / 1000) % 10 == 1){
		light1();
	}
	else if ((count / 1000) % 10 == 2){
		light2();
	}
	else if ((count / 1000) % 10 == 3){
		light3();
		}
	else if ((count / 1000) % 10 == 4){
		light4();
		}
	else if ((count / 1000) % 10 == 5){
		light5();
		}
	else if ((count / 1000) % 10 == 6){
		light6();
		}
	else if ((count / 1000) % 10 == 7){
		light7();
		}
	else if ((count / 1000) % 10 == 8){
		light8();
		}
	else{
		light9();
		}
	return 1;
}

int light_2nd_digit(void){
	clear_digit1();
	GPIOB -> ODR |= (1U << 5);
	if ((count / 100) % 10 == 0){
		light0();
	}
	else if ((count / 100) % 10 == 1){
		light1();
	}
	else if ((count / 100) % 10 == 2){
		light2();
	}
	else if ((count / 100) % 10 == 3){
		light3();
		}
	else if ((count / 100) % 10 == 4){
		light4();
		}
	else if ((count / 100) % 10 == 5){
		light5();
		}
	else if ((count / 100) % 10 == 6){
		light6();
		}
	else if ((count / 100) % 10 == 7){
		light7();
		}
	else if ((count / 100) % 10 == 8){
		light8();
		}
	else{
		light9();
		}
	return 1;
}

int light_3rd_digit(void){
	clear_digit2();
	GPIOB -> ODR |= (1U << 9);
	if ((count / 10) % 10 == 0){
		light0();
	}
	else if ((count / 10) % 10 == 1){
		light1();
	}
	else if ((count / 10) % 10 == 2){
		light2();
	}
	else if ((count / 10) % 10 == 3){
		light3();
		}
	else if ((count / 10) % 10 == 4){
		light4();
		}
	else if ((count / 10) % 10 == 5){
		light5();
		}
	else if ((count / 10) % 10 == 6){
		light6();
		}
	else if ((count / 10) % 10 == 7){
		light7();
		}
	else if ((count / 10) % 10 == 8){
		light8();
		}
	else{
		light9();
		}
	return 1;
}

int light_4th_digit(void){
	clear_digit3();
	GPIOB -> ODR |= (1U << 8);
	if (count % 10 == 0){
		light0();
	}
	else if (count % 10 == 1){
		light1();
	}
	else if (count % 10 == 2){
		light2();
	}
	else if (count % 10 == 3){
		light3();
		}
	else if (count % 10 == 4){
		light4();
		}
	else if (count % 10 == 5){
		light5();
		}
	else if (count % 10 == 6){
		light6();
		}
	else if (count % 10 == 7){
		light7();
		}
	else if (count % 10 == 8){
		light8();
		}
	else{
		light9();
		}
	return 1;
}


void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
	start:
	count = 0;
	GPIOA -> ODR &= (0U << 10);
		while (1){
			light_1st_digit();
			short_delay(500);
			light_2nd_digit();
			short_delay(500);
			light_3rd_digit();
			short_delay(500);
			light_4th_digit();
			short_delay(500);
			if ((GPIOB -> IDR >> 6) & 1)
							goto label;
		}
	while(1){
		label:
			IWDG -> KR |=  0x0000AAAA;	//Feeding periodically so it doesn't generate a reset.
			++count;
			short_delay(5);
			light_1st_digit();
			short_delay(500);
			light_2nd_digit();
			short_delay(500);
			light_3rd_digit();
			short_delay(500);
			light_4th_digit();
			short_delay(500);

			if (count == 9999){
				for (int i = 0; i < 1000; ++i){
					light_1st_digit();
					short_delay(500);
					light_2nd_digit();
					short_delay(500);
					light_3rd_digit();
					short_delay(500);
					light_4th_digit();
					short_delay(500);
					GPIOA -> ODR = (1U << 10);
				}
				short_delay(10000);
				count = 0;
				goto start;
			}
	}


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

    IWDG -> KR |= 0x0000CCCC;	//Enabling Independent Watchdog
    IWDG -> KR |=  0x0000AAAA;	//Feeding periodically

    NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 1);
    NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);

}
int main(void){


    SystemCoreClockUpdate();

    RCC -> IOPENR |= (3U << 0); //Enable GPIOA, GPIOB

    GPIOA -> MODER &= ~(3U << 2); // Necessary bits cleared for PA1
    GPIOA -> MODER |= (1U << 2); // Required pin is activated output"01 for PA1

    GPIOA -> MODER &= ~(15U << 8); // Necessary bits cleared for PA4-PA5
    GPIOA -> MODER |= (5U << 8); // Required pin is activated output"01 for PA4-PA5

    GPIOA -> MODER &= ~(15U << 12); // Necessary bits cleared for PA6-PA7
    GPIOA -> MODER |= (5U << 12); // Required pin is activated output"01 for PA6-PA7

    GPIOA -> MODER &= ~(15U << 22); // Necessary bits cleared for PA11-PA12
    GPIOA -> MODER |= (5U << 22); // Required pin is activated output"01 for PA11-PA12

    GPIOB -> MODER &= ~(15U << 8); // Necessary bits cleared for PB4-PB5
    GPIOB -> MODER |= (5U << 8); // Required pin is activated output"01 for PB4-PB5

    GPIOB -> MODER &= ~(15U << 16); // Necessary bits cleared for PB8-PB9
    GPIOB -> MODER |= (5U << 16); // Required pin is activated output"01 for PB8-PB9

    //Setup PB6 for button
    GPIOB -> MODER &= ~(3U << 12);
	GPIOB -> MODER |= (1U << 12);

    //Setup PA10 for External LED
	GPIOA -> MODER &= ~(3U << 20); // Necessary bits cleared for PA10
	GPIOA -> MODER |= (1U << 20);


    timer1(999, 1600);
    while(1){

    }
    return 0;
}
