// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from demo_interface:msg/System.idl
// generated code does not contain a copyright notice

#ifndef DEMO_INTERFACE__MSG__DETAIL__SYSTEM__STRUCT_HPP_
#define DEMO_INTERFACE__MSG__DETAIL__SYSTEM__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__demo_interface__msg__System __attribute__((deprecated))
#else
# define DEPRECATED__demo_interface__msg__System __declspec(deprecated)
#endif

namespace demo_interface
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct System_
{
  using Type = System_<ContainerAllocator>;

  explicit System_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->node_name = "";
      this->result = "";
    }
  }

  explicit System_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : node_name(_alloc),
    result(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->node_name = "";
      this->result = "";
    }
  }

  // field types and members
  using _node_name_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _node_name_type node_name;
  using _result_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _result_type result;

  // setters for named parameter idiom
  Type & set__node_name(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->node_name = _arg;
    return *this;
  }
  Type & set__result(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->result = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    demo_interface::msg::System_<ContainerAllocator> *;
  using ConstRawPtr =
    const demo_interface::msg::System_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<demo_interface::msg::System_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<demo_interface::msg::System_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      demo_interface::msg::System_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<demo_interface::msg::System_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      demo_interface::msg::System_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<demo_interface::msg::System_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<demo_interface::msg::System_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<demo_interface::msg::System_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__demo_interface__msg__System
    std::shared_ptr<demo_interface::msg::System_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__demo_interface__msg__System
    std::shared_ptr<demo_interface::msg::System_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const System_ & other) const
  {
    if (this->node_name != other.node_name) {
      return false;
    }
    if (this->result != other.result) {
      return false;
    }
    return true;
  }
  bool operator!=(const System_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct System_

// alias to use template instance with default allocator
using System =
  demo_interface::msg::System_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace demo_interface

#endif  // DEMO_INTERFACE__MSG__DETAIL__SYSTEM__STRUCT_HPP_
