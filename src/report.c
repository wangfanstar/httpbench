#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/report.h"

// HTML报告模板
static const char* HTML_TEMPLATE = R"(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>HTTP性能测试报告</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <script src="https://cdn.plot.ly/plotly-2.24.1.min.js"></script>
</head>
<body class="bg-gray-100 p-8">
    <div class="max-w-4xl mx-auto bg-white rounded-lg shadow-lg p-8">
        <h1 class="text-3xl font-bold mb-6">HTTP性能测试报告</h1>
        <div class="grid grid-cols-2 gap-4 mb-8">
            <div class="bg-gray-50 p-4 rounded">
                <h2 class="text-lg font-semibold mb-2">测试配置</h2>
                <p>服务器: %s:%d</p>
                <p>线程数: %d</p>
                <p>每线程请求数: %d</p>
            </div>
            <div class="bg-gray-50 p-4 rounded">
                <h2 class="text-lg font-semibold mb-2">测试结果</h2>
                <p>总请求数: %d</p>
                <p>成功请求数: %d</p>
                <p>总传输量: %.2f MB</p>
                <p>平均响应时间: %.2f ms</p>
                <p>QPS: %.2f</p>
            </div>
        </div>
        <div class="mb-8">
            <h2 class="text-lg font-semibold mb-4">响应时间分布</h2>
            <div id="responseTimeChart" style="height: 400px;"></div>
        </div>
    </div>
    <script>
        const responseTimesData = [%s];
        const trace = {
            x: responseTimesData,
            type: 'histogram',
            nbinsx: 50,
            marker: {
                color: 'rgb(59, 130, 246)',
            },
        };
        const layout = {
            title: '响应时间分布图',
            xaxis: {title: '响应时间 (ms)'},
            yaxis: {title: '请求数量'},
            bargap: 0.05,
        };
        Plotly.newPlot('responseTimeChart', [trace], layout);
    </script>
</body>
</html>
)";

void write_html_report(const char* filename, struct ThreadStats* stats, 
                      int num_threads, double total_time) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "无法创建报告文件\n");
        return;
    }

    // 准备响应时间数据字符串
    char* response_times_str = malloc(stats->response_time_count * 20);
    response_times_str[0] = '\0';
    for (int i = 0; i < stats->response_time_count; i++) {
        char temp[20];
        snprintf(temp, sizeof(temp), "%.2f%s", 
                stats->response_times[i],
                i < stats->response_time_count - 1 ? "," : "");
        strcat(response_times_str, temp);
    }

    // 计算统计数据
    double avg_response_time = stats->total_time / stats->total_requests;
    double qps = stats->total_requests / (total_time / 1000.0);
    double total_mb = stats->total_bytes / (1024.0 * 1024.0);

    // 写入报告
    fprintf(fp, HTML_TEMPLATE,
            g_config.server_ip,
            g_config.server_port,
            g_config.num_threads,
            g_config.requests_per_thread,
            stats->total_requests,
            stats->successful_requests,
            total_mb,
            avg_response_time,
            qps,
            response_times_str);

    free(response_times_str);
    fclose(fp);
}