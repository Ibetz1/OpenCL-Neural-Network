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

    void Kernel::push_to_device(CommandQueue& queue, USZ& local_size, const USZ& global_size) {
        // maybe move outside of function
        local_size = get_work_group_size(queue.get_context().get_device());

        U32 wait_list_size = 0;
        if (bound_event != NULL) ++wait_list_size;

        S32 status = clEnqueueNDRangeKernel(
            queue.get_queue(), 
            kernel, 
            1,      // dimensions 
            NULL,   // offset
            &global_size, 
            &local_size, 
            wait_list_size, 
            bound_event, 
            &internal_event
        );

        if (status != CL_SUCCESS) {
            THROW("failed to execute kernel %i", status);
        }
    }

    USZ Kernel::get_work_group_size(Device& device) const {
        USZ local_size = 0;

        S32 status = clGetKernelWorkGroupInfo(kernel, device.get_id(), CL_KERNEL_WORK_GROUP_SIZE, sizeof(USZ), &local_size, NULL);

        if (status != CL_SUCCESS) {
            THROW("failed to get work group info");
        }

        return local_size;
    }

    cl_kernel& Kernel::get_kernel() {
        return kernel;
    }

    void Kernel::chain(Kernel& other) {
        if (bound_event != NULL) {
            THROW("kernel already chained");
        }

        bound_event = other.get_internal_event();
    }

    cl_event* Kernel::get_internal_event() {
        return &internal_event;
    }

    cl_event* Kernel::get_bound_event() {
        return bound_event;
    }

};