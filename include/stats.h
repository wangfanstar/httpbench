#ifndef HTTPBENCH_STATS_H
#define HTTPBENCH_STATS_H

// 线程统计结构体
struct ThreadStats {
    long total_time;           // 总耗时（毫秒）
    int total_requests;        // 总请求数
    int successful_requests;   // 成功请求数
    long total_bytes;          // 总传输字节数
    double* response_times;    // 响应时间数组
    int response_time_count;   // 响应时间计数
};

// 函数声明
void init_thread_stats(struct ThreadStats* stats, int max_requests);
void free_thread_stats(struct ThreadStats* stats);
void record_response(struct ThreadStats* stats, double response_time, long bytes);
void merge_thread_stats(struct ThreadStats* total, struct ThreadStats* thread_stats, int num_threads);

#endif // HTTPBENCH_STATS_H