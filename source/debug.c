#include "Debug.h"

/*全局变量*/
#define BUFF_MAX 20				// 最大缓存数量
char Uart_Buff[BUFF_MAX];		// UART接收缓存
uint8_t Uart_Rx_Num = 0;		// UART接收指针
uint8_t Flag_OPT_Handle = 0;	// 命令处理标志
uint8_t Flag_BUFF_OverFlow = 0; // 缓存溢出标志
uint8_t arg_offset = 0;			// 参数偏移
OPT_arg_type opt_arg;			// 参数存储

/*便捷操作*/
#define SET_Flag_OPT_Handle Flag_OPT_Handle = 1
#define CLEAR_Flag_OPT_Handle Flag_OPT_Handle = 0
#define Char_is(x) (Uart_Buff[Uart_Rx_Num] == x)
#define Char_isnt(x) (Uart_Buff[Uart_Rx_Num] != x)

/*****************************************************************
 * @brief Init debug module.
 * 	Complete the configuration of gpio, uart and other peripherals
 * 	初始化Debug模块，完成gpio、uart等外设的配置
 * @param None
 * @return None
 *****************************************************************/
void Debug_init(void)
{
	/** 用户设置1 **
	 * @brief 串口串口初始化
	 * @example UART_Init(); GPIO_Init(); NVIC_config();
	 */
}

/*****************************************************************
 * @brief UART Interrupt request handler, put this function in the
 * 	串口中断处理函数，放入串口中断函数里
 * @param None
 * @return None
 *****************************************************************/
void USART_Interrupt_Handler(void)
{
	if (Uart_Rx_Num >= BUFF_MAX)
	{
		Uart_Rx_Num = 0;
		Flag_BUFF_OverFlow = 1; // 挂起缓存溢出标志
	}

	/*获取字符*/
	/** 用户设置2 **
	 * @brief 读取串口输入
	 * @example Uart_Buff[Uart_Rx_Num] = SBUF;
	 */
	Uart_Buff[Uart_Rx_Num] = getchar(); // 电脑模拟串口输入

	if (Char_is('\n') || Char_is('\0') || Char_is('*')) // 检测结束符号
	{
		if (Uart_Rx_Num != 0)
		{
			SET_Flag_OPT_Handle;
			Uart_Buff[Uart_Rx_Num] = '\0';
			Uart_Rx_Num = 0;
		}
	}
	else if (Char_isnt(' ')) // 屏蔽特定字符
		Uart_Rx_Num++;
}

/*****************************************************************
 * @brief Debug handler, put this function in the while loop
 * 	Debug处理函数，放入while循环里
 * @param None
 * @return None
 *****************************************************************/
void Debug_Handler(void)
{
	uint8_t i;
	if (Flag_OPT_Handle == 1) // 判断命令处理标志是否挂起
	{
		/*****  缓存溢出处理  *****/
		if (Flag_BUFF_OverFlow)
		{
			Flag_BUFF_OverFlow = 0; // 清除缓存溢出标志
			printf("\e[1;31m!!!erro: BUFF OVER FLOW\n\e[0m");
			CLEAR_Flag_OPT_Handle; // 清除命令处理标志
			return;				   // 跳出处理函数
		}

		/*****  命令预处理  *****/
		arg_offset = 0;			 // 参数偏移归零
		for (i = 0; i < 50; i++) // 拆分命令和数据
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

		/*****  命令匹配与执行  *****/
		#define OPTION_IS(x) (strcmp(Uart_Buff, x) == 0)
		#define OPTION_DATA &Uart_Buff[arg_offset]

		/** 用户设置3 **
		 * @brief 用户自定义命令
		 * @example 如下
		 */
		/*例1：无参数命令*/
		if (OPTION_IS("TEST")) // 判断命令匹配字符串
		{
			printf("TEST SECCUSS\n"); // 执行操作
		}
		/*例2：整数参数*/
		else if (OPTION_IS("OPEN_LED")) // 判断命令匹配字符串
		{
			opt_arg.i = atoi(OPTION_DATA); // 转换整数参数
			if (opt_arg.i == 1)			   // 执行操作
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
		/*例3：小数参数*/
		else if (OPTION_IS("INPUT_FLOAT")) // 判断命令匹配字符串
		{
			opt_arg.f = atof(OPTION_DATA);			 // 转换小数参数
			printf("float data is:%f\n", opt_arg.f); // 执行操作
		}
		/*退出debug*/
		else if (OPTION_IS("EXIT"))
		{
			exit(0);
		}
		/*无匹配命令*/
		else
		{
			printf("\e[1;31m!!!erro: here is no this option\n\e[0m");
		}

		CLEAR_Flag_OPT_Handle; // 清除命令处理标志
	}
}

/*电脑模拟运行*/
int main(void)
{
	Debug_init();
	while (1)
	{
		USART_Interrupt_Handler();
		Debug_Handler();
	}
}
