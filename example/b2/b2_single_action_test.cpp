#include "b2_motion_wrapper.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace unitree_b2;

/**
 * @brief 单个动作测试程序
 * 
 * 这个程序允许您测试B2机器人的单个动作方法，
 * 每次只执行一个动作，方便调试和验证。
 */
int main(int argc, char* argv[])
{
    // 检查命令行参数
    if (argc < 3) {
        std::cout << "使用方法: " << argv[0] << " <网络接口> <动作名称>" << std::endl;
        std::cout << "可用动作:" << std::endl;
        std::cout << "  - stand: 站立" << std::endl;
        std::cout << "  - sit: 坐下" << std::endl;
        std::cout << "  - stop: 停止" << std::endl;
        std::cout << "  - damp: 进入阻尼模式" << std::endl;
        std::cout << "  - recovery: 恢复站立" << std::endl;
        std::cout << "  - forward: 向前移动" << std::endl;
        std::cout << "  - backward: 向后移动" << std::endl;
        std::cout << "  - left: 向左移动" << std::endl;
        std::cout << "  - right: 向右移动" << std::endl;
        std::cout << "  - turn: 转向" << std::endl;
        std::cout << "  - height: 设置身体高度" << std::endl;
        std::cout << "  - pose: 设置身体姿态" << std::endl;
        std::cout << "  - gait: 切换步态" << std::endl;
        std::cout << "  - speed: 设置速度等级" << std::endl;
        std::cout << "  - freewalk: 自由行走" << std::endl;
        std::cout << "  - classicwalk: 经典行走" << std::endl;
        std::cout << "  - fastwalk: 快速行走" << std::endl;
        std::cout << "  - autorecovery: 自动恢复" << std::endl;
        std::cout << "\n示例:" << std::endl;
        std::cout << "  " << argv[0] << " eth0 stand" << std::endl;
        std::cout << "  " << argv[0] << " eth0 forward" << std::endl;
        return -1;
    }
    
    std::string network_interface = argv[1];
    std::string action = argv[2];
    
    // 创建机器人控制器
    std::cout << "正在创建B2运动控制包装器..." << std::endl;
    B2MotionWrapper robot(network_interface);
    
    // 初始化机器人连接
    std::cout << "正在初始化机器人连接..." << std::endl;
    if (!robot.initialize()) {
        std::cout << "初始化失败: " << robot.getLastResult() << std::endl;
        return -1;
    }
    std::cout << "初始化成功!" << std::endl;
    
    // 等待用户确认
    std::cout << "\n警告: 请确保机器人处于安全位置（地面平坦，周围无障碍物）" << std::endl;
    std::cout << "按回车键继续执行动作: " << action << std::endl;
    std::cin.ignore();
    
    bool success = false;
    
    try {
        // 根据动作名称执行相应的操作
        if (action == "stand") {
            std::cout << "执行动作: 站立" << std::endl;
            success = robot.standUp();
        }
        else if (action == "sit") {
            std::cout << "执行动作: 坐下" << std::endl;
            success = robot.sitDown();
        }
        else if (action == "stop") {
            std::cout << "执行动作: 停止" << std::endl;
            success = robot.stopMoving();
        }
        else if (action == "damp") {
            std::cout << "执行动作: 进入阻尼模式" << std::endl;
            success = robot.enterDampMode();
        }
        else if (action == "recovery") {
            std::cout << "执行动作: 恢复站立" << std::endl;
            success = robot.recoveryStand();
        }
        else if (action == "forward") {
            std::cout << "执行动作: 向前移动 (速度: 0.3 m/s)" << std::endl;
            success = robot.moveForward(0.3f);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            robot.stopMoving();
        }
        else if (action == "backward") {
            std::cout << "执行动作: 向后移动 (速度: 0.3 m/s)" << std::endl;
            success = robot.moveBackward(0.3f);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            robot.stopMoving();
        }
        else if (action == "left") {
            std::cout << "执行动作: 向左移动 (速度: 0.3 m/s)" << std::endl;
            success = robot.moveLeft(0.3f);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            robot.stopMoving();
        }
        else if (action == "right") {
            std::cout << "执行动作: 向右移动 (速度: 0.3 m/s)" << std::endl;
            success = robot.moveRight(0.3f);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            robot.stopMoving();
        }
        else if (action == "turn") {
            std::cout << "执行动作: 转向 (角速度: 0.5 rad/s)" << std::endl;
            success = robot.turn(0.5f);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            robot.stopMoving();
        }
        else if (action == "height") {
            std::cout << "执行动作: 设置身体高度 (高度: 0.1 m)" << std::endl;
            success = robot.setBodyHeight(0.1f);
        }
        else if (action == "pose") {
            std::cout << "执行动作: 设置身体姿态 (roll: 0.1, pitch: 0.0, yaw: 0.0)" << std::endl;
            success = robot.setBodyPose(0.1f, 0.0f, 0.0f);
        }
        else if (action == "gait") {
            std::cout << "执行动作: 切换步态 (步态类型: 1 - 快步)" << std::endl;
            success = robot.switchGait(1);
        }
        else if (action == "speed") {
            std::cout << "执行动作: 设置速度等级 (等级: 1 - 中速)" << std::endl;
            success = robot.setSpeedLevel(1);
        }
        else if (action == "freewalk") {
            std::cout << "执行动作: 启用自由行走" << std::endl;
            success = robot.enableFreeWalk();
        }
        else if (action == "classicwalk") {
            std::cout << "执行动作: 启用经典行走" << std::endl;
            success = robot.enableClassicWalk(true);
        }
        else if (action == "fastwalk") {
            std::cout << "执行动作: 启用快速行走" << std::endl;
            success = robot.enableFastWalk(true);
        }
        else if (action == "autorecovery") {
            std::cout << "执行动作: 启用自动恢复" << std::endl;
            success = robot.setAutoRecovery(true);
        }
        else {
            std::cout << "未知动作: " << action << std::endl;
            std::cout << "请使用 --help 查看可用动作列表" << std::endl;
            return -1;
        }
        
        // 显示结果
        if (success) {
            std::cout << "✅ 动作执行成功: " << robot.getLastResult() << std::endl;
        } else {
            std::cout << "❌ 动作执行失败: " << robot.getLastResult() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cout << "❌ 发生异常: " << e.what() << std::endl;
        return -1;
    }
    
    // 等待用户确认
    std::cout << "\n按回车键进入安全模式并退出..." << std::endl;
    std::cin.ignore();
    
    // 进入安全模式
    std::cout << "进入安全模式..." << std::endl;
    robot.enterDampMode();
    
    std::cout << "测试完成!" << std::endl;
    return 0;
}
