#include "main.hpp"

namespace OpenCL {

    Context::Context(Device& dev) : device(dev) {
        S32 status = 0;
        context = clCreateContext(0, 1, &device.get_id(), NULL, NULL, &status);
        if (status != CL_SUCCESS) {
            THROW("failed to create OpenCL context: sts %i", status);
        } else {
            LOGI("created OpenCL context");
        }
    }

    Context::~Context() {
        clReleaseContext(context);
        LOGI("released OpenCL context");
    }

    Device& Context::get_device() {
        return device;
    }

    cl_context& Context::get_context() {
        return context;
    }

};