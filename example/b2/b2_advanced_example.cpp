#include "b2_motion_wrapper.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cmath>

using namespace unitree_b2;

/**
 * @brief 高级B2机器人控制示例
 * 
 * 这个示例展示了如何使用B2MotionWrapper类进行更复杂的机器人控制，
 * 包括路径规划、连续运动、姿态控制等。
 */
class B2AdvancedController {
public:
    B2AdvancedController(const std::string& network_interface) 
        : robot_(network_interface) {}
    
    /**
     * @brief 初始化机器人
     */
    bool initialize() {
        return robot_.initialize();
    }
    
    /**
     * @brief 执行正方形路径
     */
    void executeSquarePath() {
        std::cout << "\n=== 执行正方形路径 ===" << std::endl;
        
        // 确保机器人站立
        if (!robot_.standUp()) {
            std::cout << "站立失败: " << robot_.getLastResult() << std::endl;
            return;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // 正方形的四条边
        std::vector<std::pair<float, float>> square_moves = {
            {1.0f, 0.0f},   // 向前
            {0.0f, 1.0f},   // 向右
            {-1.0f, 0.0f},  // 向后
            {0.0f, -1.0f}   // 向左
        };
        
        for (size_t i = 0; i < square_moves.size(); ++i) {
            std::cout << "执行第 " << (i + 1) << " 条边..." << std::endl;
            
            // 移动
            if (robot_.move(square_moves[i].first, square_moves[i].second, 0.0f)) {
                std::cout << "移动成功: " << robot_.getLastResult() << std::endl;
            } else {
                std::cout << "移动失败: " << robot_.getLastResult() << std::endl;
            }
            
            // 移动2秒
            std::this_thread::sleep_for(std::chrono::seconds(2));
            
            // 停止
            robot_.stopMoving();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
            // 转向90度（除了最后一条边）
            if (i < square_moves.size() - 1) {
                std::cout << "转向90度..." << std::endl;
                if (robot_.turn(1.57f)) {  // 90度 = π/2 弧度
                    std::cout << "转向成功: " << robot_.getLastResult() << std::endl;
                } else {
                    std::cout << "转向失败: " << robot_.getLastResult() << std::endl;
                }
                
                // 转向1秒
                std::this_thread::sleep_for(std::chrono::seconds(1));
                robot_.stopMoving();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
        
        std::cout << "正方形路径执行完成!" << std::endl;
    }
    
    /**
     * @brief 执行圆形路径
     */
    void executeCirclePath() {
        std::cout << "\n=== 执行圆形路径 ===" << std::endl;
        
        // 确保机器人站立
        if (!robot_.standUp()) {
            std::cout << "站立失败: " << robot_.getLastResult() << std::endl;
            return;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // 圆形路径参数
        const float radius = 0.5f;  // 半径
        const float angular_speed = 0.5f;  // 角速度
        const int steps = 20;  // 步数
        
        std::cout << "开始圆形路径，半径: " << radius << "m，角速度: " << angular_speed << "rad/s" << std::endl;
        
        for (int i = 0; i < steps; ++i) {
            float angle = (float)i / steps * 2.0f * M_PI;
            float vx = radius * angular_speed * cos(angle + M_PI/2);
            float vy = radius * angular_speed * sin(angle + M_PI/2);
            
            std::cout << "步骤 " << (i + 1) << "/" << steps 
                      << " - 角度: " << (angle * 180.0f / M_PI) << "度" << std::endl;
            
            if (robot_.move(vx, vy, 0.0f)) {
                std::cout << "移动成功: " << robot_.getLastResult() << std::endl;
            } else {
                std::cout << "移动失败: " << robot_.getLastResult() << std::endl;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        
        robot_.stopMoving();
        std::cout << "圆形路径执行完成!" << std::endl;
    }
    
    /**
     * @brief 执行姿态控制演示
     */
    void executePoseControl() {
        std::cout << "\n=== 执行姿态控制演示 ===" << std::endl;
        
        // 确保机器人站立
        if (!robot_.standUp()) {
            std::cout << "站立失败: " << robot_.getLastResult() << std::endl;
            return;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // 姿态控制序列
        std::vector<std::tuple<float, float, float, std::string>> poses = {
            {0.0f, 0.0f, 0.0f, "水平姿态"},
            {0.1f, 0.0f, 0.0f, "向右倾斜"},
            {-0.1f, 0.0f, 0.0f, "向左倾斜"},
            {0.0f, 0.1f, 0.0f, "向前倾斜"},
            {0.0f, -0.1f, 0.0f, "向后倾斜"},
            {0.0f, 0.0f, 0.0f, "恢复水平"}
        };
        
        for (const auto& pose : poses) {
            float roll = std::get<0>(pose);
            float pitch = std::get<1>(pose);
            float yaw = std::get<2>(pose);
            std::string description = std::get<3>(pose);
            
            std::cout << "设置姿态: " << description 
                      << " (roll:" << roll << ", pitch:" << pitch << ", yaw:" << yaw << ")" << std::endl;
            
            if (robot_.setBodyPose(roll, pitch, yaw)) {
                std::cout << "姿态设置成功: " << robot_.getLastResult() << std::endl;
            } else {
                std::cout << "姿态设置失败: " << robot_.getLastResult() << std::endl;
            }
            
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        
        std::cout << "姿态控制演示完成!" << std::endl;
    }
    
    /**
     * @brief 执行步态切换演示
     */
    void executeGaitDemo() {
        std::cout << "\n=== 执行步态切换演示 ===" << std::endl;
        
        // 确保机器人站立
        if (!robot_.standUp()) {
            std::cout << "站立失败: " << robot_.getLastResult() << std::endl;
            return;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // 步态演示序列
        std::vector<std::pair<int, std::string>> gaits = {
            {0, "慢步"},
            {1, "快步"},
            {2, "跑步"}
        };
        
        for (const auto& gait : gaits) {
            int gait_type = gait.first;
            std::string gait_name = gait.second;
            
            std::cout << "切换到" << gait_name << "模式..." << std::endl;
            
            if (robot_.switchGait(gait_type)) {
                std::cout << "步态切换成功: " << robot_.getLastResult() << std::endl;
            } else {
                std::cout << "步态切换失败: " << robot_.getLastResult() << std::endl;
            }
            
            // 在每种步态下移动一小段距离
            std::cout << "在" << gait_name << "模式下移动..." << std::endl;
            if (robot_.moveForward(0.3f)) {
                std::cout << "移动成功: " << robot_.getLastResult() << std::endl;
            } else {
                std::cout << "移动失败: " << robot_.getLastResult() << std::endl;
            }
            
            std::this_thread::sleep_for(std::chrono::seconds(2));
            robot_.stopMoving();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        
        std::cout << "步态切换演示完成!" << std::endl;
    }
    
    /**
     * @brief 执行安全演示
     */
    void executeSafetyDemo() {
        std::cout << "\n=== 执行安全演示 ===" << std::endl;
        
        // 确保机器人站立
        if (!robot_.standUp()) {
            std::cout << "站立失败: " << robot_.getLastResult() << std::endl;
            return;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // 启用自动恢复
        std::cout << "启用自动恢复功能..." << std::endl;
        if (robot_.setAutoRecovery(true)) {
            std::cout << "自动恢复设置成功: " << robot_.getLastResult() << std::endl;
        } else {
            std::cout << "自动恢复设置失败: " << robot_.getLastResult() << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // 演示恢复站立
        std::cout << "演示恢复站立功能..." << std::endl;
        if (robot_.recoveryStand()) {
            std::cout << "恢复站立成功: " << robot_.getLastResult() << std::endl;
        } else {
            std::cout << "恢复站立失败: " << robot_.getLastResult() << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // 进入安全模式
        std::cout << "进入安全模式..." << std::endl;
        if (robot_.enterDampMode()) {
            std::cout << "进入安全模式成功: " << robot_.getLastResult() << std::endl;
        } else {
            std::cout << "进入安全模式失败: " << robot_.getLastResult() << std::endl;
        }
        
        std::cout << "安全演示完成!" << std::endl;
    }

private:
    B2MotionWrapper robot_;
};

int main(int argc, char* argv[])
{
    // 检查命令行参数
    if (argc < 2) {
        std::cout << "使用方法: " << argv[0] << " <网络接口>" << std::endl;
        std::cout << "例如: " << argv[0] << " eth0" << std::endl;
        return -1;
    }
    
    std::string network_interface = argv[1];
    
    // 创建高级控制器
    std::cout << "正在创建B2高级控制器..." << std::endl;
    B2AdvancedController controller(network_interface);
    
    // 初始化机器人连接
    std::cout << "正在初始化机器人连接..." << std::endl;
    if (!controller.initialize()) {
        std::cout << "初始化失败" << std::endl;
        return -1;
    }
    std::cout << "初始化成功!" << std::endl;
    
    // 等待用户确认
    std::cout << "\n警告: 请确保机器人处于安全位置（地面平坦，周围无障碍物）" << std::endl;
    std::cout << "按回车键继续..." << std::endl;
    std::cin.ignore();
    
    try {
        // 执行各种演示
        controller.executeSquarePath();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        controller.executeCirclePath();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        controller.executePoseControl();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        controller.executeGaitDemo();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        controller.executeSafetyDemo();
        
    } catch (const std::exception& e) {
        std::cout << "发生异常: " << e.what() << std::endl;
        return -1;
    }
    
    std::cout << "\n高级示例程序执行完成!" << std::endl;
    std::cout << "机器人已进入安全模式，可以安全关闭程序。" << std::endl;
    
    return 0;
}
