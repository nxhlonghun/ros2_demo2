// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from demo_interface:srv/Partol.idl
// generated code does not contain a copyright notice
#include "demo_interface/srv/detail/partol__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
demo_interface__srv__Partol_Request__init(demo_interface__srv__Partol_Request * msg)
{
  if (!msg) {
    return false;
  }
  // target_x
  // target_y
  return true;
}

void
demo_interface__srv__Partol_Request__fini(demo_interface__srv__Partol_Request * msg)
{
  if (!msg) {
    return;
  }
  // target_x
  // target_y
}

bool
demo_interface__srv__Partol_Request__are_equal(const demo_interface__srv__Partol_Request * lhs, const demo_interface__srv__Partol_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // target_x
  if (lhs->target_x != rhs->target_x) {
    return false;
  }
  // target_y
  if (lhs->target_y != rhs->target_y) {
    return false;
  }
  return true;
}

bool
demo_interface__srv__Partol_Request__copy(
  const demo_interface__srv__Partol_Request * input,
  demo_interface__srv__Partol_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // target_x
  output->target_x = input->target_x;
  // target_y
  output->target_y = input->target_y;
  return true;
}

demo_interface__srv__Partol_Request *
demo_interface__srv__Partol_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  demo_interface__srv__Partol_Request * msg = (demo_interface__srv__Partol_Request *)allocator.allocate(sizeof(demo_interface__srv__Partol_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(demo_interface__srv__Partol_Request));
  bool success = demo_interface__srv__Partol_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
demo_interface__srv__Partol_Request__destroy(demo_interface__srv__Partol_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    demo_interface__srv__Partol_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
demo_interface__srv__Partol_Request__Sequence__init(demo_interface__srv__Partol_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  demo_interface__srv__Partol_Request * data = NULL;

  if (size) {
    data = (demo_interface__srv__Partol_Request *)allocator.zero_allocate(size, sizeof(demo_interface__srv__Partol_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = demo_interface__srv__Partol_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        demo_interface__srv__Partol_Request__fini(&data[i - 1]);
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
demo_interface__srv__Partol_Request__Sequence__fini(demo_interface__srv__Partol_Request__Sequence * array)
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
      demo_interface__srv__Partol_Request__fini(&array->data[i]);
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

demo_interface__srv__Partol_Request__Sequence *
demo_interface__srv__Partol_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  demo_interface__srv__Partol_Request__Sequence * array = (demo_interface__srv__Partol_Request__Sequence *)allocator.allocate(sizeof(demo_interface__srv__Partol_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = demo_interface__srv__Partol_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
demo_interface__srv__Partol_Request__Sequence__destroy(demo_interface__srv__Partol_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    demo_interface__srv__Partol_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
demo_interface__srv__Partol_Request__Sequence__are_equal(const demo_interface__srv__Partol_Request__Sequence * lhs, const demo_interface__srv__Partol_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!demo_interface__srv__Partol_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
demo_interface__srv__Partol_Request__Sequence__copy(
  const demo_interface__srv__Partol_Request__Sequence * input,
  demo_interface__srv__Partol_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(demo_interface__srv__Partol_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    demo_interface__srv__Partol_Request * data =
      (demo_interface__srv__Partol_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!demo_interface__srv__Partol_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          demo_interface__srv__Partol_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!demo_interface__srv__Partol_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


bool
demo_interface__srv__Partol_Response__init(demo_interface__srv__Partol_Response * msg)
{
  if (!msg) {
    return false;
  }
  // result
  return true;
}

void
demo_interface__srv__Partol_Response__fini(demo_interface__srv__Partol_Response * msg)
{
  if (!msg) {
    return;
  }
  // result
}

bool
demo_interface__srv__Partol_Response__are_equal(const demo_interface__srv__Partol_Response * lhs, const demo_interface__srv__Partol_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // result
  if (lhs->result != rhs->result) {
    return false;
  }
  return true;
}

bool
demo_interface__srv__Partol_Response__copy(
  const demo_interface__srv__Partol_Response * input,
  demo_interface__srv__Partol_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // result
  output->result = input->result;
  return true;
}

demo_interface__srv__Partol_Response *
demo_interface__srv__Partol_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  demo_interface__srv__Partol_Response * msg = (demo_interface__srv__Partol_Response *)allocator.allocate(sizeof(demo_interface__srv__Partol_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(demo_interface__srv__Partol_Response));
  bool success = demo_interface__srv__Partol_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
demo_interface__srv__Partol_Response__destroy(demo_interface__srv__Partol_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    demo_interface__srv__Partol_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
demo_interface__srv__Partol_Response__Sequence__init(demo_interface__srv__Partol_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  demo_interface__srv__Partol_Response * data = NULL;

  if (size) {
    data = (demo_interface__srv__Partol_Response *)allocator.zero_allocate(size, sizeof(demo_interface__srv__Partol_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = demo_interface__srv__Partol_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        demo_interface__srv__Partol_Response__fini(&data[i - 1]);
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
demo_interface__srv__Partol_Response__Sequence__fini(demo_interface__srv__Partol_Response__Sequence * array)
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
      demo_interface__srv__Partol_Response__fini(&array->data[i]);
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

demo_interface__srv__Partol_Response__Sequence *
demo_interface__srv__Partol_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  demo_interface__srv__Partol_Response__Sequence * array = (demo_interface__srv__Partol_Response__Sequence *)allocator.allocate(sizeof(demo_interface__srv__Partol_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = demo_interface__srv__Partol_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
demo_interface__srv__Partol_Response__Sequence__destroy(demo_interface__srv__Partol_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    demo_interface__srv__Partol_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
demo_interface__srv__Partol_Response__Sequence__are_equal(const demo_interface__srv__Partol_Response__Sequence * lhs, const demo_interface__srv__Partol_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!demo_interface__srv__Partol_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
demo_interface__srv__Partol_Response__Sequence__copy(
  const demo_interface__srv__Partol_Response__Sequence * input,
  demo_interface__srv__Partol_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(demo_interface__srv__Partol_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    demo_interface__srv__Partol_Response * data =
      (demo_interface__srv__Partol_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!demo_interface__srv__Partol_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          demo_interface__srv__Partol_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!demo_interface__srv__Partol_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
