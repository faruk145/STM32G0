/*Author:Ömer Faruk Aydın 171024002 
Problem4
*/
.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.thumb
/* make linker see this */
.global Reset_Handler
/* get these from linker script */
.word _sdata
.word _edata
.word _sbss
.word _ebss
/* define peripheral addresses from RM0444 page 57, Tables 3-4 */
.equ RCC_BASE, (0x40021000) // RCC base address
.equ RCC_IOPENR, (RCC_BASE + (0x34)) // RCC IOPENR register offset
.equ GPIOA_BASE, (0x50000000) // GPIOA base address
.equ GPIOA_MODER, (GPIOA_BASE + (0x00)) // GPIOA MODER register offset
.equ GPIOA_ODR, (GPIOA_BASE + (0x14)) // GPIOA ODR register offset
.equ GPIOB_BASE, (0x50000400) // GPIOB base address
.equ GPIOB_MODER, (GPIOB_BASE + (0x00)) // GPIOB MODER register offset
.equ GPIOB_IDR, (GPIOB_BASE + (0x10))
/* vector table, +1 thumb mode */
.section .vectors
vector_table:
.word _estack /* Stack pointer */
.word Reset_Handler +1 /* Reset handler */
.word Default_Handler +1 /* NMI handler */
.word Default_Handler +1 /* HardFault handler */
/* add rest of them here if needed */
/* reset handler */
.section .text
Reset_Handler:
/* set stack pointer */
ldr r0, =_estack
mov sp, r0
/* initialize data and bss
* not necessary for rom only code
* */
bl init_data
/* call main */
bl main
/* trap if returned */
b .
/* initialize data and bss sections */
.section .text
init_data:
/* copy rom to ram */
ldr r0, =_sdata
ldr r1, =_edata
ldr r2, =_sidata
movs r3, #0
b LoopCopyDataInit
CopyDataInit:
ldr r4, [r2, r3]
str r4, [r0, r3]
adds r3, r3, #4
LoopCopyDataInit:
adds r4, r0, r3
cmp r4, r1
bcc CopyDataInit
/* zero bss */
ldr r2, =_sbss
ldr r4, =_ebss
movs r3, #0
b LoopFillZerobss
FillZerobss:
str r3, [r2]
adds r2, r2, #4
LoopFillZerobss:
cmp r2, r4
bcc FillZerobss
bx lr
/* default handler */
.section .text
Default_Handler:
b Default_Handler
/* main function */
.section .text
main:
push {lr}
/* enable GPIOB and GPIOA clock,IOPENR */
ldr r6, =RCC_IOPENR
ldr r5, [r6]
/* movs expects imm8, so this should be fine */
movs r4, 0x3
orrs r5, r5, r4
str r5, [r6]
/* setup A01234567 for 8 led in MODER */
ldr r6, =GPIOA_MODER
ldr r5, [r6]
/* cannot do with movs, so use pc relative */
ldr r4, =0x3C0FF0F
bics r5, r5, r4
ldr r4, =0x1405505
orrs r5, r5, r4
str r5, [r6]
/* setup PB2 for button in MODER */
ldr r6, =GPIOB_MODER
ldr r5, [r6]
/* cannot do with movs, so use pc relative */
ldr r4, =#0x30 //0011_0000 //pb2
mvns r4, r4
ands r5, r5, r4
str r5, [r6]
/* start */
bl t0
/* delay*/
ldr r1, =#319488
bl delay
bl t1
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x80
bics r5, r5, r4
str r5, [r6]
/* delay*/
ldr r1, =#319488
bl delay
shiftRight:
ldr r2, =0x1
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0xC1
bics r5, r5, r4
str r5, [r6]
bl t2
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0xC0
bics r5, r5, r4
str r5, [r6]
bl t3
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x8C0
bics r5, r5, r4
str r5, [r6]
bl t4
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x1840
bics r5, r5, r4
str r5, [r6]
bl t5
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x1820
bics r5, r5, r4
str r5, [r6]
bl t6
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x1030
bics r5, r5, r4
str r5, [r6]
bl t7
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x32
bics r5, r5, r4
str r5, [r6]
bl t8
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x13
bics r5, r5, r4
str r5, [r6]
bl t9
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x83
bics r5, r5, r4
str r5, [r6]
b shiftRight
t0:
/*set LED*/
/* turn on led connected to A01234567 in ODR */
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x80
orrs r5, r5, r4
str r5, [r6]
bx lr
t1:
/*set LED*/
/* turn on led connected to A01234567 in ODR */
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0xC0
orrs r5, r5, r4
str r5, [r6]
bx lr
t2:
/*set LED*/
/* turn on led connected to A01234567 in ODR */
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x8C0
orrs r5, r5, r4
str r5, [r6]
bx lr
t3:
/*set LED*/
/* turn on led connected to A01234567 in ODR */
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x1840
orrs r5, r5, r4
str r5, [r6]
bx lr
t4:
/*set LED*/
/* turn on led connected to A01234567 in ODR */
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x1820
orrs r5, r5, r4
str r5, [r6]
bx lr
t5:
/*set LED*/
/* turn on led connected to A01234567 in ODR */
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x1030
orrs r5, r5, r4
str r5, [r6]
bx lr
t6:
/*set LED*/
/* turn on led connected to A01234567 in ODR */
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x32
orrs r5, r5, r4
str r5, [r6]
bx lr
t7:
/*set LED*/
/* turn on led connected to A01234567 in ODR */
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x13
orrs r5, r5, r4
str r5, [r6]
bx lr
t8:
/*set LED*/
/* turn on led connected to A01234567 in ODR */
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x83
orrs r5, r5, r4
str r5, [r6]
bx lr
t9:
/*set LED*/
/* turn on led connected to A01234567 in ODR */
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0xC1
orrs r5, r5, r4
str r5, [r6]
bx lr
delay:
ldr r6, =GPIOB_IDR
ldr r5, [r6]
ldr r4, =0x4
cmp r5, r4
beq shift
subs r1, r1, #1
bne delay
bx lr
shift:
ldr r1, =0x1
cmp r2, r1
beq shiftLeft
bl shiftRight
shiftLeft:
ldr r2, =0x0
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x8C0
bics r5, r5, r4
str r5, [r6]
bl t9
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x8C0
bics r5, r5, r4
str r5, [r6]
bl t8
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0xC1
bics r5, r5, r4
str r5, [r6]
bl t7
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x83
bics r5, r5, r4
str r5, [r6]
bl t6
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x13
bics r5, r5, r4
str r5, [r6]
bl t5
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x32
bics r5, r5, r4
str r5, [r6]
bl t4
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x1030
bics r5, r5, r4
str r5, [r6]
bl t3
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x1820
bics r5, r5, r4
str r5, [r6]
bl t2
/* delay*/
ldr r1, =#319488
bl delay
ldr r6, =GPIOA_ODR
ldr r5, [r6]
ldr r4, =0x1840
bics r5, r5, r4
str r5, [r6]
b shiftLeft
