#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "../include/worker.h"
#include "../include/config.h"

// 发送单个HTTP请求并返回响应大小
int send_request(const char* ip, int port, struct ThreadStats* stats) {
    struct timeval start_time, end_time;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    // 记录开始时间
    gettimeofday(&start_time, NULL);

    // 连接服务器
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(sockfd);
        return -1;
    }

    // 发送HTTP GET请求
    const char* request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    send(sockfd, request, strlen(request), 0);

    // 接收响应
    char buffer[4096];
    int total_bytes = 0;
    int bytes_received;
    
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        total_bytes += bytes_received;
    }

    // 记录结束时间
    gettimeofday(&end_time, NULL);
    
    // 计算响应时间（毫秒）
    double response_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                          (end_time.tv_usec - start_time.tv_usec) / 1000.0;

    // 记录统计数据
    record_response(stats, response_time, total_bytes);

    close(sockfd);
    return total_bytes;
}

// 工作线程函数
void* worker_thread(void* arg) {
    struct WorkerArgs* args = (struct WorkerArgs*)arg;
    
    for (int i = 0; i < g_config.requests_per_thread; i++) {
        int result = send_request(g_config.server_ip, g_config.server_port, args->stats);
        if (result < 0) {
            fprintf(stderr, "线程 %d: 请求失败\n", args->thread_id);
        }
    }

    return NULL;
}