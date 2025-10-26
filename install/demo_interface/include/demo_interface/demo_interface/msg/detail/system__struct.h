// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from demo_interface:msg/System.idl
// generated code does not contain a copyright notice

#ifndef DEMO_INTERFACE__MSG__DETAIL__SYSTEM__STRUCT_H_
#define DEMO_INTERFACE__MSG__DETAIL__SYSTEM__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'node_name'
// Member 'result'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/System in the package demo_interface.
typedef struct demo_interface__msg__System
{
  rosidl_runtime_c__String node_name;
  rosidl_runtime_c__String result;
} demo_interface__msg__System;

// Struct for a sequence of demo_interface__msg__System.
typedef struct demo_interface__msg__System__Sequence
{
  demo_interface__msg__System * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} demo_interface__msg__System__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // DEMO_INTERFACE__MSG__DETAIL__SYSTEM__STRUCT_H_
