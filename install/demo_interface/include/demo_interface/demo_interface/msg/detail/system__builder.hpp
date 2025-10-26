// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from demo_interface:msg/System.idl
// generated code does not contain a copyright notice

#ifndef DEMO_INTERFACE__MSG__DETAIL__SYSTEM__BUILDER_HPP_
#define DEMO_INTERFACE__MSG__DETAIL__SYSTEM__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "demo_interface/msg/detail/system__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace demo_interface
{

namespace msg
{

namespace builder
{

class Init_System_result
{
public:
  explicit Init_System_result(::demo_interface::msg::System & msg)
  : msg_(msg)
  {}
  ::demo_interface::msg::System result(::demo_interface::msg::System::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::demo_interface::msg::System msg_;
};

class Init_System_node_name
{
public:
  Init_System_node_name()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_System_result node_name(::demo_interface::msg::System::_node_name_type arg)
  {
    msg_.node_name = std::move(arg);
    return Init_System_result(msg_);
  }

private:
  ::demo_interface::msg::System msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::demo_interface::msg::System>()
{
  return demo_interface::msg::builder::Init_System_node_name();
}

}  // namespace demo_interface

#endif  // DEMO_INTERFACE__MSG__DETAIL__SYSTEM__BUILDER_HPP_
