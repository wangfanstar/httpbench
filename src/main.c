#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "../include/config.h"
#include "../include/worker.h"
#include "../include/stats.h"
#include "../include/report.h"

int main(int argc, char *argv[]) {
    // 初始化配置
    init_default_config();
    if (parse_arguments(argc, argv) != 0) {
        return 1;
    }

    // 创建线程和统计数据
    pthread_t* threads = malloc(g_config.num_threads * sizeof(pthread_t));
    struct ThreadStats* thread_stats = malloc(g_config.num_threads * sizeof(struct ThreadStats));
    struct WorkerArgs* worker_args = malloc(g_config.num_threads * sizeof(struct WorkerArgs));

    // 初始化统计数据
    for (int i = 0; i < g_config.num_threads; i++) {
        init_thread_stats(&thread_stats[i], g_config.requests_per_thread);
        worker_args[i].thread_id = i;
        worker_args[i].stats = &thread_stats[i];
    }

    // 记录开始时间
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // 启动工作线程
    for (int i = 0; i < g_config.num_threads; i++) {
        pthread_create(&threads[i], NULL, worker_thread, &worker_args[i]);
    }

    // 等待所有线程完成
    for (int i = 0; i < g_config.num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // 记录结束时间
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double total_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                       (end_time.tv_nsec - start_time.tv_nsec) / 1000000.0;

    // 生成报告
    char report_filename[128];
    time_t now = time(NULL);
    strftime(report_filename, sizeof(report_filename), 
             "report-%Y%m%d-%H%M%S.html", localtime(&now));
    write_html_report(report_filename, thread_stats, g_config.num_threads, total_time);

    // 清理资源
    for (int i = 0; i < g_config.num_threads; i++) {
        free_thread_stats(&thread_stats[i]);
    }
    free(threads);
    free(thread_stats);
    free(worker_args);

    printf("测试完成！报告已生成：%s\n", report_filename);
    return 0;
}