#include "Debug.h"

/*ȫ�ֱ���*/
#define BUFF_MAX 20				// ��󻺴�����
char Uart_Buff[BUFF_MAX];		// UART���ջ���
uint8_t Uart_Rx_Num = 0;		// UART����ָ��
uint8_t Flag_OPT_Handle = 0;	// ������־
uint8_t Flag_BUFF_OverFlow = 0; // ���������־
uint8_t arg_offset = 0;			// ����ƫ��
OPT_arg_type opt_arg;			// �����洢

/*��ݲ���*/
#define SET_Flag_OPT_Handle Flag_OPT_Handle = 1
#define CLEAR_Flag_OPT_Handle Flag_OPT_Handle = 0
#define Char_is(x) (Uart_Buff[Uart_Rx_Num] == x)
#define Char_isnt(x) (Uart_Buff[Uart_Rx_Num] != x)

/*****************************************************************
 * @brief Init debug module.
 * 	Complete the configuration of gpio, uart and other peripherals
 * 	��ʼ��Debugģ�飬���gpio��uart�����������
 * @param None
 * @return None
 *****************************************************************/
void Debug_init(void)
{
	/** �û�����1 **
	 * @brief ���ڴ��ڳ�ʼ��
	 * @example UART_Init(); GPIO_Init(); NVIC_config();
	 */
}

/*****************************************************************
 * @brief UART Interrupt request handler, put this function in the
 * 	�����жϴ����������봮���жϺ�����
 * @param None
 * @return None
 *****************************************************************/
void USART_Interrupt_Handler(void)
{
	if (Uart_Rx_Num >= BUFF_MAX)
	{
		Uart_Rx_Num = 0;
		Flag_BUFF_OverFlow = 1; // ���𻺴������־
	}

	/*��ȡ�ַ�*/
	/** �û�����2 **
	 * @brief ��ȡ��������
	 * @example Uart_Buff[Uart_Rx_Num] = SBUF;
	 */
	Uart_Buff[Uart_Rx_Num] = getchar(); // ����ģ�⴮������

	if (Char_is('\n') || Char_is('\0') || Char_is('*')) // ����������
	{
		if (Uart_Rx_Num != 0)
		{
			SET_Flag_OPT_Handle;
			Uart_Buff[Uart_Rx_Num] = '\0';
			Uart_Rx_Num = 0;
		}
	}
	else if (Char_isnt(' ')) // �����ض��ַ�
		Uart_Rx_Num++;
}

/*****************************************************************
 * @brief Debug handler, put this function in the while loop
 * 	Debug������������whileѭ����
 * @param None
 * @return None
 *****************************************************************/
void Debug_Handler(void)
{
	uint8_t i;
	if (Flag_OPT_Handle == 1) // �ж�������־�Ƿ����
	{
		/*****  �����������  *****/
		if (Flag_BUFF_OverFlow)
		{
			Flag_BUFF_OverFlow = 0; // ������������־
			printf("\e[1;31m!!!erro: BUFF OVER FLOW\n\e[0m");
			CLEAR_Flag_OPT_Handle; // ���������־
			return;				   // ����������
		}

		/*****  ����Ԥ����  *****/
		arg_offset = 0;			 // ����ƫ�ƹ���
		for (i = 0; i < 50; i++) // ������������
		{
			if (Uart_Buff[i] == '\0')
				break;
			if (Uart_Buff[i] == ':')
			{
				Uart_Buff[i] = '\0';
				arg_offset = i + 1;
				break;
			}
		}

		/*****  ����ƥ����ִ��  *****/
		#define OPTION_IS(x) (strcmp(Uart_Buff, x) == 0)
		#define OPTION_DATA &Uart_Buff[arg_offset]

		/** �û�����3 **
		 * @brief �û��Զ�������
		 * @example ����
		 */
		/*��1���޲�������*/
		if (OPTION_IS("TEST")) // �ж�����ƥ���ַ���
		{
			printf("TEST SECCUSS\n"); // ִ�в���
		}
		/*��2����������*/
		else if (OPTION_IS("OPEN_LED")) // �ж�����ƥ���ַ���
		{
			opt_arg.i = atoi(OPTION_DATA); // ת����������
			if (opt_arg.i == 1)			   // ִ�в���
			{
				printf("OPEN LED1\n");
			}
			else if (opt_arg.i == 2)
			{
				printf("OPEN LED2\n");
			}
			else
				printf("\e[1;33m!!wrang: data isn't in range\n\e[0m");
		}
		/*��3��С������*/
		else if (OPTION_IS("INPUT_FLOAT")) // �ж�����ƥ���ַ���
		{
			opt_arg.f = atof(OPTION_DATA);			 // ת��С������
			printf("float data is:%f\n", opt_arg.f); // ִ�в���
		}
		/*�˳�debug*/
		else if (OPTION_IS("EXIT"))
		{
			exit(0);
		}
		/*��ƥ������*/
		else
		{
			printf("\e[1;31m!!!erro: here is no this option\n\e[0m");
		}

		CLEAR_Flag_OPT_Handle; // ���������־
	}
}

/*����ģ������*/
int main(void)
{
	Debug_init();
	while (1)
	{
		USART_Interrupt_Handler();
		Debug_Handler();
	}
}
