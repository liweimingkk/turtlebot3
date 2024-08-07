cmake_minimum_required(VERSION 3.5)
project(turtlebot3_node)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 设置 location_DIR
set(location_DIR "/home/liweiming/colcon_ws/install/location/share/location/cmake")

# Find ament packages and libraries for ament and system dependencies
find_package(ament_cmake REQUIRED)
find_package(dynamixel_sdk REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(message_filters REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rcutils REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(turtlebot3_msgs REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

# Include directories
include_directories(
  include
  ${CMAKE_BINARY_DIR}/rosidl_generator_cpp/turtlebot3_node/msg
  ${CMAKE_INSTALL_PREFIX}/include
  ${location_INCLUDE_DIRS}  # 添加 location 包的包含路径
)

# Build library
add_library(${PROJECT_NAME}_lib
  "src/devices/motor_power.cpp"
  "src/devices/sound.cpp"
  "src/devices/reset.cpp"
  "src/diff_drive_controller.cpp"
  "src/dynamixel_sdk_wrapper.cpp"
  "src/odometry.cpp"
  "src/turtlebot3.cpp"
  "src/sensors/battery_state.cpp"
  "src/sensors/imu.cpp"
  "src/sensors/joint_state.cpp"
  "src/sensors/sensor_state.cpp"
)

set(DEPENDENCIES
  "dynamixel_sdk"
  "geometry_msgs"
  "message_filters"
  "nav_msgs"
  "rclcpp"
  "rcutils"
  "sensor_msgs"
  "std_msgs"
  "std_srvs"
  "tf2"
  "tf2_ros"
  "turtlebot3_msgs"
  "tf2_geometry_msgs"

)

ament_target_dependencies(${PROJECT_NAME}_lib ${DEPENDENCIES})

set(EXECUTABLE_NAME "turtlebot3_ros")
add_executable(${EXECUTABLE_NAME} src/node_main.cpp)
target_link_libraries(${EXECUTABLE_NAME} ${PROJECT_NAME}_lib)
ament_target_dependencies(${EXECUTABLE_NAME} ${DEPENDENCIES})

# Add the carttf executable
add_executable(carttf src/carttf.cc)  # 修正目标名称为 carttf
target_link_libraries(carttf ${PROJECT_NAME}_lib)
ament_target_dependencies(carttf ${DEPENDENCIES})

# Install
install(DIRECTORY param
  DESTINATION share/${PROJECT_NAME}
)

install(TARGETS ${EXECUTABLE_NAME} carttf  # 修正目标名称为 carttf
  DESTINATION lib/${PROJECT_NAME}
)

# Macro for ament package
ament_export_include_directories(include)
ament_package()
