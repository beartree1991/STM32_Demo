/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/*
******************************************************************************
*                          包含的头文件 
******************************************************************************
*/
#pragma  diag_suppress 870

#include "FreeRTOS.h"
#include "task.h"
#include "../User/bsp/led/bsp_led.h"
#include "../User/bsp/usart/bsp_usart.h"

/*************************** 任务句柄 ****************************************/
/*
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL.
 */
/* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle;
/* LED 任务句柄 */
static TaskHandle_t LED_Task_Handle;

/*************************** 内核对象句柄 ************************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 * 
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务见的通信，
 * 任务见的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */

/************************** 全局变量声明 ************************************/
/*
 * 当我们再写应用程序的时候，可能需要用到一些全局变量。
 */
/* AppTaskCreate 任务堆栈 */
static StackType_t AppTaskCreate_Stack[128];
/* LED 任务堆栈 */
static StackType_t LED_Task_Stack[128];

/* AppTaskCreate 任务控制块 */
static StaticTask_t AppTaskCreate_TCB;
/* AppTaskCreate 任务控制块 */
static StaticTask_t LED_Task_TCB;

/* 空闲任务堆栈 */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* 定时器任务堆栈 */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/* 空闲任务控制块 */
static StaticTask_t Idle_Task_TCB;
/* 定时器任务控制块 */
static StaticTask_t Timer_Task_TCB;

/*
******************************************************************************
*                             函数声明 
******************************************************************************
*/
static void AppTaskCreate(void);  /* 用于创建任务 */
static void LED_Task(void* pvParameters);  /* LED_Task 任务实现 */
static void BSP_Init(void);  /* 用于初始化板载相关资源 */

/*
 * 使用了静态分配内存，以下这两个函数是由用户实现，函数在 task.c 文件中有引用
 * 当且仅当 configSUPPORT_STATIC_ALLOCATION 这个宏定义为 1 的时候才有效
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t  **ppxTimerTaskStackBuffer,
                                    uint32_t     *pulTimerTaskStackSize);

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t  **ppxIdleTaskStackBuffer,
                                   uint32_t     *pulIdleTaskStackSize);

/****************************************************************************
 * @brief  主函数
 * @param  无
 * @retval 无
 * @note   第一步：开发板硬件初始化
 *         第二步：创建 APP 应用任务
 *         第三步：启动 FreeRTOS，开始多任务调度
 ***************************************************************************/
int main(void)
{
  /* 开发板硬件初始化 */
  BSP_Init();
  printf("FreeRTOS 静态创建任务!\r\n");
  /* 创建 AppTaskCreate 任务 */
  AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t)AppTaskCreate,
                                           (const char *  )"AppTaskCreate", //任务名称
                                           (uint32_t      )128,             //任务堆栈大小
                                           (void *        )NULL,            //传递给任务函数的参数
                                           (UBaseType_t   )3,               //任务优先级
                                           (StackType_t * )AppTaskCreate_Stack,
                                           (StaticTask_t *)&AppTaskCreate_TCB);
  if(NULL != AppTaskCreate_Handle)  /* 创建成功 */
    vTaskStartScheduler();          /* 启动任务，开启调用 */

  while(1);                         /* 正常不会执行到这里 */
}

/****************************************************************************
 * @brief   AppTaskCreate，为了方便管理，所有的任务创建函数都放在这个函数里面
 * @param   无
 * @retval  无
 * @note    无
 ***************************************************************************/
static void AppTaskCreate(void)
{
  taskENTER_CRITICAL();            //进入临界区
  
  /* 创建 LED_Task 任务 */
  LED_Task_Handle = xTaskCreateStatic((TaskFunction_t)LED_Task,    //任务函数
                                      (const char *)"LED Task",    //任务名称
                                      (uint32_t)128,               //任务栈大小
                                      (void *)NULL,                //传递给任务函数的参数
                                      (UBaseType_t)4,              //任务优先级
                                      (StackType_t *)LED_Task_Stack,//任务堆栈
                                      (StaticTask_t *)&LED_Task_TCB); //任务控制块
  
  if(NULL != LED_Task_Handle)  /* 创建成功 */
    printf("LED_Task 任务创建成功！\n");
  else
    printf("LED_Task 任务创建失败！\n");

  vTaskDelete(AppTaskCreate_Handle);  //删除 AppTaskCreate 任务
  taskEXIT_CRITICAL();                //退出临界区
}

/**************************************************************************
 * @brief  LED0_Task 任务主体
 * @param  无
 * @retval 无
 * @note   无
 *************************************************************************/
static void LED_Task(void *parameter)
{
  while(1){
    LED0_ON;
    vTaskDelay(1500);  /* 延时 500 个tick */
    printf("led0_task running, LED0_ON\r\n");

    LED0_OFF;
    vTaskDelay(500);  /* 延时 500 个tick */
    printf("led0_task running, LED0_OFF\r\n");
  }
}

// /**************************************************************************
//  * @brief  LED1_Task 任务主体
//  * @param  无
//  * @retval 无
//  * @note   无
//  *************************************************************************/
// static void LED1_Task(void *parameter)
// {
//   while(1){
//     LED1_ON;
//     vTaskDelay(1000);  /* 延时 500 个tick */
//     printf("led1_task running, LED1_ON\r\n");

//     LED1_OFF;
//     vTaskDelay(1000);  /* 延时 500 个tick */
//     printf("led1_task running, LED1_OFF\r\n");
//   }
// }

/******************************************************************
 * @brief:    板级外设初始化，所有板子上的初始化均可放在这个函数里面
 * @param:    无
 * @retval:   无
 * @note:     无
 *****************************************************************/
static void BSP_Init(void)
{
  /*
   * STM32中断优先级分组为4，即 4bit 都用来表示抢占优先级，范围为：0~15
   * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
   * 都统一用这个优先级分组，千万不要再分组，切记。
   */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* LED 初始化 */
  LED_GPIO_Config();

  /* 串口初始化 */
  USART_Config();
}

/******************************************************************
 * @brief : 获取空闲任务的任务堆栈和任务控制块内存
 * @param : ppxTimerTaskTCBBuffer  : 任务控制块内存
 *          ppxTimerTaskStackBuffer: 任务堆栈内存
 *          pulTimerTaskStackSize  : 任务堆栈大小
 * @retval: 无
 * @note  : 无
 ******************************************************************/
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t  **ppxIdleTaskStackBuffer,
                                   uint32_t     *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &Idle_Task_TCB;     /* 任务控制块内存 */
  *ppxIdleTaskStackBuffer = Idle_Task_Stack;  /* 任务堆栈内存 */
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;  /* 任务堆栈大小 */
}

/**********************************************************************************
 * @brief   获取定时器任务的任务堆栈和任务控制块内存
 * @param   ppxTimerTaskTCBBuffer   : 任务控制块内存
 *          ppxTimerTaskStackBuffer : 任务堆栈内存
 *          ppxTimerTaskStackSize   : 任务堆栈大小
 * @retval  无
 * @note    无
 **********************************************************************************/
void vApplicationGetTimerTaskMemory(StaticTask_t  **ppxTimerTaskTCBBuffer,
                                    StackType_t   **ppxTimerTaskStackBuffer,
                                    uint32_t      *pulTimerTaskStackSize)
{
  *ppxTimerTaskTCBBuffer = &Timer_Task_TCB;    /* 任务控制块内存 */
  *ppxTimerTaskStackBuffer = Timer_Task_Stack; /* 任务堆栈内存   */
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH; /* 任务堆栈大小 */
}
