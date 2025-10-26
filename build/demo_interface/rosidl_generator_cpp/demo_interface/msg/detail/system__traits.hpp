// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from demo_interface:msg/System.idl
// generated code does not contain a copyright notice

#ifndef DEMO_INTERFACE__MSG__DETAIL__SYSTEM__TRAITS_HPP_
#define DEMO_INTERFACE__MSG__DETAIL__SYSTEM__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "demo_interface/msg/detail/system__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace demo_interface
{

namespace msg
{

inline void to_flow_style_yaml(
  const System & msg,
  std::ostream & out)
{
  out << "{";
  // member: node_name
  {
    out << "node_name: ";
    rosidl_generator_traits::value_to_yaml(msg.node_name, out);
    out << ", ";
  }

  // member: result
  {
    out << "result: ";
    rosidl_generator_traits::value_to_yaml(msg.result, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const System & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: node_name
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "node_name: ";
    rosidl_generator_traits::value_to_yaml(msg.node_name, out);
    out << "\n";
  }

  // member: result
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "result: ";
    rosidl_generator_traits::value_to_yaml(msg.result, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const System & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace demo_interface

namespace rosidl_generator_traits
{

[[deprecated("use demo_interface::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const demo_interface::msg::System & msg,
  std::ostream & out, size_t indentation = 0)
{
  demo_interface::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use demo_interface::msg::to_yaml() instead")]]
inline std::string to_yaml(const demo_interface::msg::System & msg)
{
  return demo_interface::msg::to_yaml(msg);
}

template<>
inline const char * data_type<demo_interface::msg::System>()
{
  return "demo_interface::msg::System";
}

template<>
inline const char * name<demo_interface::msg::System>()
{
  return "demo_interface/msg/System";
}

template<>
struct has_fixed_size<demo_interface::msg::System>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<demo_interface::msg::System>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<demo_interface::msg::System>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // DEMO_INTERFACE__MSG__DETAIL__SYSTEM__TRAITS_HPP_
