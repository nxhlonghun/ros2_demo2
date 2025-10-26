// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from demo_interface:srv/FaceDetector.idl
// generated code does not contain a copyright notice

#ifndef DEMO_INTERFACE__SRV__DETAIL__FACE_DETECTOR__BUILDER_HPP_
#define DEMO_INTERFACE__SRV__DETAIL__FACE_DETECTOR__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "demo_interface/srv/detail/face_detector__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace demo_interface
{

namespace srv
{

namespace builder
{

class Init_FaceDetector_Request_image
{
public:
  Init_FaceDetector_Request_image()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::demo_interface::srv::FaceDetector_Request image(::demo_interface::srv::FaceDetector_Request::_image_type arg)
  {
    msg_.image = std::move(arg);
    return std::move(msg_);
  }

private:
  ::demo_interface::srv::FaceDetector_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::demo_interface::srv::FaceDetector_Request>()
{
  return demo_interface::srv::builder::Init_FaceDetector_Request_image();
}

}  // namespace demo_interface


namespace demo_interface
{

namespace srv
{

namespace builder
{

class Init_FaceDetector_Response_left
{
public:
  explicit Init_FaceDetector_Response_left(::demo_interface::srv::FaceDetector_Response & msg)
  : msg_(msg)
  {}
  ::demo_interface::srv::FaceDetector_Response left(::demo_interface::srv::FaceDetector_Response::_left_type arg)
  {
    msg_.left = std::move(arg);
    return std::move(msg_);
  }

private:
  ::demo_interface::srv::FaceDetector_Response msg_;
};

class Init_FaceDetector_Response_bottom
{
public:
  explicit Init_FaceDetector_Response_bottom(::demo_interface::srv::FaceDetector_Response & msg)
  : msg_(msg)
  {}
  Init_FaceDetector_Response_left bottom(::demo_interface::srv::FaceDetector_Response::_bottom_type arg)
  {
    msg_.bottom = std::move(arg);
    return Init_FaceDetector_Response_left(msg_);
  }

private:
  ::demo_interface::srv::FaceDetector_Response msg_;
};

class Init_FaceDetector_Response_right
{
public:
  explicit Init_FaceDetector_Response_right(::demo_interface::srv::FaceDetector_Response & msg)
  : msg_(msg)
  {}
  Init_FaceDetector_Response_bottom right(::demo_interface::srv::FaceDetector_Response::_right_type arg)
  {
    msg_.right = std::move(arg);
    return Init_FaceDetector_Response_bottom(msg_);
  }

private:
  ::demo_interface::srv::FaceDetector_Response msg_;
};

class Init_FaceDetector_Response_top
{
public:
  explicit Init_FaceDetector_Response_top(::demo_interface::srv::FaceDetector_Response & msg)
  : msg_(msg)
  {}
  Init_FaceDetector_Response_right top(::demo_interface::srv::FaceDetector_Response::_top_type arg)
  {
    msg_.top = std::move(arg);
    return Init_FaceDetector_Response_right(msg_);
  }

private:
  ::demo_interface::srv::FaceDetector_Response msg_;
};

class Init_FaceDetector_Response_use_time
{
public:
  explicit Init_FaceDetector_Response_use_time(::demo_interface::srv::FaceDetector_Response & msg)
  : msg_(msg)
  {}
  Init_FaceDetector_Response_top use_time(::demo_interface::srv::FaceDetector_Response::_use_time_type arg)
  {
    msg_.use_time = std::move(arg);
    return Init_FaceDetector_Response_top(msg_);
  }

private:
  ::demo_interface::srv::FaceDetector_Response msg_;
};

class Init_FaceDetector_Response_number
{
public:
  Init_FaceDetector_Response_number()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FaceDetector_Response_use_time number(::demo_interface::srv::FaceDetector_Response::_number_type arg)
  {
    msg_.number = std::move(arg);
    return Init_FaceDetector_Response_use_time(msg_);
  }

private:
  ::demo_interface::srv::FaceDetector_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::demo_interface::srv::FaceDetector_Response>()
{
  return demo_interface::srv::builder::Init_FaceDetector_Response_number();
}

}  // namespace demo_interface

#endif  // DEMO_INTERFACE__SRV__DETAIL__FACE_DETECTOR__BUILDER_HPP_
