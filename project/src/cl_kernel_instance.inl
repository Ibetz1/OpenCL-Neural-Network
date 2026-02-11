#ifndef _CL_KERNEL_INL
#define _CL_KERNEL_INL

namespace OpenCL {

    template <typename T>
    void KernelInstance::bind_param(USZ idx, const T& val) {
        S32 status = clSetKernelArg(kernel, idx, sizeof(T), &val);
        if (status != CL_SUCCESS) {
            THROW("failed to pass kernel argument %i", status);
        }
    }

};

#endif