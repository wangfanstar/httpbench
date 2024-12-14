# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -I./include -pthread -O2
LDFLAGS = -pthread

# 目录设置
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# 源文件和目标文件
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/httpbench

# 头文件依赖
DEPS = $(wildcard $(INC_DIR)/*.h)

# 运行参数
ip ?= 127.0.0.1    # 服务器IP
p ?= 8081          # 端口
t ?= 10            # 线程数
r ?= 100           # 请求数

# 默认目标
.PHONY: all clean help run

all: directories $(TARGET)

# 创建必要的目录
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# 编译目标文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@echo "编译 $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# 链接可执行文件
$(TARGET): $(OBJECTS)
	@echo "链接 $(notdir $(TARGET))..."
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "构建完成！"

# 运行程序
run: all
	@echo "启动性能测试..."
	@echo "服务器地址: $(ip):$(p)"
	@echo "线程数: $(t)"
	@echo "每线程请求数: $(r)"
	@$(TARGET) -i $(ip) -p $(p) -t $(t) -r $(r)

# 清理编译产物
clean:
	@echo "清理编译产物..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "清理完成！"

# 帮助信息
help:
	@echo "可用的 make 目标："
	@echo "  all        - 构建完整项目（默认目标）"
	@echo "  clean      - 删除所有编译产物"
	@echo "  run        - 构建并运行程序"
	@echo "  help       - 显示此帮助信息"
	@echo ""
	@echo "运行参数："
	@echo "  ip         - 服务器IP地址（默认：127.0.0.1）"
	@echo "  p          - 服务器端口（默认：8081）"
	@echo "  t          - 线程数量（默认：10）"
	@echo "  r          - 每线程请求数（默认：100）"
	@echo ""
	@echo "示例："
	@echo "  make run ip=192.168.1.100 p=8080 t=20 r=1000"