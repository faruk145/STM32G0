
/*
Problem 3
Ömer Faruk Aydın 171024002
 */
#include "stm32g0xx.h"

uint32_t ten_thousands,thousands,hundreds,tens;
uint32_t  number;
uint32_t c=0 ;
uint32_t flag_for_tens_or_units=1;
uint32_t uart_display_counter=0;
uint32_t duty_cycle =0;
uint8_t unit_digit =0;
uint8_t tens_digit =0;

void SystemInitial();
uint8_t ConvertNumberToAsci(uint8_t);
void InitTimer();
void UartInit(uint32_t);
void LedInit(void);
void delay(volatile uint32_t);
void KeypadInitForOutput(void);
void KeypadInitForInput(void);
void SetKeypad(void);
void ResetKeypad(void);
void PrintChar(uint8_t);

volatile int interrupt_check=0;




int main(void) {

	SystemInitial();

	SetKeypad();
    while(1) {


    }

    return 0;
}



void SystemInitial(){


    LedInit();
    KeypadInitForOutput();
    SetKeypad();
    ResetKeypad();
    KeypadInitForInput();
	InitTimer();
	UartInit(9600);
}

void InitTimer(){

		RCC->APBENR1 |=(1U<<1);
		TIM3->CR1=0;
		TIM3->CR1 |=(1<<7);
		TIM3->CNT =0;
		TIM3->PSC=10;
		TIM3->ARR=(16000);
		TIM3->DIER |=(1<<0);
		TIM3->CCMR1 |=(1<<3);

		TIM3->CCMR1 &= ~ (1u<<16); //0
		TIM3->CCMR1 |=(1u<<6); //1
		TIM3->CCMR1 |= (1u<<5); //1
		TIM3->CCMR1 &= ~(1u<<4);  // 0
		TIM3->CCER |=(1<<0);
		TIM3->CCR1 =0x0;


		TIM3->EGR |= (1<<0);
		TIM3->CR1 |= (1<<0);

		NVIC_SetPriority(TIM3_IRQn,3);
		NVIC_EnableIRQ(TIM3_IRQn);

}
uint8_t ConvertNumberToAsci(uint8_t a){
	if(a==0)
		return 48 ; // ASCII equivalent of zero

	else if(a==1) // ASCII equivalent of one
		return 49;
	else if(a==2)// ASCII equivalent of two
		return 50;
	else if(a==3) // ASCII equivalent of three
		return 51;
	else if (a==4)// ASCII equivalent of four
		return 52;
	else if(a==5)// ASCII equivalent of five
		return 53;
	else if (a==6)// ASCII equivalent of six
		return 54;
	else if(a==7)// ASCII equivalent of seven
		return 55;
	else if(a==8)// ASCII equivalent of eight
		return 56;
	else
		return 57; // // ASCII equivalent of nine
}
void TIM3_IRQHandler(){
	uart_display_counter=uart_display_counter+10; // To write to the console every two seconds
	// adjusted for a 5-digit number since the highest value is 16000

	if(uart_display_counter==2000){
				delay(2000);
				number=TIM3->CCR1;


				ten_thousands=(number)/10000; //Find the tens of thousands digits
				PrintChar(ConvertNumberToAsci(ten_thousands)); //press the console


				number -= (ten_thousands * 10000); //Find the remaining digits by subtracting the found digit from the number
				thousands=number/1000; 		//Find the thousands digits
				PrintChar(ConvertNumberToAsci(thousands)); //press the console



				number -= (thousands * 1000); //Find the remaining digits by subtracting the found digit from the number
				hundreds=(number /100);			//Find the hundreds digits
				PrintChar(ConvertNumberToAsci(hundreds)); //press the console


				number -= (hundreds * 100);  //Find the remaining digits by subtracting the found digit from the number
				tens=(number/10);			//Find the tens digits
				PrintChar(ConvertNumberToAsci(tens)); //press the console

				number -= (tens * 10);   //when all numbers are removed, the remaining digit is the units digit press the number directly to the console.
				PrintChar(ConvertNumberToAsci(number)); //press the console
				uart_display_counter=0;
	}



	TIM3->SR &=  ~(1U<<0);
	TIM3->SR &=  ~(1U<<1);
}



