#!/bin/bash

# B2机器人快速测试脚本
# 使用方法: ./quick_test.sh <网络接口> <动作名称>

if [ $# -lt 2 ]; then
    echo "使用方法: $0 <网络接口> <动作名称>"
    echo ""
    echo "可用动作:"
    echo "  stand        - 站立"
    echo "  sit          - 坐下"
    echo "  stop         - 停止"
    echo "  damp         - 进入阻尼模式"
    echo "  recovery     - 恢复站立"
    echo "  forward      - 向前移动"
    echo "  backward     - 向后移动"
    echo "  left         - 向左移动"
    echo "  right        - 向右移动"
    echo "  turn         - 转向"
    echo "  height       - 设置身体高度"
    echo "  pose         - 设置身体姿态"
    echo "  gait         - 切换步态"
    echo "  speed        - 设置速度等级"
    echo "  freewalk     - 自由行走"
    echo "  classicwalk  - 经典行走"
    echo "  fastwalk     - 快速行走"
    echo "  autorecovery - 自动恢复"
    echo ""
    echo "示例:"
    echo "  $0 eth0 stand"
    echo "  $0 eth0 forward"
    exit 1
fi

NETWORK_INTERFACE=$1
ACTION=$2

# 检查是否在正确的目录
if [ ! -f "CMakeLists.txt" ]; then
    echo "错误: 请在unitree_sdk2根目录下运行此脚本"
    echo "当前目录: $(pwd)"
    echo "请切换到: cd /path/to/unitree_sdk2"
    exit 1
fi

# 检查build目录
if [ ! -d "build" ]; then
    echo "正在创建build目录..."
    mkdir build
fi

# 进入build目录
cd build

echo "正在编译..."
make b2_single_action_test

if [ $? -ne 0 ]; then
    echo "编译失败!"
    echo "请确保:"
    echo "1. 在unitree_sdk2根目录下运行"
    echo "2. 已经执行过 'cmake ..'"
    echo "3. 所有依赖库都已安装"
    exit 1
fi

echo "编译成功!"
echo "正在运行测试: $ACTION"
echo ""

./example/b2/b2_single_action_test $NETWORK_INTERFACE $ACTION
