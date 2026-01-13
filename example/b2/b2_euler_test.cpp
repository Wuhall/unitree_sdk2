#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <unitree/robot/b2/sport/sport_client.hpp>

using namespace std;

// 生成指定范围内的随机浮点数
float randomFloat(float min, float max, std::mt19937& gen)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " networkInterface" << std::endl;
        exit(-1);
    }

    // 初始化通道
    unitree::robot::ChannelFactory::Instance()->Init(0, argv[1]);

    // 创建SportClient实例
    unitree::robot::b2::SportClient sport_client;
    sport_client.SetTimeout(25.0f);
    sport_client.Init();

    // 初始化随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());

    // roll, pitch, yaw 取值范围
    const float MIN_VALUE = -0.6f;
    const float MAX_VALUE = 0.6f;

    // 先调用一次 BalanceStand
    std::cout << "Calling BalanceStand first..." << std::endl;
    int32_t res = sport_client.BalanceStand();
    if (res == 0)
    {
        std::cout << "BalanceStand succeeded." << std::endl;
    }
    else
    {
        std::cout << "BalanceStand failed. Error code: " << res << std::endl;
    }

    // 等待机器人稳定
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Starting Euler test loop..." << std::endl;
    std::cout << "Press Ctrl+C to stop." << std::endl;

    long success_count = 0;
    long error_count = 0;

    while (true)
    {
        auto time_start = std::chrono::high_resolution_clock::now();
        static const auto dt = std::chrono::milliseconds(2000);

        // 生成随机的 roll, pitch, yaw
        float roll = randomFloat(MIN_VALUE, MAX_VALUE, gen);
        float pitch = randomFloat(MIN_VALUE, MAX_VALUE, gen);
        float yaw = randomFloat(MIN_VALUE, MAX_VALUE, gen);

        // 调用 Euler 函数
        res = sport_client.Euler(roll, pitch, yaw);

        if (res == 0)
        {
            success_count++;
            std::cout << "[SUCCESS] Euler(roll=" << roll 
                      << ", pitch=" << pitch 
                      << ", yaw=" << yaw << ") - count: " << success_count << std::endl;
        }
        else
        {
            error_count++;
            std::cout << "[ERROR] Euler failed. Error code: " << res 
                      << " - error count: " << error_count << std::endl;
        }

        // 控制循环频率
        std::this_thread::sleep_until(time_start + dt);
    }

    return 0;
}

