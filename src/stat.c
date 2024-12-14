#include <stdlib.h>
#include <string.h>
#include "../include/stats.h"

void init_thread_stats(struct ThreadStats* stats, int max_requests) {
    stats->total_time = 0;
    stats->total_requests = 0;
    stats->successful_requests = 0;
    stats->total_bytes = 0;
    stats->response_times = malloc(max_requests * sizeof(double));
    stats->response_time_count = 0;
}

void free_thread_stats(struct ThreadStats* stats) {
    if (stats->response_times) {
        free(stats->response_times);
        stats->response_times = NULL;
    }
}

void record_response(struct ThreadStats* stats, double response_time, long bytes) {
    stats->response_times[stats->response_time_count++] = response_time;
    stats->total_time += response_time;
    stats->total_requests++;
    stats->successful_requests++;
    stats->total_bytes += bytes;
}

void merge_thread_stats(struct ThreadStats* total, struct ThreadStats* thread_stats, int num_threads) {
    total->total_time = 0;
    total->total_requests = 0;
    total->successful_requests = 0;
    total->total_bytes = 0;
    total->response_time_count = 0;

    // 计算所需的总响应时间数组大小
    int total_responses = 0;
    for (int i = 0; i < num_threads; i++) {
        total_responses += thread_stats[i].response_time_count;
    }

    // 分配内存
    total->response_times = malloc(total_responses * sizeof(double));

    // 合并统计数据
    for (int i = 0; i < num_threads; i++) {
        total->total_requests += thread_stats[i].total_requests;
        total->successful_requests += thread_stats[i].successful_requests;
        total->total_bytes += thread_stats[i].total_bytes;
        
        // 合并响应时间数组
        memcpy(total->response_times + total->response_time_count,
               thread_stats[i].response_times,
               thread_stats[i].response_time_count * sizeof(double));
        total->response_time_count += thread_stats[i].response_time_count;
    }
}