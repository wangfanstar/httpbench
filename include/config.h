#ifndef HTTPBENCH_CONFIG_H
#define HTTPBENCH_CONFIG_H

// 配置结构体
struct Config {
    char server_ip[64];     // 服务器IP地址
    int server_port;        // 服务器端口
    int num_threads;        // 线程数量
    int requests_per_thread;// 每线程请求数
};

// 全局配置变量
extern struct Config g_config;

// 函数声明
void init_default_config(void);
void print_usage(const char *program_name);
int parse_arguments(int argc, char *argv[]);

#endif // HTTPBENCH_CONFIG_H