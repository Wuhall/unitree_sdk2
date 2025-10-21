add_executable(b2_sport_client  b2_sport_client.cpp)
target_link_libraries(b2_sport_client unitree_sdk2)

add_executable(b2_stand_example  b2_stand_example.cpp)
target_link_libraries(b2_stand_example unitree_sdk2)

# B2运动控制封装类示例
add_executable(b2_simple_example  b2_simple_example.cpp b2_motion_wrapper.cpp)
target_link_libraries(b2_simple_example unitree_sdk2)

add_executable(b2_advanced_example  b2_advanced_example.cpp b2_motion_wrapper.cpp)
target_link_libraries(b2_advanced_example unitree_sdk2)

# B2运动控制封装类测试
add_executable(test_b2_wrapper  test_b2_wrapper.cpp b2_motion_wrapper.cpp)
target_link_libraries(test_b2_wrapper unitree_sdk2)