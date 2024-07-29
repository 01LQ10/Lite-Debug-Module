#ifndef __DEBUG_H
#define __DEBUG_H

/*ͷ�ļ�*/
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
/** �û�����0 **
 * @brief ����ͷ�ļ�����������Ӳ��������ģ��
 * @example
 * #include "GPIO.h"
 * #include "USART.h"
 * #include "LED.h"
 */

typedef unsigned char uint8_t;
typedef unsigned short uint16_t; // 16 bits
typedef unsigned int uint32_t;	 // 32 bits

typedef union // ����ֵ
{
	uint8_t uc;	 // uchar
	uint16_t us; // ushort
	uint32_t ui; // uint
	int i;		 // int
	float f;	 // float
	double lf;	 // double
} OPT_arg_type;

/*��������*/
void Debug_init(void);
void USART_Interrupt_Handler(void);
void Debug_Handler(void);

#endif
