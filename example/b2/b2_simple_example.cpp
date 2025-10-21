#include "b2_motion_wrapper.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace unitree_b2;

/**
 * @brief 简单的B2机器人控制示例
 * 
 * 这个示例展示了如何使用B2MotionWrapper类来控制B2机器人的基本运动。
 * 适合Python开发者学习C++的基本用法。
 */
int main(int argc, char* argv[])
{
    // 检查命令行参数
    if (argc < 2) {
        std::cout << "使用方法: " << argv[0] << " <网络接口>" << std::endl;
        std::cout << "例如: " << argv[0] << " eth0" << std::endl;
        return -1;
    }
    
    std::string network_interface = argv[1];
    
    // 创建B2运动控制包装器
    std::cout << "正在创建B2运动控制包装器..." << std::endl;
    B2MotionWrapper b2_robot(network_interface);
    
    // 初始化机器人连接
    std::cout << "正在初始化机器人连接..." << std::endl;
    if (!b2_robot.initialize()) {
        std::cout << "初始化失败: " << b2_robot.getLastResult() << std::endl;
        return -1;
    }
    std::cout << "初始化成功!" << std::endl;
    
    // 等待用户确认
    std::cout << "\n警告: 请确保机器人处于安全位置（地面平坦，周围无障碍物）" << std::endl;
    std::cout << "按回车键继续..." << std::endl;
    std::cin.ignore();
    
    try {
        // 示例1: 基本站立控制
        std::cout << "\n=== 示例1: 基本站立控制 ===" << std::endl;
        
        // 让机器人站立
        std::cout << "让机器人站立..." << std::endl;
        if (b2_robot.standUp()) {
            std::cout << "站立成功: " << b2_robot.getLastResult() << std::endl;
        } else {
            std::cout << "站立失败: " << b2_robot.getLastResult() << std::endl;
        }
        
        // 等待3秒
        std::this_thread::sleep_for(std::chrono::seconds(3));
        
        // 示例2: 基本移动控制
        std::cout << "\n=== 示例2: 基本移动控制 ===" << std::endl;
        
        // 向前移动
        std::cout << "向前移动..." << std::endl;
        if (b2_robot.moveForward(0.5f)) {
            std::cout << "向前移动成功: " << b2_robot.getLastResult() << std::endl;
        } else {
            std::cout << "向前移动失败: " << b2_robot.getLastResult() << std::endl;
        }
        
        // 等待2秒
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // 停止移动
        std::cout << "停止移动..." << std::endl;
        if (b2_robot.stopMoving()) {
            std::cout << "停止成功: " << b2_robot.getLastResult() << std::endl;
        } else {
            std::cout << "停止失败: " << b2_robot.getLastResult() << std::endl;
        }
        
        // 等待2秒
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // 示例3: 转向控制
        std::cout << "\n=== 示例3: 转向控制 ===" << std::endl;
        
        // 顺时针转向
        std::cout << "顺时针转向..." << std::endl;
        if (b2_robot.turn(0.5f)) {
            std::cout << "转向成功: " << b2_robot.getLastResult() << std::endl;
        } else {
            std::cout << "转向失败: " << b2_robot.getLastResult() << std::endl;
        }
        
        // 等待2秒
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // 停止移动
        std::cout << "停止移动..." << std::endl;
        b2_robot.stopMoving();
        
        // 等待2秒
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // 示例4: 步态和速度控制
        std::cout << "\n=== 示例4: 步态和速度控制 ===" << std::endl;
        
        // 设置速度等级
        std::cout << "设置速度等级为1（中速）..." << std::endl;
        if (b2_robot.setSpeedLevel(1)) {
            std::cout << "速度设置成功: " << b2_robot.getLastResult() << std::endl;
        } else {
            std::cout << "速度设置失败: " << b2_robot.getLastResult() << std::endl;
        }
        
        // 切换步态
        std::cout << "切换步态到类型1（快步）..." << std::endl;
        if (b2_robot.switchGait(1)) {
            std::cout << "步态切换成功: " << b2_robot.getLastResult() << std::endl;
        } else {
            std::cout << "步态切换失败: " << b2_robot.getLastResult() << std::endl;
        }
        
        // 等待2秒
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // 示例5: 安全控制
        std::cout << "\n=== 示例5: 安全控制 ===" << std::endl;
        
        // 启用自动恢复
        std::cout << "启用自动恢复功能..." << std::endl;
        if (b2_robot.setAutoRecovery(true)) {
            std::cout << "自动恢复设置成功: " << b2_robot.getLastResult() << std::endl;
        } else {
            std::cout << "自动恢复设置失败: " << b2_robot.getLastResult() << std::endl;
        }
        
        // 等待2秒
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // 示例6: 坐下
        std::cout << "\n=== 示例6: 坐下 ===" << std::endl;
        
        std::cout << "让机器人坐下..." << std::endl;
        if (b2_robot.sitDown()) {
            std::cout << "坐下成功: " << b2_robot.getLastResult() << std::endl;
        } else {
            std::cout << "坐下失败: " << b2_robot.getLastResult() << std::endl;
        }
        
        // 等待3秒
        std::this_thread::sleep_for(std::chrono::seconds(3));
        
        // 示例7: 进入安全模式
        std::cout << "\n=== 示例7: 进入安全模式 ===" << std::endl;
        
        std::cout << "进入阻尼模式（安全模式）..." << std::endl;
        if (b2_robot.enterDampMode()) {
            std::cout << "进入安全模式成功: " << b2_robot.getLastResult() << std::endl;
        } else {
            std::cout << "进入安全模式失败: " << b2_robot.getLastResult() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cout << "发生异常: " << e.what() << std::endl;
        return -1;
    }
    
    std::cout << "\n示例程序执行完成!" << std::endl;
    std::cout << "机器人已进入安全模式，可以安全关闭程序。" << std::endl;
    
    return 0;
}
