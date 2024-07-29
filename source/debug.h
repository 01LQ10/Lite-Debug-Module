#ifndef __DEBUG_H
#define __DEBUG_H

/*头文件*/
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
/** 用户设置0 **
 * @brief 引入头文件，包括串口硬件、其他模块
 * @example
 * #include "GPIO.h"
 * #include "USART.h"
 * #include "LED.h"
 */

typedef unsigned char uint8_t;
typedef unsigned short uint16_t; // 16 bits
typedef unsigned int uint32_t;	 // 32 bits

typedef union // 参数值
{
	uint8_t uc;	 // uchar
	uint16_t us; // ushort
	uint32_t ui; // uint
	int i;		 // int
	float f;	 // float
	double lf;	 // double
} OPT_arg_type;

/*函数声明*/
void Debug_init(void);
void USART_Interrupt_Handler(void);
void Debug_Handler(void);

#endif
