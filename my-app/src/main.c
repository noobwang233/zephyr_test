/*
 * @Author: tao wang
 * @Date: 2025-06-24 19:56:07
 * @LastEditors: tao wang
 * @LastEditTime: 2025-07-04 15:36:21
 * @Description: 模块描述
 * @FilePath: /zephyrproject/my-app/src/main.c
 */

#include <stdio.h>
#include <zephyr/kernel.h>

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7


void thread_task(uint32_t sleep_ms, uint32_t id)
{
    int cnt = 0;
    while (1) {
        printf("thread %d running, cnt = %d\n", id, cnt);
        k_msleep(sleep_ms);
        cnt++;
    }
}

void thread1(void *p1, void *p2, void *p3)
{
    thread_task(160, 1);
}

void thread2(void *p1, void *p2, void *p3)
{
    thread_task(1000, 2);
}

/****************************************************************
 *           1. 使用 k_thread_create 创建线程                   *
 ****************************************************************/
#if 0

/* 定义线程栈, 必须在全局域定义 */
K_THREAD_STACK_DEFINE(stack_area1, STACKSIZE);
K_THREAD_STACK_DEFINE(stack_area2, STACKSIZE);

int main(void)
{
    /* 定义线程结构体变量 */
    struct k_thread thread_data1;
    struct k_thread thread_data2;
    /* 定义线程 id 变量 */
    k_tid_t my_tid1;
    k_tid_t my_tid2;

    printf("Main thread start\n");

    /* 创建线程 */
    my_tid1 = k_thread_create(&thread_data1,                         /* 未初始化的线程结构体指针 */
                              stack_area1,                           /* 堆栈空间的指针 */
                              K_THREAD_STACK_SIZEOF(stack_area1),    /* 堆栈大小（以字节为单位） */
                              thread1,                               /* 线程入口函数 */
                              NULL, NULL, NULL,                      /* 传入给线程入口函数的3个参数 */
                              PRIORITY,                              /* 线程优先级 */
                              0,                                     /* 线程选项Flag */
                              K_NO_WAIT);                            /* 启动的调度延迟，K_NO_WAIT（无延迟） */

    my_tid2 = k_thread_create(&thread_data2,
                              stack_area2,
                              K_THREAD_STACK_SIZEOF(stack_area2),
                              thread2,
                              NULL, NULL, NULL,
                              PRIORITY,
                              0,
                              K_NO_WAIT);
    printf("Main thread end\n");

    return 0;
}
#endif

/****************************************************************
 *           2. 使用 K_THREAD_DEFINE 创建线程                   *
 ****************************************************************/
#if 1
K_THREAD_DEFINE(thread1_id,         /* 线程id */
                STACKSIZE,          /* 线程栈大小 */
                thread1,            /* 线程入口函数 */
                NULL, NULL, NULL,   /* 传入给线程入口函数的3个参数 */
                PRIORITY,           /* 线程优先级 */
                0,                  /* 线程选项Flag */
                0);                 /* 调度延迟（以毫秒为单位），零表示无延迟 */
K_THREAD_DEFINE(thread2_id, STACKSIZE, thread2, NULL, NULL, NULL,
    PRIORITY, 0, 0);

#endif