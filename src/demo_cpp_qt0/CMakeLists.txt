cmake_minimum_required(VERSION 3.8)
project(demo_cpp_qt0)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(demo_interface REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

file(GLOB PRO_FORM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui)
file(GLOB_RECURSE PRO_INCLUDE_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/*.hpp *.h )
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

qt6_wrap_ui(QT_UI_HPP ${PRO_FORM_DIR})
qt6_wrap_cpp(QT_MOC_HPP ${PRO_INCLUDE_DIR})

include_directories(
        include  
        ${CMAKE_CURRENT_BINARY_DIR}    
)

add_executable(main 
      src/main.cpp 
      src/widget.cpp 
      src/sys_node.cpp
      src/SystemSub.cpp
      src/RosNodeClass.cpp
      src/CommonClass.cpp 
      src/MapWidget.cpp
      ${PRO_FORM_DIR} ${QT_UI_HPP} ${PRO_INCLUDE_DIR} ${QT_MOC_HPP} ${SRC_FILES}
)

ament_target_dependencies(main 
  rclcpp 
  demo_interface 
  rclcpp_lifecycle 
  rclcpp_components
  geometry_msgs
  nav_msgs
  sensor_msgs
  tf2_ros
  tf2
  tf2_geometry_msgs
)

target_link_libraries(main Qt6::Widgets )

target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/src/GraphicView
)

install(TARGETS main
  DESTINATION lib/${PROJECT_NAME}
)

add_library(SystemSub SHARED
  src/SystemSub.cpp
)

ament_target_dependencies(SystemSub
  rclcpp
  demo_interface 
  rclcpp_lifecycle 
  rclcpp_components
  sensor_msgs
  nav_msgs
  geometry_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
)

target_link_libraries(SystemSub Qt6::Widgets )

rclcpp_components_register_nodes(SystemSub
  "SystemSub::NodeSub::NodeListSub"
)
rclcpp_components_register_nodes(SystemSub
  "SystemSub::MapSub::MapSub"
)

install(
  TARGETS SystemSub
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
