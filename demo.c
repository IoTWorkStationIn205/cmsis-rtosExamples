#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/**
 * @brief 线程       0 
 *        信号量     1
 *        消息队列   2
 *        软件定时器 3
 *        内存池     4
 *         
 */
#define CHOOSE 0

#if CHOOSE == 0

/**
 * @brief 函数结构体
 * 
 */
osThreadId_t testTaskHandle;
const osThreadAttr_t testTask_attributes = {
  .name = "testTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void StartTestTask(void *argument);

/**
 * @brief 初始化函数
 * 
 */
void Init(void){
    testTaskHandle = osThreadNew(StartTestTask, NULL, &testTask_attributes);
}

/**
 * @brief Test线程函数
 * 
 * @param argument 
 */
void StartTestTask(void *argument){
    int counter = 0;
    for(;;){
        printf("counter = %d",counter++);
    osDelay(500);
    }
}
#elif CHOOSE == 1
/**
 * @brief 信号量测试
 * 
 */

/**
 * @brief 线程定义
 * 
 */
osThreadId_t test1TaskHandle;
const osThreadAttr_t test1Task_attributes = {
  .name = "test1Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
osThreadId_t test2TaskHandle;
const osThreadAttr_t test2Task_attributes = {
  .name = "test2Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};

/**
 * @brief 信号量定义
 * 
 */
osSemaphoreId_t myBinarySem01Handle;
const osSemaphoreAttr_t myBinarySem01_attributes = {
  .name = "myBinarySem01"
};

void StartTest1Task(void *argument);
void StartTest2Task(void *argument);

/**
 * @brief 初始化函数
 * 
 */
void Init(void){
    printf("信号量测试！\n");
    
    test1TaskHandle = osThreadNew(StartTest1Task, NULL, &test1Task_attributes);
    test2TaskHandle = osThreadNew(StartTest2Task, NULL, &test2Task_attributes);
    
    myBinarySem01Handle = osSemaphoreNew(1,1,&myBinarySem01_attributes);
}

/**
 * @brief 测试1线程函数
 * 
 * @param argument 
 */
void StartTest1Task(void *argument){
    int counter = 0;
    for(;;){
        osSemaphoreAcquire(myBinarySem01Handle,osWaitForever);
        HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
        printf("counter = %d",counter++);
    }
}

/**
 * @brief 测试2线程函数
 * 
 * @param argument 
 */
void StartTest2Task(void *argument){
    for(;;){
        osSemaphoreRelease(myBinarySem01Handle);
    osDelay(1000);
    }
}
#elif CHOOSE == 2
/**
 * @brief 队列示例
 * 
 */


/**
 * @brief 线程定义
 * 
 */
osThreadId_t test1TaskHandle;
const osThreadAttr_t test1Task_attributes = {
  .name = "test1Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
osThreadId_t test2TaskHandle;
const osThreadAttr_t test2Task_attributes = {
  .name = "test2Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};

/**
 * @brief 消息队列结构体定义
 * 
 */
osMessageQueueId_t testMsgQueueHandle;
const osMessageQueueAttr_t testQueue = {
  .name = "myBinarySem01"
};

void StartTest1Task(void *argument);
void StartTest2Task(void *argument);

/**
 * @brief 初始化函数
 * 
 */
void Init(void){
    printf("消息队列测试测试!\n");
    
    test1TaskHandle = osThreadNew(StartTest1Task, NULL, &test1Task_attributes);
    test2TaskHandle = osThreadNew(StartTest2Task, NULL, &test2Task_attributes);
    
    testMsgQueueHandle = osMessageQueueNew(16,32,&testQueue);
    if(testMsgQueueHandle == NULL){
        Error_Handler();
    }
}

/**
 * @brief 测试1线程函数
 * 
 * @param argument 
 */
void StartTest1Task(void *argument){
    int counter = 0;
    for(;;){
        osMessageQueuePut(testMsgQueueHandle, &counter, 0U, 0U);
        counter++;
        osDelay(1000);
    }
}

/**
 * @brief 测试2线程函数
 * 
 * @param argument 
 */
void StartTest2Task(void *argument){
    
    int data = 0;
    for(;;){
    osMessageQueueGet(testMsgQueueHandle, &data, NULL, osWaitForever);
        printf("counter = %d\n",data);
    }
}

#elif CHOOSE == 3
/**
 * @brief 软件定时器示例
 * 
 */


/**
 * @brief 定时器结构体定义
 * 
 */
osTimerId_t test1TimerHandle;
const osTimerAttr_t test1Timer = {
  .name = "myTimer01"
};

osTimerId_t test2TimerHandle;
const osTimerAttr_t tes2Timer = {
  .name = "myTimer02"
};

void Timer1_Callback(void *argument);
void Timer2_Callback(void *argument);

/**
 * @brief 初始化函数
 * 
 */
void Init(void){
    printf("软件定时器测试!\n");
    
    test1TimerHandle = osTimerNew(Timer1_Callback,osTimerOnce,NULL,NULL);
    if(test1TimerHandle == NULL){
        Error_Handler();
    }
    test2TimerHandle = osTimerNew(Timer2_Callback,osTimerPeriodic,NULL,NULL);
    if(test2TimerHandle == NULL){
        Error_Handler();
    }
    
    /* 开启定时器 */
    osTimerStart(test1TimerHandle, 1000U);
    osTimerStart(test2TimerHandle, 2000U);
}

/**
 * @brief 定时器1回调函数
 * 
 * @param argument 
 */
void Timer1_Callback(void *argument){
    printf("一次性定时器结束!\n");
}

/**
 * @brief 定时器2回调函数
 * 
 * @param argument 
 */
void Timer2_Callback(void *argument){
        printf("定时器2定时到!\n");
}

#elif CHOOSE == 4
/**
 * @brief 内存池示例
 * 
 */

typedef struct {
    uint8_t Buf[32];
    uint8_t Id;
}test;

/**
 * @brief 内存池结构体定义
 * 
 */
osMemoryPoolId_t testPoolHandle;
const osMemoryPoolAttr_t testPool_attr = {
  .name = "myPool01"
};

/**
 * @brief 线程结构体定义
 * 
 */
osThreadId_t testTaskHandle;
const osThreadAttr_t testTask_attributes = {
  .name = "testTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void StartTest1Task(void *argument);

/**
 * @brief 初始化函数
 * 
 */
void Init(void){
    printf("软件定时器测试!\n");
    
    /* 创建内存池 */
    testPoolHandle = osMemoryPoolNew(16,sizeof(test),&testPool_attr);
    
    /* 创建线程 */
    testTaskHandle = osThreadNew(StartTest1Task,NULL,&testTask_attributes);
    if(testTaskHandle == NULL){
        Error_Handler();
    }
    
}

/**
 * @brief 测试线程函数
 * 
 * @param argument 
 */
void StartTest1Task(void *argument){
    osStatus_t status;
    test *Pom;
    
    int counter = 1;
    for(;;){
        Pom = (test*)osMemoryPoolAlloc(testPoolHandle,0U);
        if(!NULL){
            Pom->Buf[0] = counter * 10;
            Pom->Id = counter++;
        }
        printf("<创建成功>ID%d = %d\n",Pom->Id,Pom->Buf[0]);
        
        status = osMemoryPoolFree(testPoolHandle,Pom);
        switch (status){
            case osOK:
                printf("<释放成功>ID%d 当前剩余内存块:%d\n",Pom->Id,osMemoryPoolGetSpace(testPoolHandle));
                break;
            case osErrorParameter:
                
                break;
            case osErrorNoMemory:
                break;
            default:
                break;
        }
        osDelay(1000);
    }
}

#endif

