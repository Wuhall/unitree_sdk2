# B2机器人运动控制封装类使用指南

## 概述

这个封装类为Python开发者提供了一个简化的C++接口来控制宇树B2机器人的基本运动功能。封装了复杂的底层API，提供简单易用的方法。

## 文件结构

```
b2_motion_wrapper.hpp          # 封装类头文件
b2_motion_wrapper.cpp          # 封装类实现文件
b2_simple_example.cpp          # 简单使用示例
b2_advanced_example.cpp        # 高级使用示例
```

## 快速开始

### 1. 基本使用

```cpp
#include "b2_motion_wrapper.hpp"

int main() {
    // 创建机器人控制器
    unitree_b2::B2MotionWrapper robot("eth0");
    
    // 初始化连接
    if (!robot.initialize()) {
        std::cout << "初始化失败: " << robot.getLastResult() << std::endl;
        return -1;
    }
    
    // 让机器人站立
    if (robot.standUp()) {
        std::cout << "站立成功!" << std::endl;
    }
    
    // 向前移动
    robot.moveForward(0.5f);
    
    // 停止移动
    robot.stopMoving();
    
    // 进入安全模式
    robot.enterDampMode();
    
    return 0;
}
```

### 2. 编译和运行

```bash
# 编译
cd /path/to/unitree_sdk2
mkdir build && cd build
cmake ..
make

# 运行简单示例
./example/b2/b2_simple_example eth0

# 运行高级示例
./example/b2/b2_advanced_example eth0
```

## API参考

### 基本运动控制

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `standUp()` | 让机器人站立 | 无 | bool |
| `sitDown()` | 让机器人坐下 | 无 | bool |
| `stopMoving()` | 停止所有运动 | 无 | bool |
| `enterDampMode()` | 进入阻尼模式（安全模式） | 无 | bool |
| `recoveryStand()` | 恢复站立 | 无 | bool |

### 移动控制

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `move(forward, side, turn)` | 综合移动控制 | forward: 前进速度, side: 侧向速度, turn: 转向速度 | bool |
| `moveForward(speed)` | 向前移动 | speed: 前进速度 (m/s) | bool |
| `moveBackward(speed)` | 向后移动 | speed: 后退速度 (m/s) | bool |
| `moveLeft(speed)` | 向左移动 | speed: 侧向速度 (m/s) | bool |
| `moveRight(speed)` | 向右移动 | speed: 侧向速度 (m/s) | bool |
| `turn(angular_speed)` | 转向 | angular_speed: 转向速度 (rad/s) | bool |

### 步态和速度控制

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `switchGait(gait_type)` | 切换步态 | gait_type: 0=慢步, 1=快步, 2=跑步 | bool |
| `setSpeedLevel(level)` | 设置速度等级 | level: 0=慢速, 1=中速, 2=快速 | bool |
| `enableFreeWalk()` | 启用自由行走 | 无 | bool |
| `enableClassicWalk(enable)` | 启用/禁用经典行走 | enable: true/false | bool |
| `enableFastWalk(enable)` | 启用/禁用快速行走 | enable: true/false | bool |

### 姿态控制

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `setBodyHeight(height)` | 设置身体高度 | height: 高度值 | bool |
| `setBodyPose(roll, pitch, yaw)` | 设置身体姿态 | roll: 横滚角, pitch: 俯仰角, yaw: 偏航角 | bool |

### 安全功能

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `setAutoRecovery(enable)` | 设置自动恢复 | enable: true/false | bool |
| `isConnected()` | 检查连接状态 | 无 | bool |
| `getLastResult()` | 获取最后操作结果 | 无 | string |

## 使用示例

### 示例1: 基本运动序列

```cpp
// 初始化
unitree_b2::B2MotionWrapper robot("eth0");
robot.initialize();

// 运动序列
robot.standUp();                    // 站立
std::this_thread::sleep_for(std::chrono::seconds(2));

robot.moveForward(0.5f);           // 向前移动
std::this_thread::sleep_for(std::chrono::seconds(2));

robot.turn(1.57f);                 // 转向90度
std::this_thread::sleep_for(std::chrono::seconds(1));

robot.stopMoving();                 // 停止
robot.enterDampMode();             // 进入安全模式
```

### 示例2: 路径规划

```cpp
// 执行正方形路径
std::vector<std::pair<float, float>> square_moves = {
    {1.0f, 0.0f},   // 向前
    {0.0f, 1.0f},   // 向右
    {-1.0f, 0.0f},  // 向后
    {0.0f, -1.0f}   // 向左
};

for (const auto& move : square_moves) {
    robot.move(move.first, move.second, 0.0f);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    robot.stopMoving();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
```

### 示例3: 姿态控制

```cpp
// 姿态控制序列
std::vector<std::tuple<float, float, float, std::string>> poses = {
    {0.0f, 0.0f, 0.0f, "水平姿态"},
    {0.1f, 0.0f, 0.0f, "向右倾斜"},
    {-0.1f, 0.0f, 0.0f, "向左倾斜"},
    {0.0f, 0.1f, 0.0f, "向前倾斜"},
    {0.0f, -0.1f, 0.0f, "向后倾斜"}
};

for (const auto& pose : poses) {
    float roll = std::get<0>(pose);
    float pitch = std::get<1>(pose);
    float yaw = std::get<2>(pose);
    std::string description = std::get<3>(pose);
    
    robot.setBodyPose(roll, pitch, yaw);
    std::this_thread::sleep_for(std::chrono::seconds(2));
}
```

## 安全注意事项

1. **环境安全**: 确保机器人在平坦、无障碍物的环境中运行
2. **初始化检查**: 始终检查`initialize()`的返回值
3. **错误处理**: 检查每个运动命令的返回值
4. **安全模式**: 程序结束时确保机器人进入阻尼模式
5. **网络连接**: 确保网络接口参数正确

## 常见问题

### Q: 初始化失败怎么办？
A: 检查网络接口参数是否正确，确保机器人已连接。

### Q: 机器人不响应命令怎么办？
A: 检查机器人是否处于安全状态，尝试先调用`enterDampMode()`。

### Q: 如何调试运动问题？
A: 使用`getLastResult()`方法获取详细的错误信息。

### Q: 可以同时执行多个运动命令吗？
A: 不建议，应该等待当前命令完成后再执行下一个命令。

## 扩展开发

如果需要添加新的运动功能，可以：

1. 在`b2_motion_wrapper.hpp`中添加新的方法声明
2. 在`b2_motion_wrapper.cpp`中实现具体功能
3. 使用`handleMotionResult()`方法处理结果
4. 添加适当的错误检查和参数验证

## 技术支持

如有问题，请参考：
- 宇树官方文档
- unitree_sdk2示例代码
- 本封装类的示例程序
