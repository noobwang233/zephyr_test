/*
 * @Author: tao wang
 * @Date: 2025-06-24 19:56:07
 * @LastEditors: tao wang
 * @LastEditTime: 2025-07-04 18:05:07
 * @Description: 
 * 简化版 Zephyr 线程 API Demo，
 * 不涉及中断、不使用定时器、不含线程通信，
 * 仅保留线程的基本创建、命名、挂起、恢复、优先级调整、终止和遍历操作，非常适合用于理解 Zephyr 的线程基本用法。
 * @FilePath: /zephyrproject/my-app/src/main.c
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include "thread_monitor.h"

#define STACKSIZE 1024
#define PRIORITY_HIGH 1
#define PRIORITY_MEDIUM 2
#define PRIORITY_LOW 3

K_THREAD_STACK_DEFINE(high_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(medium_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(low_stack, STACKSIZE);

struct k_thread high_thread_data;
struct k_thread medium_thread_data;
struct k_thread low_thread_data;

/* 高优先级线程：启动后会抢占其他线程 */
void high_priority_thread(void *a, void *b, void *c)
{
    while (1) {
        printk("[HIGH] Executing high priority task\n");
        k_sleep(K_MSEC(600));
    }
}

/* 中优先级线程：定期让出 CPU */
void medium_priority_thread(void *a, void *b, void *c)
{
    while (1) {
        printk("[MEDIUM] Working, then yield\n");
        k_sleep(K_MSEC(200));
        k_yield();  // 主动让出 CPU（会调度到低优线程）
        k_sleep(K_MSEC(200));
    }
}

/* 低优先级线程：只在高/中优线程不活跃时运行 */
void low_priority_thread(void *a, void *b, void *c)
{
    while (1) {
        printk("[low] Doing background work...\n");
        k_sleep(K_MSEC(100));
    }
}


int main(void)
{
    printk("=== Zephyr Thread API Demo (Simplified) ===\n");

    /* 创建线程，观察调度顺序 */
    k_tid_t tid1 = k_thread_create(&low_thread_data, low_stack, STACKSIZE,
                                    low_priority_thread, NULL, NULL, NULL,
                                    PRIORITY_LOW, 0, K_NO_WAIT);
    k_thread_name_set(tid1, "low");

    k_tid_t tid2 = k_thread_create(&medium_thread_data, medium_stack, STACKSIZE,
                                    medium_priority_thread, NULL, NULL, NULL,
                                    PRIORITY_MEDIUM, 0, K_NO_WAIT);
    k_thread_name_set(tid2, "medium");

    k_tid_t tid3 = k_thread_create(&high_thread_data, high_stack, STACKSIZE,
                                    high_priority_thread, NULL, NULL, NULL,
                                    PRIORITY_HIGH, 0, K_NO_WAIT);
    k_thread_name_set(tid3, "high");

    /* 遍历所有线程 */
    while (1) {
        printk("\n== Current Threads ==\n");
        print_all_threads();
        k_sleep(K_MSEC(100));
    }

    return 0;
}