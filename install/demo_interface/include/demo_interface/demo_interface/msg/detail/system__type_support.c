// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from demo_interface:msg/System.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "demo_interface/msg/detail/system__rosidl_typesupport_introspection_c.h"
#include "demo_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "demo_interface/msg/detail/system__functions.h"
#include "demo_interface/msg/detail/system__struct.h"


// Include directives for member types
// Member `node_name`
// Member `result`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void demo_interface__msg__System__rosidl_typesupport_introspection_c__System_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  demo_interface__msg__System__init(message_memory);
}

void demo_interface__msg__System__rosidl_typesupport_introspection_c__System_fini_function(void * message_memory)
{
  demo_interface__msg__System__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember demo_interface__msg__System__rosidl_typesupport_introspection_c__System_message_member_array[2] = {
  {
    "node_name",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(demo_interface__msg__System, node_name),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "result",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(demo_interface__msg__System, result),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers demo_interface__msg__System__rosidl_typesupport_introspection_c__System_message_members = {
  "demo_interface__msg",  // message namespace
  "System",  // message name
  2,  // number of fields
  sizeof(demo_interface__msg__System),
  demo_interface__msg__System__rosidl_typesupport_introspection_c__System_message_member_array,  // message members
  demo_interface__msg__System__rosidl_typesupport_introspection_c__System_init_function,  // function to initialize message memory (memory has to be allocated)
  demo_interface__msg__System__rosidl_typesupport_introspection_c__System_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t demo_interface__msg__System__rosidl_typesupport_introspection_c__System_message_type_support_handle = {
  0,
  &demo_interface__msg__System__rosidl_typesupport_introspection_c__System_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_demo_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, demo_interface, msg, System)() {
  if (!demo_interface__msg__System__rosidl_typesupport_introspection_c__System_message_type_support_handle.typesupport_identifier) {
    demo_interface__msg__System__rosidl_typesupport_introspection_c__System_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &demo_interface__msg__System__rosidl_typesupport_introspection_c__System_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
