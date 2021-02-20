/*Author: Ömer Faruk Aydın 
Problem-1
*/
#include "bsp.h"

int main(void) {

    initPWMPin();
    initTIM3();
    initADC();
    initTIM1();

	while (1) {
	}
    return 0;
}
