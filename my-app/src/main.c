/*
 * @Author: tao wang
 * @Date: 2025-06-24 19:56:07
 * @LastEditors: tao wang
 * @LastEditTime: 2025-07-07 17:54:42
 * @Description: 
 * 简化版 Zephyr 中断 API Demo，
 * @FilePath: /zephyrproject/my-app/src/main.c
 */
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define DEMO_IRQ_LINE 20  // 模拟中断号（可选 16 ~ 239 之间的未占用号）

static struct k_work demo_work;


/* 工作项处理函数（线程上下文） */
void demo_work_handler(struct k_work *work)
{
    printk(">>> demo_work_handler!\n");
}

/* 中断服务程序（ISR 上下文） */
void demo_isr1(const void *arg)
{
    ARG_UNUSED(arg);
    printk(">>> ISR 1 triggered (IRQ %d)\n", DEMO_IRQ_LINE);
    k_work_submit(&demo_work);
}


/* 中断服务程序（ISR 上下文） */
void demo_isr2(const void *arg)
{
    ARG_UNUSED(arg);
    printk(">>> ISR 2 triggered (IRQ %d)\n", DEMO_IRQ_LINE);
    k_work_submit(&demo_work);
}

/* 直接中断 */
ISR_DIRECT_DECLARE(direct_isr)
{
    printk(">>> DIRECT ISR triggered (IRQ %d)\n", DEMO_IRQ_LINE);
   /* PM done after servicing interrupt for best latency. This cannot be
   used for zero-latency IRQs because it accesses kernel data. */
   ISR_DIRECT_PM();
   /* Ask the kernel to check if scheduling decision should be made. If the
   ISR is for a zero-latency IRQ then the return value must always be 0. */
   return 1;
}


int main(void)
{
    printk("== Zephyr Software IRQ Demo (No peripheral) ==\n");

#if 0
    IRQ_CONNECT(DEMO_IRQ_LINE, 0, demo_isr, NULL, 0);
#endif

#if 1
    /* 注册动态中断处理程序 */
    irq_connect_dynamic(DEMO_IRQ_LINE, 0, demo_isr1, NULL, 0);
    irq_connect_dynamic(DEMO_IRQ_LINE, 0, demo_isr2, NULL, 0);
#endif

#if 0
    IRQ_DIRECT_CONNECT(DEMO_IRQ_LINE, 0, direct_isr, 0);
#endif

    irq_enable(DEMO_IRQ_LINE);

    printk("Registered dynamic IRQ handler on line %u\n", DEMO_IRQ_LINE);

    k_work_init(&demo_work, demo_work_handler);

    while (1) {
        printk("Triggering IRQ %d from main thread...\n", DEMO_IRQ_LINE);
        /* 手动设置挂起（触发中断） */
        NVIC_SetPendingIRQ(DEMO_IRQ_LINE);  // ARM Cortex-M 方式
        k_sleep(K_SECONDS(2));

    }

    return 0;
}