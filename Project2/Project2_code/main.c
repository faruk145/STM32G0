/*
* Project-2
* Author:Ömer Faruk Aydın 171024002
* main.c
*/
#include "bsp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define PI 3.14159
#define DEGREE PI / 180
int standby_time;
int indeks;
char s[] = "1702"; //idle
char ptemp[] = " ";
double temps;
int temp;
int result;
double resultd;
/*Calculation flags for integer results*/
int addition_flag;
int subtraction_flag;
int multiplication_flag;
int division_flag;
/*Calculation flags for floating results*/
int addition_flagf;
int subtraction_flagf;
int multiplication_flagf;
int division_flagf;
int isFloating;
/*Operation Flags*/
int E_flag;
int EA_flag;
int EB_flag;
int EC_flag;
int ED_flag;
int trigonometric_mode;
int EEA_flag;
int EEB_flag;
int EEC_flag;
int EED_flag;
int EEE_flag;
void EXTI0_1_IRQHandler(void){
//Interrupt comes from PB1 column 4
if (!strcmp(s,"1702")) //If it was in idle mode before clear all the digits.
discard_array(s);
clearRowsKeypad();
GPIOB->ODR ^= (1U << 2); //PB2 -> R4
if ((GPIOB->IDR >> 1) & 1){
standby_time = 0; //D button is pressed.
if (resultd == 0){ //Checking if floating result has obtained or not.Depending on result,different display function will be called.
division_flag = 1;
division_flagf = 0;
temp = atoi(s); //Value is in string.It converted to integer so it can be kept.It is same for all other A,B,C buttons.
}
else{
division_flag = 0;
division_flagf = 1;
isFloating = 1;
temps = atof(s); //Value is in string.It converted to float so it can be kept.It is same for all other A,B,C buttons.
}
if (E_flag && !trigonometric_mode){
ED_flag = 1;
strcpy(ptemp,s); //Keep the string in a temporary string.Because s string will be erased.It is same for all other operations.
}
if (trigonometric_mode){
EED_flag = 1;
strcpy(ptemp,s); //Keep the string in a temporary string.Because s string will be erased.It is same for all other A,B,C buttons.
}
indeks = 0; //Reset index so that next string consists of number can be entered.It is same for all other buttons.
discard_array(s); //Empty the string for the next number that will be entered.It is same for all other A,B,C buttons.
delay();
}
GPIOB->ODR ^= (1U << 2); //PB2 -> R4
GPIOB->ODR ^= (1U << 0); //PB0 -> R3
if ((GPIOB->IDR >> 1) & 1){
standby_time = 0; //C button is pressed.
if (resultd == 0){
multiplication_flag = 1;
multiplication_flagf = 0;
temp = atoi(s);
}
else{
multiplication_flag = 0;
multiplication_flagf = 1;
isFloating = 1;
temps = atof(s);
}
if (E_flag && !trigonometric_mode){
EC_flag = 1;
strcpy(ptemp,s);
}
if (trigonometric_mode){
EEC_flag = 1;
strcpy(ptemp,s);
}
indeks = 0;
discard_array(s);
delay();
}
GPIOB->ODR ^= (1U << 0); //PB0 -> R3
GPIOA->ODR ^= (1U << 9); //PA9 -> R2
if ((GPIOB->IDR >> 1) & 1){
standby_time = 0; //B button is pressed.
if (resultd == 0){
subtraction_flag = 1;
subtraction_flagf = 0;
temp = atoi(s);
}
else{
subtraction_flag = 0;
subtraction_flagf = 1;
isFloating = 1;
temps = atof(s);
}
if (E_flag && !trigonometric_mode){
EB_flag = 1;
strcpy(ptemp,s);
}
if (trigonometric_mode){
EEB_flag = 1;
strcpy(ptemp,s);
}
indeks = 0;
discard_array(s);
delay();
}
GPIOA->ODR ^= (1U << 9); //PA9 -> R2
GPIOA->ODR ^= (1U << 10); //PA10 -> R1
if ((GPIOB->IDR >> 1) & 1){
standby_time = 0; //A button is pressed.
if (resultd == 0){
addition_flag = 1;
addition_flagf = 0;
temp = atoi(s);
}
else{
addition_flag = 0;
addition_flagf = 1;
isFloating = 1;
temps = atof(s);
}
if (E_flag && !trigonometric_mode){
EA_flag = 1;
strcpy(ptemp,s);
}
if (trigonometric_mode){
EEA_flag = 1;
strcpy(ptemp,s);
}
indeks = 0;
discard_array(s);
delay();
}
GPIOA->ODR ^= (1U << 10); //PA10 -> R1
EXTI->RPR1 |= (1U << 1); //Clear interrupt flag
setAllRows();
}
void EXTI4_15_IRQHandler(void){
if (!strcmp(s,"1702"))
discard_array(s);
if ((EXTI->RPR1 & (1 << 15))) { //Interrupt comes from PA15
clearRowsKeypad();
GPIOB->ODR ^= (1U << 2); //PB2 -> R4
/*# equal button is pressed*/
if ((GPIOA->IDR >> 15) & 1){
standby_time = 0;
delay();
if(EA_flag == 1){
resultd = log10(atof(ptemp));
gcvt(resultd, 4, s); //Convert
floating result to string to read it later.
if (atof(ptemp) <= 0){ //log10's argument cannot be negative or zero.In that case display Inf
for(int i = 0; i < 10000; ++i)
Display_Invalid();
reset_values();
//After Displaying Inf go back to idle.
}
EA_flag = 0;
E_flag = 0;
discard_array(ptemp); //Empty the ptemp array to avoid displayin previous number.
addition_flagf = 0;
}
if(EB_flag == 1){
resultd = log(atof(ptemp));
gcvt(resultd, 4, s);
if (atof(ptemp) <= 0){ //log's argument cannot be negative or zero.In that case display Inf.
for(int i = 0; i < 10000; ++i)
Display_Invalid();
reset_values();
//After Displaying Inf go back to idle.
}
EB_flag = 0;
E_flag = 0;
discard_array(ptemp);
addition_flagf = 0;
subtraction_flagf = 0;
}
if(EC_flag == 1){
resultd = sqrt(atof(ptemp));
gcvt(resultd, 4, s);
if (atof(ptemp) < 0){ //sqrt's argument
cannot be negative.In that case display Inf.
for(int i = 0; i < 10000; ++i)
Display_Invalid();
reset_values();
}
EC_flag = 0;
E_flag = 0;
discard_array(ptemp);
addition_flagf = 0;
subtraction_flagf = 0;
multiplication_flagf = 0;
}
if(ED_flag == 1){
resultd = pow((atof(ptemp)),2);
gcvt(resultd, 4, s);
ED_flag = 0;
E_flag = 0;
discard_array(ptemp);
addition_flagf = 0;
subtraction_flagf = 0;
multiplication_flagf = 0;
division_flagf = 0;
}
if(EEA_flag == 1){
resultd = sin(atof(ptemp) * DEGREE);
//Trigonometric functions' arguments are degrees.
gcvt(resultd, 4, s);
EEA_flag = 0;
trigonometric_mode = 0;
E_flag = 0;
discard_array(ptemp);
addition_flagf = 0;
subtraction_flagf = 0;
multiplication_flagf = 0;
division_flagf = 0;
}
if(EEB_flag == 1){
resultd = cos(atof(ptemp) * DEGREE);
gcvt(resultd, 4, s);
EEB_flag = 0;
trigonometric_mode = 0;
E_flag = 0;
discard_array(ptemp);
addition_flagf = 0;
subtraction_flagf = 0;
multiplication_flagf = 0;
division_flagf = 0;
}
if(EEC_flag == 1){
resultd = tan(atof(ptemp) * DEGREE);
gcvt(resultd, 4, s);
EEC_flag = 0;
trigonometric_mode = 0;
E_flag = 0;
discard_array(ptemp);
addition_flagf = 0;
subtraction_flagf = 0;
multiplication_flagf = 0;
division_flagf = 0;
}
if(EED_flag == 1){
resultd = 1/tan(atof(ptemp) * DEGREE);
//cot function doesn't exist in C. 1/tan = cot
gcvt(resultd, 4, s);
if (atof(ptemp) == 0){
//cot's argument cannot be zero.
for(int i = 0; i < 10000; ++i)
Display_Invalid();
reset_values();
}
EED_flag = 0;
trigonometric_mode = 0;
E_flag = 0;
discard_array(ptemp);
addition_flagf = 0;
subtraction_flagf = 0;
multiplication_flagf = 0;
division_flagf = 0;
}
if(EEE_flag == 1){
resultd = 3.14159;
gcvt(resultd, 4, s);
EEE_flag = 0;
trigonometric_mode = 0;
E_flag = 0;
discard_array(ptemp);
addition_flagf = 0;
subtraction_flagf = 0;
multiplication_flagf = 0;
division_flagf = 0;
}
/*Perform operations as long as numbers are integer
and set necessary flags.*/
while (resultd == 0 && (addition_flag != 0 ||
subtraction_flag != 0 || multiplication_flag != 0 ||division_flag != 0)){
if (addition_flag && !isFloating){
result = temp + atoi(s); //Converts string to integer in order to perform addition.
itoa(result, s, 10); //Convert
result to string to prepare it for display.
addition_flag = 0;
}
if (subtraction_flag && !isFloating){
result = temp - atoi(s); //Convert
s string to integer in order to perform subtraction.
itoa(result, s, 10);
subtraction_flag = 0;
}
if (multiplication_flag && !isFloating){
result = temp * atoi(s); //Converts string to integer in order to perform multiplication.
itoa(result, s, 10);
multiplication_flag = 0;
}
if (division_flag && !isFloating){
temps = (double)temp;
//With the first division operation,make a transition to floating numbers.
resultd = (double)result;
resultd = (double)temps / atof(s);
if (atoi(s) == 0){
//Denominator cannot be zero.In that case display Inf.
for (int i = 0; i < 10000; ++i)
Display_Invalid();
reset_values();
}
gcvt(resultd, 4, s);
division_flag = 0;
}
}
/*Perform operations as long as numbers are double
and set necessary flags.*/
while (resultd != 0 && (addition_flagf != 0 ||
subtraction_flagf != 0 || multiplication_flagf != 0 ||division_flagf != 0)){
if (addition_flagf && isFloating){
resultd = temps + atof(s);
gcvt(resultd, 4, s);
addition_flagf = 0;
}
if (subtraction_flagf && isFloating){
resultd = temps - atof(s);
gcvt(resultd, 4, s);
subtraction_flagf = 0;
}
if (multiplication_flagf && isFloating){
resultd = temps * atof(s);
gcvt(resultd, 4, s);
multiplication_flagf = 0;
}
if (division_flagf && isFloating){
resultd = temps / atoi(s);
if (atoi(s) == 0){ //Denominator cannot be zero.In that case display Inf.
for (int i = 0; i < 10000; ++i)
Display_Invalid();
reset_values();
}
gcvt(resultd, 4, s);
division_flagf = 0;
}
}
}
GPIOB->ODR ^= (1U << 2); //PB2 -> R4
GPIOB->ODR ^= (1U << 0); //PB0 -> R3
if (((GPIOA->IDR >> 15) & 1) && indeks != 4){ //When button is pressed if all 4 digits are full.Ignore the button.
standby_time = 0;
s[indeks++] = '9'; //Place the number as a character.
delay();
}
GPIOB->ODR ^= (1U << 0); //PB0 -> R3
GPIOA->ODR ^= (1U << 9); //PA9 -> R2
if (((GPIOA->IDR >> 15) & 1) && indeks != 4){ //When button is pressed if all 4 digits are full.Ignore the button.
standby_time = 0;
s[indeks++] = '6'; //Place the number as a character.
delay();
}
GPIOA->ODR ^= (1U << 9); //PA9 -> R2
GPIOA->ODR ^= (1U << 10); //PA10 -> R1
if (((GPIOA->IDR >> 15) & 1) && indeks != 4){ //When button is pressed if all 4 digits are full.Ignore the button.
standby_time = 0;
s[indeks++] = '3'; //Place the number as a character.
delay();
}
GPIOA->ODR ^= (1U << 10); //PA10 -> R1
EXTI->RPR1 |= (1U << 15); //Clear interrupt flag
setAllRows();
}
else if ((EXTI->RPR1 & (1 << 7))) { //Interrupt comes from
PB7
clearRowsKeypad();
GPIOB->ODR ^= (1U << 2); //PB2 -> R4
if (((GPIOB->IDR >> 7) & 1) && indeks != 4){ //When button is pressed if all 4 digits are full.Ignore the button.
standby_time = 0;
s[indeks++] = '0'; //Place the number as a character.
delay();
}
GPIOB->ODR ^= (1U << 2); //PB2 -> R4
GPIOB->ODR ^= (1U << 0); //PB0 -> R3
if (((GPIOB->IDR >> 7) & 1) && indeks != 4){ //When button is pressed if all 4 digits are full.Ignore the button.
standby_time = 0;
s[indeks++] = '8'; //Place the number as a character.
delay();
}
GPIOB->ODR ^= (1U << 0); //PB0 -> R3
GPIOA->ODR ^= (1U << 9); //PA9 -> R2
if (((GPIOB->IDR >> 7) & 1) && indeks != 4){ //When button is pressed if all 4 digits are full.Ignore the button.
standby_time = 0;
s[indeks++] = '5'; //Place the number as a character.
delay();
}
GPIOA->ODR ^= (1U << 9); //PA9 -> R2
GPIOA->ODR ^= (1U << 10); //PA10 -> R1
if (((GPIOB->IDR >> 7) & 1) && indeks != 4){ //When button is pressed if all 4 digits are full.Ignore the button.
standby_time = 0;
s[indeks++] = '2'; //Place the number as a character.
delay();
}
GPIOA->ODR ^= (1U << 10); //PA10 -> R1
EXTI->RPR1 |= (1U << 7); //Clear interrupt flag
setAllRows();
}
else if ((EXTI->RPR1 & (1 << 6))) { //Interrupt comes from PB6
clearRowsKeypad();
GPIOB->ODR ^= (1U << 2); //PB2 -> R4
if ((GPIOB->IDR >> 6) & 1){
standby_time = 0; //E button is pressed.
/*Check the necessary flags.*/
if (trigonometric_mode == 1){
EEE_flag = 1;
E_flag = 0;
delay();
}
if (E_flag == 1){
trigonometric_mode = 1;
delay();
}
E_flag = 1;
delay();
}
GPIOB->ODR ^= (1U << 2); //PB2 -> R4
GPIOB->ODR ^= (1U << 0); //PB0 -> R3
if (((GPIOB->IDR >> 6) & 1) && indeks != 4){ //When button is pressed if all 4 digits are full.Ignore the button.
standby_time = 0;
s[indeks++] = '7'; //Place the number as a character.
delay();
}
GPIOB->ODR ^= (1U << 0); //PB0 -> R3
GPIOA->ODR ^= (1U << 9); //PA9 -> R2
if (((GPIOB->IDR >> 6) & 1) && indeks != 4){ //When button is pressed if all 4 digits are full.Ignore the button.
standby_time = 0;
s[indeks++] = '4'; //Place the number as a character.
delay();
}
GPIOA->ODR ^= (1U << 9); //PA9 -> R2
GPIOA->ODR ^= (1U << 10); //PA10 -> R1
if (((GPIOB->IDR >> 6) & 1) && indeks != 4){ //When button is pressed if all 4 digits are full.Ignore the button.
standby_time = 0;
s[indeks++] = '1'; //Place the number as a character.
delay();
}
GPIOA->ODR ^= (1U << 10); //PA10 -> R1
EXTI->RPR1 |= (1U << 6); //Clear interrupt flag
setAllRows();
}
}
void reset_values(void){
standby_time = 0;
result =0 ;
indeks = 0;
strcpy(s,"1702");
resultd = 0;
}
void initTIM3(void) {
/* Setup TIM3 */
RCC->APBENR1 |= (1U << 1);
TIM3->CR1 = 0;
TIM3->CR1 |= (1U << 7);
TIM3->CNT = 0;
TIM3->PSC = 99;
TIM3->ARR = (16);
TIM3->DIER |= (1U << 0);
TIM3->CR1 |= (1U << 0);
NVIC_SetPriority(TIM3_IRQn, 1);
NVIC_EnableIRQ(TIM3_IRQn);
}
void TIM3_IRQHandler(void) {
while(strchr(s,'.') == NULL){
++standby_time;
label:
Display_Number(atoi(s));
//Display integer Number
if (result > 9999 || result < -999 ){ //If result overflows display OuFL and go back to Idle
for (int i = 0; i < 10000; ++i)
display_Overflow();
reset_values();
}
if (standby_time == 80000) //If button is not pressed for 10 seconds go back to idle
reset_values();
}
while(resultd != 0){
Display_Floating_Number(resultd); //Display floating Number
++standby_time;
if (resultd > 9999 || resultd < -999 ){
for (int i = 0; i < 10000; ++i)
display_Overflow();
reset_values();
}
if (strchr(s,'.') == NULL)
goto label;
if (standby_time == 80000) //If button is not pressed for 10 seconds go back to idle
reset_values();
}
TIM3->SR &= ~(1U << 0); //Update Status Register
}
int main(void) {
SysTick_Config(SystemCoreClock / 1000);
Setup_Pins();
initTIM3();
 while(1) {
 }
 return 0;
}
