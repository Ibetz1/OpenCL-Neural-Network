#ifndef _MAIN_HPP
#define _MAIN_HPP

#define CL_TARGET_OPENCL_VERSION 200

#include <CL/cl.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LOGI(fmt, ...) do { printf("I: " fmt "\n",##__VA_ARGS__); } while (0)
#define LOGE(fmt, ...) do { printf("E: " fmt "\n",##__VA_ARGS__); } while (0)
#define THROW(fmt, ...) do { LOGE(__FILE__ " " fmt, ##__VA_ARGS__); exit(1); } while (0)

typedef size_t USZ;
typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef int8_t SS8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;
typedef float F32;
typedef double F64;

#include "cl_kernel.hpp"
#include "cl_device.hpp"

#endif