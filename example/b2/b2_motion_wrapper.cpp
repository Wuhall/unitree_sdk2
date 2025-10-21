#include "b2_motion_wrapper.hpp"
#include <iostream>
#include <chrono>
#include <thread>

namespace unitree_b2 {

B2MotionWrapper::B2MotionWrapper(const std::string& network_interface)
    : network_interface_(network_interface)
    , is_initialized_(false)
    , last_result_("未初始化")
{
    // 初始化运动客户端
    sport_client_ = std::make_unique<unitree::robot::b2::SportClient>();
    motion_switcher_ = std::make_unique<unitree::robot::b2::MotionSwitcherClient>();
}

B2MotionWrapper::~B2MotionWrapper()
{
    // 析构时确保机器人进入安全状态
    if (is_initialized_) {
        enterDampMode();
    }
}

bool B2MotionWrapper::initialize()
{
    try {
        // 初始化网络通道
        unitree::robot::ChannelFactory::Instance()->Init(0, network_interface_.c_str());
        
        // 初始化运动客户端
        sport_client_->SetTimeout(10.0f);
        int32_t sport_result = sport_client_->Init();
        if (sport_result != 0) {
            last_result_ = "SportClient初始化失败，错误代码: " + std::to_string(sport_result);
            return false;
        }
        
        // 初始化运动切换客户端
        motion_switcher_->SetTimeout(10.0f);
        int32_t switcher_result = motion_switcher_->Init();
        if (switcher_result != 0) {
            last_result_ = "MotionSwitcherClient初始化失败，错误代码: " + std::to_string(switcher_result);
            return false;
        }
        
        is_initialized_ = true;
        last_result_ = "初始化成功";
        return true;
        
    } catch (const std::exception& e) {
        last_result_ = "初始化异常: " + std::string(e.what());
        return false;
    }
}

bool B2MotionWrapper::isConnected() const
{
    return is_initialized_;
}

bool B2MotionWrapper::enterDampMode()
{
    if (!checkSafety()) return false;
    return handleMotionResult(sport_client_->Damp(), "进入阻尼模式");
}

bool B2MotionWrapper::standUp()
{
    if (!checkSafety()) return false;
    return handleMotionResult(sport_client_->BalanceStand(), "平衡站立");
}

bool B2MotionWrapper::sitDown()
{
    if (!checkSafety()) return false;
    return handleMotionResult(sport_client_->StandDown(), "坐下");
}

bool B2MotionWrapper::stopMoving()
{
    if (!checkSafety()) return false;
    return handleMotionResult(sport_client_->StopMove(), "停止运动");
}

bool B2MotionWrapper::recoveryStand()
{
    if (!checkSafety()) return false;
    return handleMotionResult(sport_client_->RecoveryStand(), "恢复站立");
}

bool B2MotionWrapper::move(float forward_speed, float side_speed, float turn_speed)
{
    if (!checkSafety()) return false;
    
    // 限制速度范围以确保安全
    forward_speed = std::max(-2.0f, std::min(2.0f, forward_speed));
    side_speed = std::max(-1.0f, std::min(1.0f, side_speed));
    turn_speed = std::max(-2.0f, std::min(2.0f, turn_speed));
    
    return handleMotionResult(
        sport_client_->Move(forward_speed, side_speed, turn_speed),
        "移动 (前:" + std::to_string(forward_speed) + 
        ", 侧:" + std::to_string(side_speed) + 
        ", 转:" + std::to_string(turn_speed) + ")"
    );
}

bool B2MotionWrapper::moveForward(float speed)
{
    return move(speed, 0.0f, 0.0f);
}

bool B2MotionWrapper::moveBackward(float speed)
{
    return move(-speed, 0.0f, 0.0f);
}

bool B2MotionWrapper::moveLeft(float speed)
{
    return move(0.0f, speed, 0.0f);
}

bool B2MotionWrapper::moveRight(float speed)
{
    return move(0.0f, -speed, 0.0f);
}

bool B2MotionWrapper::turn(float angular_speed)
{
    return move(0.0f, 0.0f, angular_speed);
}

bool B2MotionWrapper::switchGait(int gait_type)
{
    if (!checkSafety()) return false;
    
    // 限制步态类型范围
    gait_type = std::max(0, std::min(2, gait_type));
    
    return handleMotionResult(
        sport_client_->SwitchGait(gait_type),
        "切换步态到类型: " + std::to_string(gait_type)
    );
}

bool B2MotionWrapper::setSpeedLevel(int level)
{
    if (!checkSafety()) return false;
    
    // 限制速度等级范围
    level = std::max(0, std::min(2, level));
    
    return handleMotionResult(
        sport_client_->SpeedLevel(level),
        "设置速度等级: " + std::to_string(level)
    );
}

bool B2MotionWrapper::enableFreeWalk()
{
    if (!checkSafety()) return false;
    return handleMotionResult(sport_client_->FreeWalk(), "启用自由行走");
}

bool B2MotionWrapper::enableClassicWalk(bool enable)
{
    if (!checkSafety()) return false;
    return handleMotionResult(
        sport_client_->ClassicWalk(enable),
        "经典行走模式: " + std::string(enable ? "启用" : "禁用")
    );
}

bool B2MotionWrapper::enableFastWalk(bool enable)
{
    if (!checkSafety()) return false;
    return handleMotionResult(
        sport_client_->FastWalk(enable),
        "快速行走模式: " + std::string(enable ? "启用" : "禁用")
    );
}

bool B2MotionWrapper::setBodyHeight(float height)
{
    if (!checkSafety()) return false;
    
    // 限制高度范围
    height = std::max(-0.2f, std::min(0.2f, height));
    
    return handleMotionResult(
        sport_client_->BodyHeight(height),
        "设置身体高度: " + std::to_string(height)
    );
}

bool B2MotionWrapper::setBodyPose(float roll, float pitch, float yaw)
{
    if (!checkSafety()) return false;
    
    // 限制角度范围
    roll = std::max(-0.5f, std::min(0.5f, roll));
    pitch = std::max(-0.5f, std::min(0.5f, pitch));
    yaw = std::max(-0.5f, std::min(0.5f, yaw));
    
    return handleMotionResult(
        sport_client_->Euler(roll, pitch, yaw),
        "设置身体姿态 (roll:" + std::to_string(roll) + 
        ", pitch:" + std::to_string(pitch) + 
        ", yaw:" + std::to_string(yaw) + ")"
    );
}

bool B2MotionWrapper::setAutoRecovery(bool enable)
{
    if (!checkSafety()) return false;
    return handleMotionResult(
        sport_client_->AutoRecoverySet(enable),
        "自动恢复: " + std::string(enable ? "启用" : "禁用")
    );
}

std::string B2MotionWrapper::getLastResult() const
{
    return last_result_;
}

bool B2MotionWrapper::handleMotionResult(int32_t result, const std::string& operation_name)
{
    if (result == 0) {
        last_result_ = operation_name + " 成功";
        return true;
    } else {
        last_result_ = operation_name + " 失败，错误代码: " + std::to_string(result);
        return false;
    }
}

bool B2MotionWrapper::checkSafety()
{
    if (!is_initialized_) {
        last_result_ = "机器人未初始化，请先调用initialize()";
        return false;
    }
    
    if (!sport_client_) {
        last_result_ = "运动客户端未初始化";
        return false;
    }
    
    return true;
}

} // namespace unitree_b2
