/*
bsp.c
*/
#include "bsp.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
void clear_SSD(void){
GPIOA->ODR |= (1U << 0); //PA0 Decimal Point
GPIOA->ODR |= (1U << 1); //PA1 A
GPIOA->ODR |= (1U << 4); //PA4 B
GPIOA->ODR |= (1U << 5); //PA5 C
GPIOA->ODR |= (1U << 12); //PA12 D
GPIOA->ODR |= (1U << 11); //PA11 E
GPIOA->ODR |= (1U << 6); //PA6 F
GPIOA->ODR |= (1U << 7); //PA7 G
}
void clearRowsKeypad(void){
GPIOA->ODR &= ~(1U << 10); //PA10
GPIOA->ODR &= ~(1U << 9);//PA9
GPIOB->ODR &= ~(1U << 0);//PB0
GPIOB->ODR &= ~(1U << 2);//PB2
}
void setAllRows(void){
GPIOB->ODR |= (1U << 2); //PB2->R4
GPIOB->ODR |= (1U << 0); //PB0->R3
GPIOA->ODR |= (1U << 9); //PA9->R2
GPIOA->ODR |= (1U << 10); //PA10->R1
}
void Setup_Pins(void){
RCC->IOPENR |= (3U << 0); //Enable GPIOA and GPIOB
ports
/*Setup SSD*/
GPIOA->MODER &= ~(3U << 0); //PA0 for Decimal
Point in SSD
GPIOA->MODER |= (1U << 0);
GPIOA->MODER &= ~(3U << 2*1);//PA1 for A in SSD
GPIOA->MODER |= (1U << 2*1);
GPIOA->MODER &= ~(3U << 2*4);//PA4 for B in SSD
GPIOA->MODER |= (1U << 2*4);
GPIOA->MODER &= ~(3U << 2*5);//PA5 for C in SSD
GPIOA->MODER |= (1U << 2*5);
GPIOA->MODER &= ~(3U << 2*12); //PA12 for D in
SSD
GPIOA->MODER |= (1U << 2*12);
GPIOA->MODER &= ~(3U << 2*11); //PA11 for E in
SSD
GPIOA->MODER |= (1U << 2*11);
GPIOA->MODER &= ~(3U << 2*6);//PA6 for F in SSD
GPIOA->MODER |= (1U << 2*6);
GPIOA->MODER &= ~(3U << 2*7);//PA7 for G in SSD
GPIOA->MODER |= (1U << 2*7);
GPIOB->MODER &= ~(3U << 2*4);//PB4 for D1 in SSD
GPIOB->MODER |= (1U << 2*4);
GPIOB->MODER &= ~(3U << 2*5);//PB5 for D2 in SSD
GPIOB->MODER |= (1U << 2*5);
GPIOB->MODER &= ~(3U << 2*9);//PB9 for D3 in SSD
GPIOB->MODER |= (1U << 2*9);
GPIOB->MODER &= ~(3U << 2*8);//PB8 for D4 in SSD
GPIOB->MODER |= (1U << 2*8);
/*Setup Membrane 4x4 Keypad*/
GPIOA->MODER &= ~(3U << 2*10); //PA10 for R1 in
4x4 Keypad
GPIOA->MODER |= (1U << 2*10);
GPIOA->MODER &= ~(3U << 2*9);//PA9 for R2 in 4x4
Keypad
GPIOA->MODER |= (1U << 2*9);
GPIOB->MODER &= ~(3U << 0); //PB0 for R3 in
4x4 Keypad
GPIOB->MODER |= (1U << 0);
GPIOB->MODER &= ~(3U << 2*2);//PB2 for R4 in 4x4
Keypad
GPIOB->MODER |= (1U << 2*2);
GPIOB->MODER &= ~(3U << 2*6);//PB6 for C1 in 4x4
Keypad
GPIOB->PUPDR |= (2U << 2*6); //Pull-down mode
GPIOB->MODER &= ~(3U << 2*7);//PB7 for C2 in 4x4
Keypad
GPIOB->PUPDR |= (2U << 2*7); //Pull-down mode
GPIOA->MODER &= ~(3U << 2*15); //PA15 for C3 in
4x4 Keypad
GPIOA->PUPDR |= (2U << 2*15); //Pull-down mode
GPIOB->MODER &= ~(3U << 2*1);//PB1 for C4 in 4x4
Keypad
GPIOB->PUPDR |= (2U << 2*1); //Pull-down mode
/*Setup interrupts for inputs*/
EXTI->EXTICR[1] |= (1U << 8*2); //PB6
EXTI->EXTICR[1] |= (1U << 8*3); //PB7
EXTI->EXTICR[3] |= (0U << 8*3); //PA15
EXTI->EXTICR[0] |= (1U << 8*1); //PB1
/*Rising Edge*/
EXTI->RTSR1 |= (1U << 6); //6th pin
EXTI->RTSR1 |= (1U << 7); //7th pin
EXTI->RTSR1 |= (1U << 15); //15th pin
EXTI->RTSR1 |= (1U << 1); //1st pin
/*Mask*/
EXTI->IMR1 |= (1U << 6); //6th pin
EXTI->IMR1 |= (1U << 7); //7th pin
EXTI->IMR1 |= (1U << 15); //15th pin
EXTI->IMR1 |= (1U << 1); //1st pin
/*NVIC*/
NVIC_SetPriority(EXTI0_1_IRQn, 0);
NVIC_EnableIRQ(EXTI0_1_IRQn);
NVIC_SetPriority(EXTI4_15_IRQn, 0);
NVIC_EnableIRQ(EXTI4_15_IRQn);
/*Set all rows for 5V*/
GPIOA->ODR |= (1U << 10); //PA10
GPIOA->ODR |= (1U << 9); //PA9
GPIOB->ODR |= (1U << 0); //PB0
GPIOB->ODR |= (1U << 2); //PB2
clear_SSD();
}
int ndigit(int num){ //Find the number of digits.
int count = 0;
if (num == 0)
return 1;
while (num != 0) {
++count;
num /= 10;
}
return count;
}
void setSSD(int x){
clear_SSD();
switch(x){
case 0:
GPIOA->ODR &= ~(1U << 1); //PA1 A
GPIOA->ODR &= ~(1U << 4); //PA4 B
GPIOA->ODR &= ~(1U << 5); //PA5 C
GPIOA->ODR &= ~(1U << 12); //PA12 D
GPIOA->ODR &= ~(1U << 11); //PA11 E
GPIOA->ODR &= ~(1U << 6); //PA6 F
break;
case 1:
GPIOA->ODR &= ~(1U << 4); //PA4 B
GPIOA->ODR &= ~(1U << 5); //PA5 C
break;
case 2:
GPIOA->ODR &= ~(1U << 1); //PA1 A
GPIOA->ODR &= ~(1U << 4); //PA4 B
GPIOA->ODR &= ~(1U << 12); //PA12 D
GPIOA->ODR &= ~(1U << 11); //PA11 E
GPIOA->ODR &= ~(1U << 7); //PA7 G
break;
case 3:
GPIOA->ODR &= ~(1U << 1); //PA1 A
GPIOA->ODR &= ~(1U << 4); //PA4 B
GPIOA->ODR &= ~(1U << 5); //PA5 C
GPIOA->ODR &= ~(1U << 12); //PA12 D
GPIOA->ODR &= ~(1U << 7); //PA7 G
break;
case 4:
GPIOA->ODR &= ~(1U << 4); //PA4 B
GPIOA->ODR &= ~(1U << 5); //PA5 C
GPIOA->ODR &= ~(1U << 6); //PA6 F
GPIOA->ODR &= ~(1U << 7); //PA7 G
break;
case 5:
GPIOA->ODR &= ~(1U << 1); //PA1 A
GPIOA->ODR &= ~(1U << 5); //PA5 C
GPIOA->ODR &= ~(1U << 12); //PA12 D
GPIOA->ODR &= ~(1U << 7); //PA7 G
GPIOA->ODR &= ~(1U << 6); //PA6 F
break;
case 6:
GPIOA->ODR &= ~(1U << 12); //PA12 D
GPIOA->ODR &= ~(1U << 11); //PA11 E
GPIOA->ODR &= ~(1U << 6); //PA6 F
GPIOA->ODR &= ~(1U << 7); //PA7 G
GPIOA->ODR &= ~(1U << 1); //PA1 A
GPIOA->ODR &= ~(1U << 5); //PA5 C
break;
case 7:
GPIOA->ODR &= ~(1U << 1); //PA1 A
GPIOA->ODR &= ~(1U << 4); //PA4 B
GPIOA->ODR &= ~(1U << 5); //PA5 C
break;
case 8:
GPIOA->ODR &= ~(1U << 1); //PA1 A
GPIOA->ODR &= ~(1U << 4); //PA4 B
GPIOA->ODR &= ~(1U << 5); //PA5 C
GPIOA->ODR &= ~(1U << 12); //PA12 D
GPIOA->ODR &= ~(1U << 11); //PA11 E
GPIOA->ODR &= ~(1U << 6); //PA6 F
GPIOA->ODR &= ~(1U << 7); //PA7 G
break;
case 9:
GPIOA->ODR &= ~(1U << 1); //PA1 A
GPIOA->ODR &= ~(1U << 4); //PA4 B
GPIOA->ODR &= ~(1U << 5); //PA5 C
GPIOA->ODR &= ~(1U << 12); //PA12 D
GPIOA->ODR &= ~(1U << 6); //PA6 F
GPIOA->ODR &= ~(1U << 7); //PA7 G
break;
}
}
void idle(void){
/*Display 1702 School ID*/
setSSD(1);
GPIOB->ODR |= (1U << 4);
GPIOB->ODR &= ~(1U << 4);
setSSD(7);
GPIOB->ODR |= (1U << 5);
GPIOB->ODR &= ~(1U << 5);
setSSD(0);
GPIOB->ODR |= (1U << 9);
GPIOB->ODR &= ~(1U << 9);
setSSD(2);
GPIOB->ODR |= (1U << 8);
GPIOB->ODR &= ~(1U << 8);
}
void clear_Digit(int x){
switch(x){
case 1:
GPIOB->ODR &= ~(1U << 4);
break;
case 2:
GPIOB->ODR &= ~(1U << 5);
break;
case 3:
GPIOB->ODR &= ~(1U << 9);
break;
case 4:
GPIOB->ODR &= ~(1U << 8);
break;
}
}
void TurnOn_Digit(int x){
switch(x){
case 1:
GPIOB->ODR |= (1U << 4);
break;
case 2:
GPIOB->ODR |= (1U << 5);
break;
case 3:
GPIOB->ODR |= (1U << 9);
break;
case 4:
GPIOB->ODR |= (1U << 8);
break;
}
}
void Display_Number(int x){ //Display function for integer numbers.
clear_SSD();
/*Check the number of digits and whether it is positive or
not.*/
if (ndigit(x) == 1 && x >=0){
setSSD(x % 10);
TurnOn_Digit(4);
clear_Digit(4);
}
else if (ndigit(x) == 2 && x >=0){
setSSD(x / 10);
TurnOn_Digit(3);
clear_Digit(3);
setSSD(x % 10);
TurnOn_Digit(4);
clear_Digit(4);
}
else if (ndigit(x) == 3 && x >=0){
setSSD(x / 100);
TurnOn_Digit(2);
clear_Digit(2);
setSSD((x / 10) % 10);
TurnOn_Digit(3);
clear_Digit(3);
setSSD(x % 10);
TurnOn_Digit(4);
clear_Digit(4);
}
else if (ndigit(x) == 4 && x >=0){
setSSD(x / 1000);
TurnOn_Digit(1);
clear_Digit(1);
setSSD((x / 100) % 10);
TurnOn_Digit(2);
clear_Digit(2);
setSSD((x / 10) % 10);
TurnOn_Digit(3);
clear_Digit(3);
setSSD(x % 10);
TurnOn_Digit(4);
clear_Digit(4);
}
/*Minus symbol must be moved depending on the number of 
digits.*/
else if (x < 0){
GPIOA->ODR &= ~(1U << 7); //PA7 G for minus
symbol
if (ndigit(x) == 1){
TurnOn_Digit(3);
clear_Digit(3);
setSSD(abs(x) % 10);
TurnOn_Digit(4);
clear_Digit(4);
}
else if (ndigit(x) == 2){
TurnOn_Digit(2);
clear_Digit(2);
setSSD((abs(x) / 10) % 10);
TurnOn_Digit(3);
clear_Digit(3);
setSSD(abs(x) % 10);
TurnOn_Digit(4);
clear_Digit(4);
}
else if (ndigit(x) == 3){
TurnOn_Digit(1);
clear_Digit(1);
setSSD((abs(x) / 100) % 10);
TurnOn_Digit(2);
clear_Digit(2);
setSSD((abs(x) / 10) % 10);
TurnOn_Digit(3);
clear_Digit(3);
setSSD(abs(x) % 10);
TurnOn_Digit(4);
clear_Digit(4);
}
}
}
void Display_Floating_Number(double x){
clear_SSD();
/*Check the digit number and whether it is positive or not.*/
if (ndigit(x) == 1 && x >= 0){
double tam, ondalik;
ondalik = modf(x, &tam); //Extract
the floating part in order to obtain it seperately.
setSSD((int)tam);
GPIOA->ODR &= ~(1U << 0); //PA0
Turn on Decimal Point
TurnOn_Digit(1);
clear_Digit(1);
setSSD(((int)(ondalik * 1000)) / 100);
TurnOn_Digit(2);
clear_Digit(2);
setSSD((((int)(ondalik * 1000)) / 10) % 10);
TurnOn_Digit(3);
clear_Digit(3);
setSSD((((int)(ondalik * 1000))) % 10);
TurnOn_Digit(4);
clear_Digit(4);
}
else if (ndigit(x) == 2 && x >= 0){
double tam, ondalik;
ondalik = modf(x, &tam);
setSSD(((int)tam) / 10);
TurnOn_Digit(1);
clear_Digit(1);
setSSD(((int)tam) % 10);
GPIOA->ODR &= ~(1U << 0); //PA0
Turn on Decimal Point
TurnOn_Digit(2);
clear_Digit(2);
setSSD(((int)(ondalik * 100)) / 10);
TurnOn_Digit(3);
clear_Digit(3);
setSSD((((int)(ondalik * 100))) % 10);
TurnOn_Digit(4);
clear_Digit(4);
}
else if (ndigit(x) == 3 && x >= 0){
double tam, ondalik;
ondalik = modf(x, &tam);
setSSD(((int)tam) / 100);
TurnOn_Digit(1);
clear_Digit(1);
setSSD((((int)tam) / 10) % 10);
TurnOn_Digit(2);
clear_Digit(2);
setSSD(((int)tam) % 10);
GPIOA->ODR &= ~(1U << 0); //PA0
Turn on Decimal Point
TurnOn_Digit(3);
clear_Digit(3);
setSSD(((int)(ondalik * 10)) % 10);
TurnOn_Digit(4);
clear_Digit(4);
}
else if(ndigit(x) == 4 && x >= 0){
double tam;
modf(x, &tam);
setSSD(((int)tam) / 1000);
TurnOn_Digit(1);
clear_Digit(1);
setSSD((((int)tam) / 100) % 10);
TurnOn_Digit(2);
clear_Digit(2);
setSSD((((int)tam) / 10) % 10);
TurnOn_Digit(3);
clear_Digit(3);
setSSD(((int)tam) % 10);
GPIOA->ODR &= ~(1U << 0); //PA0
Turn on Decimal Point
TurnOn_Digit(4);
clear_Digit(4);
}
else if(ndigit(x) == 1 && x < 0){
double tam, ondalik;
ondalik = modf(x, &tam);
GPIOA->ODR &= ~(1U << 7); //PA7 G
for minus symbol
TurnOn_Digit(1);
clear_Digit(1);
setSSD(abs((int)tam));
GPIOA->ODR &= ~(1U << 0); //PA0
Turn on Decimal Point
TurnOn_Digit(2);
clear_Digit(2);
setSSD(abs((int)(ondalik * 100) / 10));
TurnOn_Digit(3);
clear_Digit(3);
setSSD(abs((int)(ondalik * 100) % 10));
TurnOn_Digit(4);
clear_Digit(4);
}
else if(ndigit(x) == 2 && x < 0){
double tam, ondalik;
ondalik = modf(x, &tam);
GPIOA->ODR &= ~(1U << 7); //PA7 G
for minus symbol
TurnOn_Digit(1);
clear_Digit(1);
setSSD(abs((int)tam) / 10);
TurnOn_Digit(2);
clear_Digit(2);
setSSD((abs((int)tam) % 10));
GPIOA->ODR &= ~(1U << 0); //PA0
Turn on Decimal Point
TurnOn_Digit(3);
clear_Digit(3);
setSSD((abs((int)(ondalik * 10)) % 10));
TurnOn_Digit(4);
clear_Digit(4);
}
else if (ndigit(x) == 3 && x < 0){
double tam;
modf(x, &tam);
GPIOA->ODR &= ~(1U << 7); //PA7 G
for minus symbol
TurnOn_Digit(1);
clear_Digit(1);
setSSD(abs((int)tam) / 100);
TurnOn_Digit(2);
clear_Digit(2);
setSSD(((abs((int)tam) / 10) % 10));
TurnOn_Digit(3);
clear_Digit(3);
setSSD((abs((int)tam) % 10));
GPIOA->ODR &= ~(1U << 0); //PA0
Turn on Decimal Point
TurnOn_Digit(4);
clear_Digit(4);
}
}
void delay(void){
for(int i = 0; i < 200000; ++i);
}
void discard_array(char *s){ //Reset the string
s[0] = '\0';
s[1] = '\0';
s[2] = '\0';
s[3] = '\0';
}
void display_Overflow(void){
clear_SSD();
/*Display O*/
setSSD(0);
TurnOn_Digit(1);
clear_Digit(1);
clear_SSD();
/*Display U*/
GPIOA->ODR &= ~(1U << 4);//PA4 B
GPIOA->ODR &= ~(1U << 5);//PA5 C
GPIOA->ODR &= ~(1U << 12); //PA12 D
GPIOA->ODR &= ~(1U << 11); //PA11 E
GPIOA->ODR &= ~(1U << 6);//PA6 F
TurnOn_Digit(2);
clear_Digit(2);
clear_SSD();
/*Display F*/
GPIOA->ODR &= ~(1U << 11); //PA11 E
GPIOA->ODR &= ~(1U << 6);//PA6 F
GPIOA->ODR &= ~(1U << 7);//PA7 G
GPIOA->ODR &= ~(1U << 1);//PA1 A
TurnOn_Digit(3);
clear_Digit(3);
clear_SSD();
/*Display L*/
GPIOA->ODR &= ~(1U << 12); //PA12 D
GPIOA->ODR &= ~(1U << 11); //PA11 E
GPIOA->ODR &= ~(1U << 6);//PA6 F
TurnOn_Digit(4);
clear_Digit(4);
}
void Display_Invalid(void){
/*Display I*/
clear_SSD();
GPIOA->ODR &= ~(1U << 11); //PA11 E
GPIOA->ODR &= ~(1U << 6);//PA6 F
TurnOn_Digit(1);
clear_Digit(1);
clear_SSD();
/*Display N*/
GPIOA->ODR &= ~(1U << 11); //PA11 E
GPIOA->ODR &= ~(1U << 6);//PA6 F
GPIOA->ODR &= ~(1U << 1);//PA1 A
GPIOA->ODR &= ~(1U << 4);//PA4 B
GPIOA->ODR &= ~(1U << 5);//PA5 C
TurnOn_Digit(2);
clear_Digit(2);
clear_SSD();
/*Display F*/
GPIOA->ODR &= ~(1U << 11); //PA11 E
GPIOA->ODR &= ~(1U << 6);//PA6 F
GPIOA->ODR &= ~(1U << 7);//PA7 G
GPIOA->ODR &= ~(1U << 1);//PA1 A
TurnOn_Digit(3);
clear_Digit(3);
clear_SSD();
}
