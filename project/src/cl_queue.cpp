#include "main.hpp"

namespace OpenCL {
    CommandQueue::CommandQueue(Context& ctx) : context(ctx) {
        S32 status = CL_SUCCESS;
        queue = clCreateCommandQueueWithProperties(ctx.get_context(), ctx.get_device().get_id(), 0, &status);
        if (status != CL_SUCCESS) {
            THROW("failed to create command queue: %i", status);
        } else {
            LOGI("created command queue");
        }
    }

    CommandQueue::~CommandQueue() {
        clReleaseCommandQueue(queue);
    }

    cl_command_queue& CommandQueue::get_queue() {
        return queue;
    }

    void CommandQueue::wait() {
        S32 status = clFinish(queue);
        if (status != CL_SUCCESS) {
            THROW("Failed to finish command queue %i", status);
        }
    }

    /*
        returns queue context
    */  
    Context& CommandQueue::get_context() {
        return context;
    }

    // void CommandQueue::push_to_kernel(Kernel& kernel, USZ& local_size, USZ& global_size) {
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