# httpbench

轻量级多线程 HTTP 服务器性能测试工具，支持可视化报告。

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## 功能特点

- 🚀 多线程并发测试
- 📊 可视化 HTML 报告
- 🎯 可自定义测试参数
- 📈 响应时间分析
- 🛠 简单易用的命令行接口

## 快速开始

```bash
# 克隆仓库
git clone https://github.com/yourusername/httpbench.git
cd httpbench

# 编译
make

# 使用默认配置运行
make run

# 使用自定义配置运行
make run ip=192.168.1.100 p=8080 t=20 r=1000
```

## 使用方法

简化的命令行参数：

| 参数 | 说明 | 默认值 |
|------|------|--------|
| ip | 服务器IP地址 | 127.0.0.1 |
| p  | 端口号 | 8081 |
| t  | 线程数 | 10 |
| r  | 每线程请求数 | 100 |

### 使用示例

```bash
# 基础测试
make run

# 指定服务器
make run ip=192.168.1.100 p=8080

# 高负载测试
make run t=50 r=500

# 完全自定义
make run ip=10.0.0.1 p=3000 t=20 r=1000
```

## 测试报告

工具会生成包含以下内容的 HTML 报告：
- 整体性能指标
- 响应时间分布
- 数据传输统计
- 交互式图表

## 编译要求

- GCC 编译器
- Make 工具
- pthread 库
- Unix/Linux 环境

## 开源协议

MIT License - 可自由使用和修改

## 参与贡献

欢迎提交 Pull Request！重大更改请先创建 Issue 讨论。