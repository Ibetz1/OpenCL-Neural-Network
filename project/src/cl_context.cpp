#include "main.hpp"

namespace OpenCL {
    Context::Context(const Device& dev) : device(dev) {
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

    const cl_device_id& Context::get_device_id() const {
        return device.get_id();
    }

    const cl_context& Context::get_context() const {
        return context;
    }

};