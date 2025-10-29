cmake_minimum_required(VERSION 3.8)
project(demo_cpp_qt2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(demo_interface REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(rviz_common REQUIRED)
find_package(rviz_rendering REQUIRED)
find_package(rviz_default_plugins REQUIRED)
#find_package(ros_integration REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

file(GLOB PRO_FORM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui)
file(GLOB_RECURSE PRO_INCLUDE_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/*.hpp *.h )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

qt5_wrap_ui(QT_UI_HPP ${PRO_FORM_DIR})
qt5_wrap_cpp(QT_MOC_HPP ${PRO_INCLUDE_DIR})

include_directories(
        include  
        ${CMAKE_CURRENT_BINARY_DIR}    
)

add_executable(main 
      src/main.cpp 
      src/widget.cpp 
      src/sys_node.cpp
      src/SysSub.cpp
      src/RosNodeClass.cpp
      src/RosSpinThread.cpp
      ${PRO_FORM_DIR} ${QT_UI_HPP} ${PRO_INCLUDE_DIR} ${QT_MOC_HPP}
)

ament_target_dependencies(main 
  rclcpp demo_interface 
  rclcpp_lifecycle 
  rviz_common 
  rviz_rendering 
  rviz_default_plugins
  #ros_integration
)
target_link_libraries(main Qt5::Widgets )
#target_link_libraries(main ros_integration)
#target_include_directories(sys_sub_dispaly PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
#target_link_libraries(widget Qt6::Widgets Qt6::Core Qt6::Gui)
#target_include_directories(widget PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS main
  DESTINATION lib/${PROJECT_NAME}
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
