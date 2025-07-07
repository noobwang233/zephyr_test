/*
 * @Author: tao wang
 * @Date: 2025-07-04 17:47:19
 * @LastEditors: tao wang
 * @LastEditTime: 2025-07-04 17:49:25
 * @Description: 线程检测头文件
 * @FilePath: /zephyrproject/my-app/src/thread_monitor.h
 */
#ifndef THREAD_MONITOR_H
#define THREAD_MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @brief 打印系统中所有线程的信息（名字、优先级、状态、栈剩余空间）
 */
void print_all_threads(void);

#ifdef __cplusplus
}
#endif

#endif /* THREAD_MONITOR_H */