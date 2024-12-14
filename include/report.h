#ifndef HTTPBENCH_REPORT_H
#define HTTPBENCH_REPORT_H

#include "stats.h"

// 函数声明
void write_html_report(const char* filename, struct ThreadStats* thread_stats, 
                      int num_threads, double total_time);

#endif // HTTPBENCH_REPORT_H