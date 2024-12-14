#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/report.h"
#include "../include/config.h"

// CSS样式模板
const char* CSS_TEMPLATE = R"(
    /* 基础样式 */
    :root {
        --primary-color: rgb(59, 130, 246);
        --bg-color: rgb(243, 244, 246);
        --card-bg: rgb(249, 250, 251);
        --text-color: rgb(17, 24, 39);
    }
    
    * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
    }
    
    body {
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
        background-color: var(--bg-color);
        color: var(--text-color);
        line-height: 1.5;
        padding: 2rem;
    }
    
    .container {
        max-width: 1200px;
        margin: 0 auto;
        background: white;
        border-radius: 0.5rem;
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        padding: 2rem;
    }
    
    h1 {
        font-size: 1.875rem;
        font-weight: bold;
        margin-bottom: 1.5rem;
        color: var(--text-color);
    }
    
    .grid {
        display: grid;
        grid-template-columns: repeat(2, 1fr);
        gap: 1rem;
        margin-bottom: 2rem;
    }
    
    .card {
        background-color: var(--card-bg);
        padding: 1rem;
        border-radius: 0.375rem;
    }
    
    h2 {
        font-size: 1.25rem;
        font-weight: 600;
        margin-bottom: 0.5rem;
        color: var(--text-color);
    }
    
    p {
        margin-bottom: 0.5rem;
        color: var(--text-color);
    }
    
    .chart-container {
        margin-top: 2rem;
        height: 400px;
        background: white;
        border-radius: 0.375rem;
        padding: 1rem;
    }
)";

// JavaScript代码模板
const char* JS_TEMPLATE = R"(
    // 简单的直方图实现
    function createHistogram(data, bins = 50) {
        const min = Math.min(...data);
        const max = Math.max(...data);
        const binWidth = (max - min) / bins;
        const histogram = new Array(bins).fill(0);
        
        data.forEach(value => {
            const binIndex = Math.min(Math.floor((value - min) / binWidth), bins - 1);
            histogram[binIndex]++;
        });
        
        return {
            bins: histogram,
            binWidth,
            min,
            max
        };
    }

    // 绘制直方图
    function drawHistogram(canvasId, data) {
        const canvas = document.getElementById(canvasId);
        const ctx = canvas.getContext('2d');
        const hist = createHistogram(data);
        
        // 设置画布大小
        canvas.width = canvas.parentElement.clientWidth;
        canvas.height = canvas.parentElement.clientHeight - 60;
        
        // 计算边距
        const margin = {
            top: 20,
            right: 30,
            bottom: 40,
            left: 50
        };
        
        const width = canvas.width - margin.left - margin.right;
        const height = canvas.height - margin.top - margin.bottom;
        
        // 清空画布
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        
        // 计算比例
        const xScale = width / hist.bins.length;
        const yScale = height / Math.max(...hist.bins);
        
        // 绘制坐标轴
        ctx.beginPath();
        ctx.moveTo(margin.left, margin.top);
        ctx.lineTo(margin.left, height + margin.top);
        ctx.lineTo(width + margin.left, height + margin.top);
        ctx.strokeStyle = '#666';
        ctx.stroke();
        
        // 绘制柱状图
        hist.bins.forEach((value, i) => {
            const x = i * xScale + margin.left;
            const y = height + margin.top - value * yScale;
            const barWidth = xScale * 0.9;
            const barHeight = value * yScale;
            
            ctx.fillStyle = 'rgb(59, 130, 246)';
            ctx.fillRect(x, y, barWidth, barHeight);
        });
        
        // 绘制坐标轴标签
        ctx.fillStyle = '#666';
        ctx.font = '12px Arial';
        ctx.textAlign = 'center';
        
        // X轴标签
        for (let i = 0; i <= 10; i++) {
            const x = margin.left + (width * i / 10);
            const value = hist.min + (hist.max - hist.min) * i / 10;
            ctx.fillText(value.toFixed(1), x, height + margin.top + 20);
        }
        
        // Y轴标签
        ctx.textAlign = 'right';
        const maxCount = Math.max(...hist.bins);
        for (let i = 0; i <= 5; i++) {
            const y = height + margin.top - (height * i / 5);
            const value = maxCount * i / 5;
            ctx.fillText(Math.round(value), margin.left - 10, y + 5);
        }
        
        // 添加轴标题
        ctx.font = '14px Arial';
        ctx.textAlign = 'center';
        ctx.fillText('响应时间 (ms)', canvas.width / 2, canvas.height - 5);
        
        ctx.save();
        ctx.translate(20, canvas.height / 2);
        ctx.rotate(-Math.PI / 2);
        ctx.fillText('请求数量', 0, 0);
        ctx.restore();
    }

    // 页面加载完成后绘制图表
    window.addEventListener('load', () => {
        const responseTimesData = [%s];
        drawHistogram('responseTimeChart', responseTimesData);
    });

    // 窗口大小改变时重绘图表
    window.addEventListener('resize', () => {
        const responseTimesData = [%s];
        drawHistogram('responseTimeChart', responseTimesData);
    });
)";

