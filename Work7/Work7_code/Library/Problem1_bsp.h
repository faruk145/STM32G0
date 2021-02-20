/*Author: Ömer Faruk Aydın 
Problem-1
*/
#ifndef INC_BSP_H_
#define INC_BSP_H_

#include "stm32g0xx.h"

void initTIM1(void);
void TIM1_BRK_UP_TRG_COM_IRQHandler(void);
void initTIM3(void);
void TIM3_IRQHandler(void);
void initPWMPin(void);
void initADC(void);
uint16_t startADC(void);

#endif /* INC_BSP_H_ */
