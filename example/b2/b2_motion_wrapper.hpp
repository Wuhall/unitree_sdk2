#ifndef __B2_MOTION_WRAPPER_HPP__
#define __B2_MOTION_WRAPPER_HPP__

#include <iostream>
#include <string>
#include <memory>
#include <unitree/robot/b2/sport/sport_client.hpp>
#include <unitree/robot/b2/motion_switcher/motion_switcher_client.hpp>
#include <unitree/robot/channel/channel_factory.hpp>

namespace unitree_b2 {

/**
 * @brief 简化的B2机器人运动控制封装类
 * 
 * 这个类为Python开发者提供了一个简化的C++接口来控制B2机器人的基本运动功能。
 * 封装了复杂的底层API，提供简单易用的方法。
 */
class B2MotionWrapper {
public:
    /**
     * @brief 构造函数
     * @param network_interface 网络接口名称（如"eth0"）
     */
    explicit B2MotionWrapper(const std::string& network_interface);
    
    /**
     * @brief 析构函数
     */
    ~B2MotionWrapper();
    
    /**
     * @brief 初始化机器人连接
     * @return true表示初始化成功，false表示失败
     */
    bool initialize();
    
    /**
     * @brief 检查机器人是否已连接
     * @return true表示已连接，false表示未连接
     */
    bool isConnected() const;
    
    // ========== 基本运动控制 ==========
    
    /**
     * @brief 让机器人进入阻尼模式（安全模式）
     * @return true表示成功，false表示失败
     */
    bool enterDampMode();
    
    /**
     * @brief 让机器人平衡站立
     * @return true表示成功，false表示失败
     */
    bool standUp();
    
    /**
     * @brief 让机器人坐下
     * @return true表示成功，false表示失败
     */
    bool sitDown();
    
    /**
     * @brief 停止所有运动
     * @return true表示成功，false表示失败
     */
    bool stopMoving();
    
    /**
     * @brief 恢复站立（从跌倒状态恢复）
     * @return true表示成功，false表示失败
     */
    bool recoveryStand();
    
    // ========== 移动控制 ==========
    
    /**
     * @brief 让机器人移动
     * @param forward_speed 前进速度 (m/s)，正值向前，负值向后
     * @param side_speed 侧向速度 (m/s)，正值向右，负值向左
     * @param turn_speed 转向速度 (rad/s)，正值顺时针，负值逆时针
     * @return true表示成功，false表示失败
     */
    bool move(float forward_speed, float side_speed, float turn_speed);
    
    /**
     * @brief 让机器人向前移动
     * @param speed 前进速度 (m/s)
     * @return true表示成功，false表示失败
     */
    bool moveForward(float speed);
    
    /**
     * @brief 让机器人向后移动
     * @param speed 后退速度 (m/s)
     * @return true表示成功，false表示失败
     */
    bool moveBackward(float speed);
    
    /**
     * @brief 让机器人向左移动
     * @param speed 侧向速度 (m/s)
     * @return true表示成功，false表示失败
     */
    bool moveLeft(float speed);
    
    /**
     * @brief 让机器人向右移动
     * @param speed 侧向速度 (m/s)
     * @return true表示成功，false表示失败
     */
    bool moveRight(float speed);
    
    /**
     * @brief 让机器人转向
     * @param angular_speed 转向速度 (rad/s)，正值顺时针，负值逆时针
     * @return true表示成功，false表示失败
     */
    bool turn(float angular_speed);
    
    // ========== 步态控制 ==========
    
    /**
     * @brief 切换步态模式
     * @param gait_type 步态类型：0=慢步，1=快步，2=跑步
     * @return true表示成功，false表示失败
     */
    bool switchGait(int gait_type);
    
    /**
     * @brief 设置速度等级
     * @param level 速度等级：0=慢速，1=中速，2=快速
     * @return true表示成功，false表示失败
     */
    bool setSpeedLevel(int level);
    
    // ========== 特殊动作 ==========
    
    /**
     * @brief 进入自由行走模式
     * @return true表示成功，false表示失败
     */
    bool enableFreeWalk();
    
    /**
     * @brief 进入经典行走模式
     * @param enable 是否启用
     * @return true表示成功，false表示失败
     */
    bool enableClassicWalk(bool enable);
    
    /**
     * @brief 进入快速行走模式
     * @param enable 是否启用
     * @return true表示成功，false表示失败
     */
    bool enableFastWalk(bool enable);
    
    /**
     * @brief 设置机器人身体高度
     * @param height 高度值
     * @return true表示成功，false表示失败
     */
    bool setBodyHeight(float height);
    
    /**
     * @brief 设置机器人姿态
     * @param roll 横滚角 (rad)
     * @param pitch 俯仰角 (rad)
     * @param yaw 偏航角 (rad)
     * @return true表示成功，false表示失败
     */
    bool setBodyPose(float roll, float pitch, float yaw);
    
    // ========== 安全功能 ==========
    
    /**
     * @brief 设置自动恢复功能
     * @param enable 是否启用自动恢复
     * @return true表示成功，false表示失败
     */
    bool setAutoRecovery(bool enable);
    
    /**
     * @brief 获取最后一次操作的结果信息
     * @return 结果描述字符串
     */
    std::string getLastResult() const;

private:
    std::string network_interface_;
    std::unique_ptr<unitree::robot::b2::SportClient> sport_client_;
    std::unique_ptr<unitree::robot::b2::MotionSwitcherClient> motion_switcher_;
    bool is_initialized_;
    std::string last_result_;
    
    /**
     * @brief 执行运动命令并处理结果
     * @param result 运动命令的返回结果
     * @param operation_name 操作名称（用于错误信息）
     * @return true表示成功，false表示失败
     */
    bool handleMotionResult(int32_t result, const std::string& operation_name);
    
    /**
     * @brief 检查机器人是否处于安全状态
     * @return true表示安全，false表示不安全
     */
    bool checkSafety();
};

} // namespace unitree_b2

#endif // __B2_MOTION_WRAPPER_HPP__
