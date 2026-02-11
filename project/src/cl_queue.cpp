#include "main.hpp"

namespace OpenCL {
    Queue::Queue(const Context& ctx) : context(ctx) {
        S32 status = CL_SUCCESS;
        queue = clCreateCommandQueueWithProperties(ctx.get_context(), ctx.get_device_id(), 0, &status);
        if (status != CL_SUCCESS) {
            THROW("failed to create command queue: %i", status);
        } else {
            LOGI("created command queue");
        }
    }

    Queue::~Queue() {
        clReleaseCommandQueue(queue);
    }

    cl_command_queue& Queue::get_queue() {
        return queue;
    }

    void Queue::wait() {
        S32 status = clFinish(queue);
        if (status != CL_SUCCESS) {
            THROW("Failed to finish command queue %i", status);
        }
    }

    /*
        returns queue context
    */  
    const Context& Queue::get_context() const {
        return context;
    }

    // void Queue::push_to_kernel(KernelInstance& kernel, USZ& local_size, USZ& global_size) {
    //     S32 status = clGetKernelWorkGroupInfo(kernel.get_kernel(), context.get_device(), CL_KERNEL_WORK_GROUP_SIZE, sizeof(USZ), &local_size, NULL);

    //     if (status != CL_SUCCESS) {
    //         THROW("failed to get work group info");
    //     }

    //     status = clEnqueueNDRangeKernel(queue, kernel.get_kernel(), 1, NULL, &global_size, &local_size, 0, kernel.get_internal_event(), kernel.get_bound_event());

    //     if (status != CL_SUCCESS) {
    //         THROW("failed to execute kernel");
    //     }

    //     clFinish(queue);
    // }
};