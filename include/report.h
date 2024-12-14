#ifndef HTTPBENCH_REPORT_H
#define HTTPBENCH_REPORT_H

#include "stats.h"

// CSS样式模板
extern const char* CSS_TEMPLATE;

// JavaScript代码模板
extern const char* JS_TEMPLATE;

// HTML基础模板
extern const char* HTML_BASE_TEMPLATE;

// 报告生成相关函数
void write_html_report(const char* filename, struct ThreadStats* stats, 
                      int num_threads, double total_time);

// 辅助函数
char* create_response_times_string(const struct ThreadStats* stats);
char* get_current_time_string(void);
void calculate_statistics(const struct ThreadStats* stats, double total_time,
                        double* avg_response_time, double* qps, double* total_mb);

#endif // HTTPBENCH_REPORT_H