void LedInit(){
	 /* Enable GPIOB and GPIOA clock */
	    RCC->IOPENR |= (3U << 0);
	 /* Setup PA6 as alternate function */

		GPIOA->MODER &= ~(3U << 2*6);
		GPIOA->MODER |=  (2<< 2*6) ;

		GPIOA->AFR[0]  &= ~(0xFU<<4*6);
		GPIOA->AFR[0]  |=  (1<<4*6);

}
void EXTI0_1_IRQHandler (){ //c1
	ResetKeypad();

	GPIOA->ODR |=(1<<0);
	if((GPIOB->IDR>>1)&1){
		if(flag_for_tens_or_units==1){ // 	KEYPAD button control for number 1 - 1st column
			tens_digit=1;
			flag_for_tens_or_units=0;
		}
		else{
			unit_digit=1;
			flag_for_tens_or_units=1;
		}

	}
	else{						// 	KEYPAD button control for number 4
		GPIOA->ODR &=  ~(1U <<0);
		GPIOA->ODR |=(1<<1);
		if((GPIOB->IDR>>1)&1){

			if(flag_for_tens_or_units==1){
						tens_digit=4;
						flag_for_tens_or_units=0;
					}
					else{
						unit_digit=4;
						flag_for_tens_or_units=1;
					}

				}

		else {						// 	KEYPAD button control for number 7
			GPIOA->ODR &=  ~(1U <<1);
			GPIOA->ODR |=(1<<4);
			if((GPIOB->IDR>>1)&1){
				if(flag_for_tens_or_units==1){
										tens_digit=7;
										flag_for_tens_or_units=0;
									}
									else{
										unit_digit=7;
										flag_for_tens_or_units=1;
									}


			}
			else{
				GPIOA->ODR &=  ~(1U <<4);
				GPIOA->ODR |=(1<<5);
				if((GPIOB->IDR>>1)&1){

				}

			}

		}

	}
	SetKeypad();
	EXTI->RPR1 |=(1<<1);
}
void EXTI4_15_IRQHandler (){
	ResetKeypad();

	GPIOA->ODR |=(1<<0);
	if((GPIOB->IDR>>4)&1){
		TIM3->CCR1 =100;
	}
	else{
		GPIOA->ODR &=  ~(1U <<0);
		GPIOA->ODR |=(1<<1);
		if((GPIOB->IDR>>4)&1){
			TIM3->CCR1 =100;
		}
		else {
			GPIOA->ODR &=  ~(1U <<1);
			GPIOA->ODR |=(1<<4);
			if((GPIOB->IDR>>4)&1){
				TIM3->CCR1 =100;
			}
			else{
				GPIOA->ODR &=  ~(1U <<4);
				GPIOA->ODR |=(1<<5);
				if((GPIOB->IDR>>4)&1){
					TIM3->CCR1 =100;
				}

			}

		}

	}

	SetKeypad();
	EXTI->RPR1 |=(1<<4);
}
void EXTI2_3_IRQHandler (){
	ResetKeypad();
	if((EXTI->RPR1>>2)&1){ // 	KEYPAD button control for number 2 - 2nd column
		GPIOA->ODR |=(1<<0);
		if((GPIOB->IDR>>2)&1){
			if(flag_for_tens_or_units==1){
						tens_digit=2;
						flag_for_tens_or_units=0;
					}
					else{
						unit_digit=2;
						flag_for_tens_or_units=1;
					}

				}

		else{
			GPIOA->ODR &=  ~(1U <<0); // 	KEYPAD button control for number 5
			GPIOA->ODR |=(1<<1);
			if((GPIOB->IDR>>2)&1){
				if(flag_for_tens_or_units==1){
							tens_digit=5;
							flag_for_tens_or_units=0;
						}
						else{
							unit_digit=5;
							flag_for_tens_or_units=1;
						}

					}

			else {					// 	KEYPAD button control for number 8
				GPIOA->ODR &=  ~(1U <<1);
				GPIOA->ODR |=(1<<4);
				if((GPIOB->IDR>>2)&1){
					if(flag_for_tens_or_units==1){
								tens_digit=8;
								flag_for_tens_or_units=0;
							}
							else{
								unit_digit=8;
								flag_for_tens_or_units=1;
							}

						}

				else{				// 	KEYPAD button control for number 0
					GPIOA->ODR &=  ~(1U <<4);
					GPIOA->ODR |=(1<<5);
					if((GPIOB->IDR>>2)&1){
						if(flag_for_tens_or_units==1){
									tens_digit=0;
									flag_for_tens_or_units=0;
								}
								else{
									unit_digit=0;
									flag_for_tens_or_units=1;
								}


					}

				}

			}

		}
		EXTI->RPR1 |=(1<<2);
	}
	else{						// 	KEYPAD button control for number 3 - 3rd column
		GPIOA->ODR |=(1<<0);
		if((GPIOB->IDR>>3)&1){
			if(flag_for_tens_or_units==1){
						tens_digit=3;
						flag_for_tens_or_units=0;
					}
					else{
						unit_digit=3;
						flag_for_tens_or_units=1;
					}


		}
		else{								// 	KEYPAD button control for number 6
				GPIOA->ODR &=  ~(1U <<0);
				GPIOA->ODR |=(1<<1);
				if((GPIOB->IDR>>3)&1){
					if(flag_for_tens_or_units==1){
								tens_digit=6;
								flag_for_tens_or_units=0;
							}
							else{
								unit_digit=6;
								flag_for_tens_or_units=1;
							}

						}

				else {						// 	KEYPAD button control for number 9
						GPIOA->ODR &=  ~(1U <<1);
						GPIOA->ODR |=(1<<4);
						if((GPIOB->IDR>>3)&1){
							if(flag_for_tens_or_units==1){
										tens_digit=9;
										flag_for_tens_or_units=0;
									}
									else{
										unit_digit=9;
										flag_for_tens_or_units=1;
									}


						}
						else{			// 	KEYPAD button control for  #
								GPIOA->ODR &=  ~(1U <<4);
								GPIOA->ODR |=(1<<5);
						if((GPIOB->IDR>>3)&1){
							duty_cycle=(16000*((tens_digit*10)+unit_digit))/100;
							TIM3->CCR1=duty_cycle;
							}

						}

					}

		}

		EXTI->RPR1 |=(1<<3);
	}
	SetKeypad();// all output set AS  5V
}
void KeypadInitForOutput(){

	 GPIOA->MODER &=  ~(3U<<2*0); //PA0 as output  R1
	 GPIOA->MODER |=  (1U<<0);


	 GPIOA->MODER &=  ~(3U <<2*1); //PA1  as output R2
	 GPIOA->MODER |=  (1 << 2);

	 GPIOA->MODER &=  ~(3U <<2*4); //PA4  as output R3
	 GPIOA->MODER |=  (1 << 8);

	 GPIOA->MODER &=  ~(3U <<2*5); //PA5  as output R4
	 GPIOA->MODER |=  (1 << 10);
}
void SetKeypad(){
	GPIOA->ODR |= (1<<0);  //R1 set
	GPIOA->ODR |= (1<<1);  //R2 set
	GPIOA->ODR |= (1<<4);  //R3 set
	GPIOA->ODR |= (1<<5);  //R4 set

}
void ResetKeypad(){
	GPIOA->ODR &=  ~(1U <<0); //R1 reset
	GPIOA->ODR &=  ~(1U <<1); //R2 reset
	GPIOA->ODR &=  ~(1U <<4); //R3 reset
	GPIOA->ODR &=  ~(1U <<5);  //R4 reset
}

