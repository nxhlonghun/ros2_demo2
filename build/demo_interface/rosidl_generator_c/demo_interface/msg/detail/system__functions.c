// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from demo_interface:msg/System.idl
// generated code does not contain a copyright notice
#include "demo_interface/msg/detail/system__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `node_name`
// Member `result`
#include "rosidl_runtime_c/string_functions.h"

bool
demo_interface__msg__System__init(demo_interface__msg__System * msg)
{
  if (!msg) {
    return false;
  }
  // node_name
  if (!rosidl_runtime_c__String__init(&msg->node_name)) {
    demo_interface__msg__System__fini(msg);
    return false;
  }
  // result
  if (!rosidl_runtime_c__String__init(&msg->result)) {
    demo_interface__msg__System__fini(msg);
    return false;
  }
  return true;
}

void
demo_interface__msg__System__fini(demo_interface__msg__System * msg)
{
  if (!msg) {
    return;
  }
  // node_name
  rosidl_runtime_c__String__fini(&msg->node_name);
  // result
  rosidl_runtime_c__String__fini(&msg->result);
}

bool
demo_interface__msg__System__are_equal(const demo_interface__msg__System * lhs, const demo_interface__msg__System * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // node_name
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->node_name), &(rhs->node_name)))
  {
    return false;
  }
  // result
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->result), &(rhs->result)))
  {
    return false;
  }
  return true;
}

bool
demo_interface__msg__System__copy(
  const demo_interface__msg__System * input,
  demo_interface__msg__System * output)
{
  if (!input || !output) {
    return false;
  }
  // node_name
  if (!rosidl_runtime_c__String__copy(
      &(input->node_name), &(output->node_name)))
  {
    return false;
  }
  // result
  if (!rosidl_runtime_c__String__copy(
      &(input->result), &(output->result)))
  {
    return false;
  }
  return true;
}

demo_interface__msg__System *
demo_interface__msg__System__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  demo_interface__msg__System * msg = (demo_interface__msg__System *)allocator.allocate(sizeof(demo_interface__msg__System), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(demo_interface__msg__System));
  bool success = demo_interface__msg__System__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
demo_interface__msg__System__destroy(demo_interface__msg__System * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    demo_interface__msg__System__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
demo_interface__msg__System__Sequence__init(demo_interface__msg__System__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  demo_interface__msg__System * data = NULL;

  if (size) {
    data = (demo_interface__msg__System *)allocator.zero_allocate(size, sizeof(demo_interface__msg__System), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = demo_interface__msg__System__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        demo_interface__msg__System__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
demo_interface__msg__System__Sequence__fini(demo_interface__msg__System__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      demo_interface__msg__System__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

demo_interface__msg__System__Sequence *
demo_interface__msg__System__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  demo_interface__msg__System__Sequence * array = (demo_interface__msg__System__Sequence *)allocator.allocate(sizeof(demo_interface__msg__System__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = demo_interface__msg__System__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
demo_interface__msg__System__Sequence__destroy(demo_interface__msg__System__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    demo_interface__msg__System__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
demo_interface__msg__System__Sequence__are_equal(const demo_interface__msg__System__Sequence * lhs, const demo_interface__msg__System__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!demo_interface__msg__System__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
demo_interface__msg__System__Sequence__copy(
  const demo_interface__msg__System__Sequence * input,
  demo_interface__msg__System__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(demo_interface__msg__System);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    demo_interface__msg__System * data =
      (demo_interface__msg__System *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!demo_interface__msg__System__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          demo_interface__msg__System__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!demo_interface__msg__System__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