// HTML基础模板
const char* HTML_BASE_TEMPLATE = R"(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>HTTP性能测试报告</title>
    <style>
    %s
    </style>
</head>
<body>
    <div class="container">
        <h1>HTTP性能测试报告</h1>
        <div class="grid">
            <div class="card">
                <h2>测试配置</h2>
                <p>服务器: %s:%d</p>
                <p>线程数: %d</p>
                <p>每线程请求数: %d</p>
                <p>测试时间: %s</p>
            </div>
            <div class="card">
                <h2>测试结果</h2>
                <p>总请求数: %d</p>
                <p>成功请求数: %d</p>
                <p>总传输量: %.2f MB</p>
                <p>平均响应时间: %.2f ms</p>
                <p>QPS: %.2f</p>
            </div>
        </div>
        <div class="chart-container">
            <h2>响应时间分布</h2>
            <canvas id="responseTimeChart"></canvas>
        </div>
    </div>
    <script>
    %s
    </script>
</body>
</html>
)";

// 创建响应时间字符串
char* create_response_times_string(const struct ThreadStats* stats) {
    char* response_times_str = malloc(stats->response_time_count * 20);
    response_times_str[0] = '\0';
    
    for (int i = 0; i < stats->response_time_count; i++) {
        char temp[20];
        snprintf(temp, sizeof(temp), "%.2f%s", 
                stats->response_times[i],
                i < stats->response_time_count - 1 ? "," : "");
        strcat(response_times_str, temp);
    }
    
    return response_times_str;
}

// 获取当前时间字符串
char* get_current_time_string(void) {
    time_t now = time(NULL);
    char* time_str = malloc(64);
    strftime(time_str, 64, "%Y-%m-%d %H:%M:%S", localtime(&now));
    return time_str;
}

// 计算统计数据
void calculate_statistics(const struct ThreadStats* stats, double total_time,
                        double* avg_response_time, double* qps, double* total_mb) {
    *avg_response_time = stats->total_time / stats->total_requests;
    *qps = stats->total_requests / (total_time / 1000.0);
    *total_mb = stats->total_bytes / (1024.0 * 1024.0);
}

// 生成HTML报告
void write_html_report(const char* filename, struct ThreadStats* stats, 
                      int num_threads, double total_time) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "无法创建报告文件\n");
        return;
    }

    // 准备数据
    char* response_times_str = create_response_times_string(stats);
    char* time_str = get_current_time_string();
    
    double avg_response_time, qps, total_mb;
    calculate_statistics(stats, total_time, &avg_response_time, &qps, &total_mb);

    // 准备JavaScript代码（替换响应时间数据）
    char* js_code = malloc(strlen(JS_TEMPLATE) + strlen(response_times_str) * 2 + 1);
    sprintf(js_code, JS_TEMPLATE, response_times_str, response_times_str);

    // 写入报告
    fprintf(fp, HTML_BASE_TEMPLATE,
            CSS_TEMPLATE,
            g_config.server_ip,
            g_config.server_port,
            g_config.num_threads,
            g_config.requests_per_thread,
            time_str,
            stats->total_requests,
            stats->successful_requests,
            total_mb,
            avg_response_time,
            qps,
            js_code);

    // 清理资源
    free(response_times_str);
    free(time_str);
    free(js_code);
    fclose(fp);
}