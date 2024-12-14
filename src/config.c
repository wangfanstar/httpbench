#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "../include/config.h"

// 全局配置变量
struct Config g_config;

// 初始化默认配置
void init_default_config(void) {
    strcpy(g_config.server_ip, "127.0.0.1");
    g_config.server_port = 8081;
    g_config.num_threads = 10;
    g_config.requests_per_thread = 100;
}

// 打印使用说明
void print_usage(const char *program_name) {
    printf("使用方法: %s [选项]\n\n", program_name);
    printf("选项:\n");
    printf("  -h, --help                显示帮助信息\n");
    printf("  -i, --ip <ip>            设置服务器IP地址（默认：127.0.0.1）\n");
    printf("  -p, --port <port>        设置服务器端口（默认：8081）\n");
    printf("  -t, --threads <num>      设置线程数量（默认：10）\n");
    printf("  -r, --requests <num>     设置每线程请求数（默认：100）\n");
}

// 解析命令行参数
int parse_arguments(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"help",     no_argument,       0, 'h'},
        {"ip",       required_argument, 0, 'i'},
        {"port",     required_argument, 0, 'p'},
        {"threads",  required_argument, 0, 't'},
        {"requests", required_argument, 0, 'r'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;

    while ((c = getopt_long(argc, argv, "hi:p:t:r:", long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                print_usage(argv[0]);
                exit(0);
            case 'i':
                strncpy(g_config.server_ip, optarg, sizeof(g_config.server_ip) - 1);
                break;
            case 'p':
                g_config.server_port = atoi(optarg);
                if (g_config.server_port <= 0 || g_config.server_port > 65535) {
                    fprintf(stderr, "错误：端口号必须在1-65535之间\n");
                    return 1;
                }
                break;
            case 't':
                g_config.num_threads = atoi(optarg);
                if (g_config.num_threads <= 0) {
                    fprintf(stderr, "错误：线程数必须大于0\n");
                    return 1;
                }
                break;
            case 'r':
                g_config.requests_per_thread = atoi(optarg);
                if (g_config.requests_per_thread <= 0) {
                    fprintf(stderr, "错误：每线程请求数必须大于0\n");
                    return 1;
                }
                break;
            case '?':
                return 1;
            default:
                return 1;
        }
    }

    return 0;
}