void KeypadInitForInput(){

		GPIOB->MODER &= ~(3U << 2*1);// PB1 as input C1
		GPIOB->PUPDR |= (2U << 2*1); //Use of Pull ups to prevent bouncing

		GPIOB->MODER &= ~(3U << 2*2);// PB2 as input C2
		GPIOB->PUPDR |= (2U << 2*2); //Use of Pull ups to prevent bouncing

		GPIOB->MODER &= ~(3U << 2*3); // PB3 as input C3
		GPIOB->PUPDR |= (2U << 2*3); //Use of Pull ups to prevent bouncing

		GPIOB->MODER &= ~(3U << 2*4); // PB4 as input C4
		GPIOB->PUPDR |= (2U << 2*4); //Use of Pull ups to prevent bouncing


	    EXTI->RTSR1 |=(1U<<1); // PB1 as interrupt
	    EXTI->EXTICR[0] |= (1U<<8*1);
	    EXTI->IMR1 |=(1<<1);
	    NVIC_SetPriority(EXTI0_1_IRQn,1);
	    NVIC_EnableIRQ(EXTI0_1_IRQn);


	    EXTI->RTSR1 |=(1U<<2); // PB2 as interrupt
	    EXTI->EXTICR[0] |= (1U<<8*2);
 	    EXTI->IMR1 |=(1<<2);


	    EXTI->RTSR1 |=(1U<<3);// PB3 as interrupt
	    EXTI->EXTICR[0] |= (1U<<8*3);
	    EXTI->IMR1 |=(1<<3);
	    NVIC_SetPriority(EXTI2_3_IRQn,0);
	    NVIC_EnableIRQ(EXTI2_3_IRQn);


	    EXTI->RTSR1 |=(1U<<4);		// PB4 as interrupt
	    EXTI->EXTICR[1] |= (1U<<8*0);
	    EXTI->IMR1 |=(1<<4);
	    NVIC_SetPriority(EXTI4_15_IRQn,2);
	    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}
void UartInit(uint32_t baud){
	//Enable GPIOA
	RCC->IOPENR |= (1U << 0);
	RCC->APBENR1 |= (1U << 17);

	//Setup PA3 as Alternate Function (AF1)
	GPIOA-> MODER &= ~(3U << 2*2);
	GPIOA-> MODER |=  (2U << 2*2);

	// Chose AF1
	GPIOA-> AFR[0] &= ~(0xFU << 4*2);
	GPIOA-> AFR[0] |=  (1U << 4*2);

	//Setup PA3 as Alternate Function (AF1)
	GPIOA-> MODER &= ~(3U << 2*3);
	GPIOA-> MODER |=  (2U << 2*3);

	// Chose AF1
	GPIOA-> AFR[0] &= ~(0xFU << 4*3);
	GPIOA-> AFR[0] |=  (1U << 4*3);

	USART2 -> CR1 = 0;
	USART2 -> CR1 |= (1U << 3); // Transmitter
	USART2 -> CR1 |= (1U << 2); // Receiver

	USART2 -> CR1 |= (1U << 5);

	USART2 -> BRR = (uint16_t)(SystemCoreClock / baud);

	USART2 -> CR1 |= (1U << 0); // Usart Enable


}

void PrintChar(uint8_t b){
	USART2->TDR =(uint16_t)b;
	while(!(USART2->ISR&(1<<6)));
}
