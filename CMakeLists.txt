cmake_minimum_required(VERSION 3.0.2)
project(ros_lidar_recorder)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  std_msgs
  roslib
)

catkin_package(
   INCLUDE_DIRS include
   LIBRARIES ros_lidar_recorder
   CATKIN_DEPENDS roscpp sensor_msgs roslib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
  src/recorder.cpp
)

target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})

add_executable(recorder_event_node node/recorder_event_node.cpp)
target_link_libraries(recorder_event_node ${catkin_LIBRARIES} ${PROJECT_NAME})

add_executable(recorder_auto_node node/recorder_auto_node.cpp)
target_link_libraries(recorder_auto_node ${catkin_LIBRARIES} ${PROJECT_NAME})
