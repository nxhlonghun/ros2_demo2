// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from demo_interface:msg/System.idl
// generated code does not contain a copyright notice

#ifndef DEMO_INTERFACE__MSG__DETAIL__SYSTEM__FUNCTIONS_H_
#define DEMO_INTERFACE__MSG__DETAIL__SYSTEM__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "demo_interface/msg/rosidl_generator_c__visibility_control.h"

#include "demo_interface/msg/detail/system__struct.h"

/// Initialize msg/System message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * demo_interface__msg__System
 * )) before or use
 * demo_interface__msg__System__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_demo_interface
bool
demo_interface__msg__System__init(demo_interface__msg__System * msg);

/// Finalize msg/System message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_demo_interface
void
demo_interface__msg__System__fini(demo_interface__msg__System * msg);

/// Create msg/System message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * demo_interface__msg__System__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_demo_interface
demo_interface__msg__System *
demo_interface__msg__System__create();

/// Destroy msg/System message.
/**
 * It calls
 * demo_interface__msg__System__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_demo_interface
void
demo_interface__msg__System__destroy(demo_interface__msg__System * msg);

/// Check for msg/System message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_demo_interface
bool
demo_interface__msg__System__are_equal(const demo_interface__msg__System * lhs, const demo_interface__msg__System * rhs);

/// Copy a msg/System message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_demo_interface
bool
demo_interface__msg__System__copy(
  const demo_interface__msg__System * input,
  demo_interface__msg__System * output);

/// Initialize array of msg/System messages.
/**
 * It allocates the memory for the number of elements and calls
 * demo_interface__msg__System__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_demo_interface
bool
demo_interface__msg__System__Sequence__init(demo_interface__msg__System__Sequence * array, size_t size);

/// Finalize array of msg/System messages.
/**
 * It calls
 * demo_interface__msg__System__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_demo_interface
void
demo_interface__msg__System__Sequence__fini(demo_interface__msg__System__Sequence * array);

/// Create array of msg/System messages.
/**
 * It allocates the memory for the array and calls
 * demo_interface__msg__System__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_demo_interface
demo_interface__msg__System__Sequence *
demo_interface__msg__System__Sequence__create(size_t size);

/// Destroy array of msg/System messages.
/**
 * It calls
 * demo_interface__msg__System__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_demo_interface
void
demo_interface__msg__System__Sequence__destroy(demo_interface__msg__System__Sequence * array);

/// Check for msg/System message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_demo_interface
bool
demo_interface__msg__System__Sequence__are_equal(const demo_interface__msg__System__Sequence * lhs, const demo_interface__msg__System__Sequence * rhs);

/// Copy an array of msg/System messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_demo_interface
bool
demo_interface__msg__System__Sequence__copy(
  const demo_interface__msg__System__Sequence * input,
  demo_interface__msg__System__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // DEMO_INTERFACE__MSG__DETAIL__SYSTEM__FUNCTIONS_H_
