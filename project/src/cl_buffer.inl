#ifndef _CL_BUFFER_INL
#define _CL_BUFFER_INL

namespace OpenCL {

    // template <typename T>
    // CommandBuffer<T>::

    template <typename T> 
    CommandBuffer<T>::CommandBuffer(Context& ctx, USZ size) : context(ctx), size(size) {
        output = (T*) malloc(size * sizeof(T));

        // Create output memory buffer
        S32 status = 0;
        cl_output = clCreateBuffer(ctx.get_context(), CL_MEM_WRITE_ONLY, size * sizeof(T), NULL, &status);
        if (status != CL_SUCCESS) {
            THROW("failed to create write buffer: %i", status);
        } else {
            LOGI("created output buffer size %i", size);
        }

        // create input memory buffer
        cl_input = clCreateBuffer(ctx.get_context(), CL_MEM_READ_ONLY, size * sizeof(T), NULL, &status);
        if (status != CL_SUCCESS) {
            THROW("failed to create read buffer: %i", status);
        } else {
            LOGI("created input buffer size %i", size);
        }
    }

    template <typename T> 
    CommandBuffer<T>::~CommandBuffer() {
        clReleaseMemObject(cl_input);
        clReleaseMemObject(cl_output);

        free(output);

        LOGI("released command buffer");
    }

    template <typename T>
    void CommandBuffer<T>::queue_data(CommandQueue& queue, T* data, USZ length) {
        if (length > size) {
            THROW("command buffer overflow");
        }

        S32 status = 0;
        status = clEnqueueWriteBuffer(queue.get_queue(), cl_input, CL_TRUE, 0, sizeof(T) * length, data, 0, NULL, NULL);

        if (status != CL_SUCCESS) {
            LOGE("failed to queue kernel data");
        }
    }

    template <typename T>
    void CommandBuffer<T>::read_data(CommandQueue& queue, USZ length) {
        if (length > size) {
            THROW("command buffer overflow");
        }

        S32 status = 0;
        status = clEnqueueReadBuffer(queue.get_queue(), cl_output, CL_TRUE, 0, sizeof(T) * length, output, 0, NULL, NULL);
        if (status != CL_SUCCESS) {
            LOGE("failed to read kernel output");
        }
    }

    template <typename T>
    void CommandBuffer<T>::bind_kernel(Kernel& kernel) {
        S32 status = 0;

        status = clSetKernelArg(kernel.get_kernel(), 0, sizeof(cl_mem), &cl_input);
        if (status != CL_SUCCESS) {
            THROW("failed to pass kernel argument %i", status);
        }

        status = clSetKernelArg(kernel.get_kernel(), 1, sizeof(cl_mem), &cl_output);
        if (status != CL_SUCCESS) {
            THROW("failed to pass kernel argument %i", status);
        }

        status = clSetKernelArg(kernel.get_kernel(), 2, sizeof(U32), &size);
        if (status != CL_SUCCESS) {
            THROW("failed to pass kernel argument %i", status);
        }
    }

    template <typename T>
    void CommandBuffer<T>::push_to_kernel(Kernel& kernel, CommandQueue& queue, USZ& local_size, USZ& global_size) {
        S32 status = 0;

        status = clGetKernelWorkGroupInfo(kernel.get_kernel(), context.get_device(), CL_KERNEL_WORK_GROUP_SIZE, sizeof(USZ), &local_size, NULL);

        if (status != CL_SUCCESS) {
            THROW("failed to get work group info");
        }

        status = clEnqueueNDRangeKernel(queue.get_queue(), kernel.get_kernel(), 1, NULL, &global_size, &local_size, 0, NULL, NULL);

        if (status != CL_SUCCESS) {
            THROW("failed to execute kernel");
        }

        clFinish(queue.get_queue());
    }

    template <typename T>
    cl_mem& CommandBuffer<T>::get_output_mem() {
        return cl_output;
    }

    template <typename T>
    cl_mem& CommandBuffer<T>::get_input_mem() {
        return cl_input;
    }

    template <typename T>
    T*& CommandBuffer<T>::get_output_buffer() {
        return output;
    }

};

#endif