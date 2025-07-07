/*
 * @Author: tao wang
 * @Date: 2025-07-04 17:44:55
 * @LastEditors: tao wang
 * @LastEditTime: 2025-07-04 17:55:52
 * @Description: 检测线程状态
 * @FilePath: /zephyrproject/my-app/src/thread_monitor.c
 */
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <string.h>


// 自定义状态宏（避免与 Zephyr 内部冲突）
#ifndef MY_THREAD_DUMMY
#define MY_THREAD_DUMMY      0x01
#define MY_THREAD_PENDING    0x02
#define MY_THREAD_PRESTART   0x04
#define MY_THREAD_DEAD       0x08
#define MY_THREAD_SUSPENDED  0x10
#define MY_THREAD_ABORTING   0x20
#define MY_THREAD_QUEUED     0x40
#define MY_THREAD_STARTED    0x80
#endif

const char *thread_state_to_str(uint8_t state)
{
    if (state == 0) return "READY";

    static char buf[64];
    buf[0] = '\0';

    if (state & MY_THREAD_DEAD) strcat(buf, "DEAD ");
    if (state & MY_THREAD_SUSPENDED) strcat(buf, "SUSPENDED ");
    if (state & MY_THREAD_PENDING) strcat(buf, "PENDING ");
    if (state & MY_THREAD_PRESTART) strcat(buf, "PRESTART ");
    if (state & MY_THREAD_ABORTING) strcat(buf, "ABORTING ");
    if (state & MY_THREAD_DUMMY) strcat(buf, "DUMMY ");
    if (state & MY_THREAD_QUEUED) strcat(buf, "QUEUED ");
    if (state & MY_THREAD_STARTED) strcat(buf, "STARTED ");

    return buf;
}

void thread_callback(const struct k_thread *thread, void *user_data)
{
    const char *name = k_thread_name_get((k_tid_t)thread);
    if (!name)
        name = "<unnamed>";

    int prio = k_thread_priority_get((k_tid_t)thread);
    const char *state_str = thread_state_to_str(thread->base.thread_state);

    printk("Thread: %-12s | Prio: %2d | State: %s\n", name, prio, state_str);

}

void print_all_threads(void)
{
    printk("\n== Thread Monitor ==\n");
    k_thread_foreach(thread_callback, NULL);
}