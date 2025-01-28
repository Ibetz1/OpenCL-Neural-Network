#include "main.hpp"

namespace OpenCL {
    CommandQueue::CommandQueue(Context& ctx) : context(ctx) {
        S32 status = CL_SUCCESS;
        queue = clCreateCommandQueueWithProperties(ctx.get_context(), ctx.get_device(), 0, &status);
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
};