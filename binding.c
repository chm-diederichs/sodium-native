#include <node_api.h>
#include <assert.h>
#include "macros.h"
#include <sodium.h>

napi_value sn_randombytes_random (napi_env env, napi_callback_info info) {
  uint32_t upp

  napi_value result;
  assert(napi_create_uint32(env, randombytes_random(), &result) == napi_ok);
  return result;
}

napi_value sn_randombytes_uniform (napi_env env, napi_callback_info info) {
  NAPI_ARGV(1, randombytes_uniform);
  NAPI_TYPE_ASSERT(upper_bound, argv[0], napi_number, "upper_bound must be a Number");

  uint32_t upper_bound;
  assert(napi_get_value_uint32(env, argv[0], &upper_bound) == napi_ok);

  napi_value result;
  assert(napi_create_uint32(env, randombytes_uniform(upper_bound), &result) == napi_ok);
  return result;
}

uint8_t typedarray_width(napi_typedarray_type type) {
  switch (type) {
    case napi_int8_array: return 1;
    case napi_uint8_array: return 1;
    case napi_uint8_clamped_array: return 1;
    case napi_int16_array: return 2;
    case napi_uint16_array: return 2;
    case napi_int32_array: return 4;
    case napi_uint32_array: return 4;
    case napi_float32_array: return 4;
    case napi_float64_array: return 8;
    case napi_bigint64_array: return 8;
    case napi_biguint64_array: return 8;
    default: return 0;
  }
}

napi_value sn_sodium_memcmp(napi_env env, napi_callback_info info) {
  NAPI_ARGV(2, sodium_memcmp);
  NAPI_TYPEDARRAY_ASSERT(b1, argv[0], "b1 must be instance of TypedArray");
  NAPI_TYPEDARRAY_ASSERT(b2, argv[1], "b1 must be instance of TypedArray");

  NAPI_TYPEDARRAY(b1, argv[0])
  NAPI_TYPEDARRAY(b2, argv[1])

  NAPI_THROWS(b1_size != b2_size, "buffers must be of same length");

  int cmp = sodium_memcmp(b1_data, b2_data, b1_size);

  napi_value result;
  assert(napi_get_boolean(env, cmp == 0, &result) == napi_ok);
  return result;
}

napi_value sn_sodium_increment(napi_env env, napi_callback_info info) {
  NAPI_ARGV(1, sodium_increment);
  NAPI_TYPEDARRAY_ASSERT(n, argv[0], "n must be an instance of TypedArray");

  NAPI_TYPEDARRAY(n, argv[0]);

  sodium_increment(n_data, n_size);

  return NULL;
}

napi_value sn_sodium_add(napi_env env, napi_callback_info info) {
  NAPI_ARGV(2, sodium_add);
  NAPI_TYPEDARRAY_ASSERT(a, argv[0], "a must be an instance of TypedArray");
  NAPI_TYPEDARRAY_ASSERT(b, argv[1], "b must be an instance of TypedArray");

  NAPI_TYPEDARRAY(a, argv[0]);
  NAPI_TYPEDARRAY(b, argv[1]);

  NAPI_THROWS(a_size != b_size, "buffers must be of same length");
  sodium_add(a_data, b_data, a_size);

  return NULL;
}

napi_value sn_sodium_sub(napi_env env, napi_callback_info info) {
  NAPI_ARGV(2, sodium_sub);
  NAPI_TYPEDARRAY_ASSERT(a, argv[0], "a must be an instance of TypedArray");
  NAPI_TYPEDARRAY_ASSERT(b, argv[1], "b must be an instance of TypedArray");

  NAPI_TYPEDARRAY(a, argv[0]);
  NAPI_TYPEDARRAY(b, argv[1]);

  NAPI_THROWS(a_size != b_size, "buffers must be of same length");
  sodium_sub(a_data, b_data, a_size);

  return NULL;
}

napi_value sn_sodium_compare(napi_env env, napi_callback_info info) {
  NAPI_ARGV(2, sodium_compare);
  NAPI_TYPEDARRAY_ASSERT(a, argv[0], "a must be an instance of TypedArray");
  NAPI_TYPEDARRAY_ASSERT(b, argv[1], "b must be an instance of TypedArray");

  NAPI_TYPEDARRAY(a, argv[0]);
  NAPI_TYPEDARRAY(b, argv[1]);

  NAPI_THROWS(a_size != b_size, "buffers must be of same length");
  int cmp = sodium_compare(a_data, b_data, a_size);

  napi_value result;
  napi_create_int32(env, cmp, &result);

  return result;
}

napi_value sn_sodium_is_zero(napi_env env, napi_callback_info info) {
  NAPI_ARGV(1, sodium_is_zero);
  NAPI_TYPEDARRAY_ASSERT(a, argv[0], "a must be an instance of TypedArray");

  NAPI_TYPEDARRAY(a, argv[0]);

  sodium_is_zero(a_data, a_size);
  int cmp = sodium_is_zero(a_data, a_size);

  napi_value result;
  assert(napi_get_boolean(env, cmp == 1, &result) == napi_ok);
  return result;
}

napi_value create_sodium_native(napi_env env) {
  napi_value exports;
  assert(napi_create_object(env, &exports) == napi_ok);

  NAPI_EXPORT_FUNCTION(randombytes_uniform, sn_randombytes_uniform)
  NAPI_EXPORT_FUNCTION(randombytes_random, sn_randombytes_random)
  NAPI_EXPORT_FUNCTION(sodium_memcmp, sn_sodium_memcmp)
  NAPI_EXPORT_FUNCTION(sodium_increment, sn_sodium_increment)
  NAPI_EXPORT_FUNCTION(sodium_add, sn_sodium_add)
  NAPI_EXPORT_FUNCTION(sodium_sub, sn_sodium_sub)
  NAPI_EXPORT_FUNCTION(sodium_compare, sn_sodium_compare)
  NAPI_EXPORT_FUNCTION(sodium_is_zero, sn_sodium_is_zero)

  return exports;
}

static napi_value Init(napi_env env, napi_value exports) {
  return create_sodium_native(env);
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
