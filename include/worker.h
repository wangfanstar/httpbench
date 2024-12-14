#ifndef HTTPBENCH_WORKER_H
#define HTTPBENCH_WORKER_H

#include "stats.h"

// 工作线程参数结构体
struct WorkerArgs {
    int thread_id;              // 线程ID
    struct ThreadStats* stats;  // 统计数据
};

// 函数声明
void* worker_thread(void* arg);
int send_request(const char* ip, int port, struct ThreadStats* stats);

#endif // HTTPBENCH_WORKER_H