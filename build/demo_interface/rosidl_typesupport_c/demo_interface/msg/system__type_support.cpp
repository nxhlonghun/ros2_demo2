// generated from rosidl_typesupport_c/resource/idl__type_support.cpp.em
// with input from demo_interface:msg/System.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "demo_interface/msg/detail/system__struct.h"
#include "demo_interface/msg/detail/system__type_support.h"
#include "rosidl_typesupport_c/identifier.h"
#include "rosidl_typesupport_c/message_type_support_dispatch.h"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_c/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace demo_interface
{

namespace msg
{

namespace rosidl_typesupport_c
{

typedef struct _System_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _System_type_support_ids_t;

static const _System_type_support_ids_t _System_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _System_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _System_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _System_type_support_symbol_names_t _System_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, demo_interface, msg, System)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, demo_interface, msg, System)),
  }
};

typedef struct _System_type_support_data_t
{
  void * data[2];
} _System_type_support_data_t;

static _System_type_support_data_t _System_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _System_message_typesupport_map = {
  2,
  "demo_interface",
  &_System_message_typesupport_ids.typesupport_identifier[0],
  &_System_message_typesupport_symbol_names.symbol_name[0],
  &_System_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t System_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_System_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_c

}  // namespace msg

}  // namespace demo_interface

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, demo_interface, msg, System)() {
  return &::demo_interface::msg::rosidl_typesupport_c::System_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
