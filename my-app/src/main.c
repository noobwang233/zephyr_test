/*
 * @Author: tao wang
 * @Date: 2025-06-24 19:56:07
 * @LastEditors: tao wang
 * @LastEditTime: 2025-07-04 16:13:17
 * @Description: 
 * 简化版 Zephyr 线程 API Demo，
 * 不涉及中断、不使用定时器、不含线程通信，
 * 仅保留线程的基本创建、命名、挂起、恢复、优先级调整、终止和遍历操作，非常适合用于理解 Zephyr 的线程基本用法。
 * @FilePath: /zephyrproject/my-app/src/main.c
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* 线程栈大小 */
#define STACKSIZE 1024

/* 线程优先级 */
#define PRIORITY_WORKER 2
#define PRIORITY_HELPER 2

/* 定义线程栈, 必须在全局域定义 */
K_THREAD_STACK_DEFINE(worker_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(helper_stack, STACKSIZE);

/* 线程控制块 */
struct k_thread worker_thread_data;
struct k_thread helper_thread_data;

/* 线程函数 */
void worker_thread(void *a, void *b, void *c)
{
    while (1) {
        printk("[Worker] Running...\n");
        k_sleep(K_SECONDS(1));
    }
}

void helper_thread(void *a, void *b, void *c)
{
    while (1) {
        printk("[Helper] Assisting...\n");
        k_sleep(K_SECONDS(2));
    }
}

/* 遍历线程的回调函数 */
void print_thread_info(const struct k_thread *thread, void *user_data)
{
    const char *name = k_thread_name_get((k_tid_t)thread);
    if (!name)
        name = "<unnamed>";

    printk("Thread: %s | Priority: %d | State: 0x%x\n",
           name,
           k_thread_priority_get((k_tid_t)thread),
           thread->base.thread_state);
}

int main(void)
{
    printk("=== Zephyr Thread API Demo (Simplified) ===\n");

    /* 创建线程 */
    k_tid_t tid_worker = k_thread_create(&worker_thread_data, worker_stack, STACKSIZE,
        worker_thread, NULL, NULL, NULL,
        PRIORITY_WORKER, 0, K_NO_WAIT);
    k_thread_name_set(tid_worker, "worker");

    k_tid_t tid_helper = k_thread_create(&helper_thread_data, helper_stack, STACKSIZE,
            helper_thread, NULL, NULL, NULL,
            PRIORITY_HELPER, 0, K_NO_WAIT);
    k_thread_name_set(tid_helper, "helper");

    /* 运行一段时间 */
    k_sleep(K_SECONDS(3));

    /* 挂起 worker */
    printk("Suspending worker thread...\n");
    k_thread_suspend(tid_worker);
    k_sleep(K_SECONDS(2));

    /* 恢复 worker */
    printk("Resuming worker thread...\n");
    k_thread_resume(tid_worker);
    k_sleep(K_SECONDS(2));

    /* 提高 worker 优先级 */
    printk("Raising worker thread priority...\n");
    k_thread_priority_set(tid_worker, PRIORITY_WORKER - 1);
    k_sleep(K_SECONDS(1));

    /* 遍历所有线程 */
    printk("\n== Current Threads ==\n");
    k_thread_foreach(print_thread_info, NULL);

    /* 停止 helper */
    printk("Aborting helper thread...\n");
    k_thread_abort(tid_helper);

    printk("Demo complete.\n");

    return 0;
}