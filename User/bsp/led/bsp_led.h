#ifndef __LED_H
#define	__LED_H


#include "stm32f10x.h"


/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
// R-红色
#define LED0_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define LED0_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED0_GPIO_PIN		GPIO_Pin_5			        /* 连接到SCL时钟线的GPIO */

// G-绿色
#define LED1_GPIO_PORT    	GPIOE			              /* GPIO端口 */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOE		/* GPIO端口时钟 */
#define LED1_GPIO_PIN		GPIO_Pin_5			        /* 连接到SCL时钟线的GPIO */


/** the macro definition to trigger the led on or off 
  * 1 - off
  * 0 - on
  */
#define ON  0
#define OFF 1

/* 使用标准的固件库控制IO*/
#define LED0(a)	if (a)	\
					GPIO_SetBits(LED0_GPIO_PORT,LED0_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED0_GPIO_PORT,LED0_GPIO_PIN)

#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define LED0_TOGGLE		 digitalToggle(LED0_GPIO_PORT,LED0_GPIO_PIN)
#define LED0_OFF		   digitalHi(LED0_GPIO_PORT,LED0_GPIO_PIN)
#define LED0_ON			   digitalLo(LED0_GPIO_PORT,LED0_GPIO_PIN)

#define LED1_TOGGLE		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON			   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

/* 基本混色，后面高级用法使用PWM可混出全彩颜色,且效果更好 */

//红
#define LED_RED  \
					LED0_ON;\
					LED1_OFF

//绿
#define LED_GREEN		\
					LED0_OFF;\
					LED1_ON

//蓝
#define LED_BLUE	\
					LED0_OFF;\
					LED1_OFF

					
//黄(红+绿)					
#define LED_YELLOW	\
					LED0_ON;\
					LED1_ON
//紫(红+蓝)
#define LED_PURPLE	\
					LED0_ON;\
					LED1_OFF

//青(绿+蓝)
#define LED_CYAN \
					LED0_OFF;\
					LED1_ON
					
//白(红+绿+蓝)
#define LED_WHITE	\
					LED0_ON;\
					LED1_ON
					
//黑(全部关闭)
#define LED_RGBOFF	\
					LED0_OFF;\
					LED1_OFF

void LED_GPIO_Config(void);

#endif /* __LED_H */
