#include "main.hpp"

namespace OpenCL {

    Kernel::Kernel(Program& program, const char* name) : 
        program(program)
    {
        S32 status = 0;

        kernel = clCreateKernel(program.get_program(), name, &status);
        if (status != CL_SUCCESS) {
            THROW("failed to create kernel");
        } else {
            LOGI("created kernel");
        }
    }

    Kernel::~Kernel() {
        LOGI("released kernel");
        clReleaseKernel(kernel);
    }

    cl_kernel& Kernel::get_kernel() {
        return kernel;
    }

};