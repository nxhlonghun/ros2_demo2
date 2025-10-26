cmake_minimum_required(VERSION 3.8)
project(demo_coo_qt2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(demo_interface REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets )

file(GLOB PRO_FORM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui)
file(GLOB_RECURSE PRO_INCLUDE_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/*.hpp *.h )

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

add_executable(sys_sub_dispaly 
      src/sys_sub_dispaly.cpp 
      src/widget.cpp 
      src/sys_node.cpp
      src/sys_sub_node.cpp
      src/sys_control.cpp
      src/sys_node_list.cpp
      ${PRO_FORM_DIR} ${QT_UI_HPP} ${PRO_INCLUDE_DIR} ${QT_MOC_HPP}
)

ament_target_dependencies(sys_sub_dispaly rclcpp demo_interface rclcpp_lifecycle )
target_link_libraries(sys_sub_dispaly Qt6::Widgets )
#target_include_directories(sys_sub_dispaly PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
#target_link_libraries(widget Qt6::Widgets Qt6::Core Qt6::Gui)
#target_include_directories(widget PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS sys_sub_dispaly
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
