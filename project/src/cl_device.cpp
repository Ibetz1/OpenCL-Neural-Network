#include "main.hpp"

namespace OpenCL {

    Device::Device() {
        load_platforms();
    }
    
    Device::Device(cl_device_type type, U32 platform) {
        load_platforms();
        select(platform, type);
    }

    Device::~Device() {
        if (platforms != nullptr) {
            free(platforms);
        }
    }

    void Device::load_platforms() {

        // get number of platforms
        S32 status = clGetPlatformIDs(0, NULL, &num_platforms);

        if (status != CL_SUCCESS || num_platforms == 0) {
            THROW("no OpenCL platforms found cnt: %i", num_platforms);
        }

        // load in platform ids
        platforms = (cl_platform_id*) malloc(num_platforms);
        clGetPlatformIDs(num_platforms, platforms, NULL);

        for (U32 i = 0; i < num_platforms; i++) {
            char platform_name[256] = { 0 };
            clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(platform_name), platform_name, NULL);
            LOGI("found platform %s: idx %i", platform_name, i);
        }
    }

    void Device::select(U32 platform, cl_device_type type) {
        if (platforms == nullptr) {
            THROW("platforms have not been loaded");
        }

        if (platform >= num_platforms) {
            THROW("invalid device platform");
        }

        S32 status = clGetDeviceIDs(platforms[platform], type, 1, &device, &num_devices);

        if (status != CL_SUCCESS) {
            THROW("unable to find platform ID: sts: %i cnt: %i", status, num_devices);
        } else {
            LOGI("selected OpenCL device %i platform");
        }
    }

    const cl_device_id& Device::get_id() const {
        return device;
    }

    cl_platform_id& Device::get_platform(U32 platform) {
        if (platform >= num_platforms) {
            THROW("invalid device platform");
        }
        return platforms[platform];
    }

};