#ifndef _CL_BUFFER_INL
#define _CL_BUFFER_INL

namespace OpenCL {

    // template <typename T>
    // CommandBuffer<T>::

    template <typename T> 
    CommandBuffer<T>::CommandBuffer(Context& ctx, USZ size, USZ count) : context(ctx), size(size), count(count) {
        if (count < 1) {
            THROW("command buffer needs minimum of 1 cl_buffers");
        }

        output = (T*) malloc(size * sizeof(T));

        // Create output memory buffer
        S32 status = 0;
        buffers.reserve(count);

        for (int i = 0; i < count; i++) {
            cl_mem buffer = clCreateBuffer(ctx.get_context(), CL_MEM_READ_ONLY, size * sizeof(T), NULL, &status);
            if (status != CL_SUCCESS) {
                THROW("failed to create read buffer: %i", status);
            } else {
                LOGI("added input buffer with size %i", size);
            }
            buffers.push_back(buffer);
        }
    }

    template <typename T> 
    CommandBuffer<T>::~CommandBuffer() {
        for (cl_mem cl_buffer : buffers) {
            clReleaseMemObject(cl_buffer);
        }

        free(output);

        LOGI("released command buffer");
    }

    template <typename T>
    void CommandBuffer<T>::queue_data(CommandQueue& queue, USZ buffer_id, T* data, USZ length) {
        if (length > size) {
            THROW("command buffer overflow");
        }

        if (buffer_id > count) {
            THROW("invalid buffer id");
        }

        S32 status = 0;
        cl_mem& buffer = buffers[buffer_id];
        status = clEnqueueWriteBuffer(queue.get_queue(), buffer, CL_TRUE, 0, sizeof(T) * length, data, 0, NULL, NULL);

        if (status != CL_SUCCESS) {
            LOGE("failed to queue kernel data");
        }
    }

    template <typename T>
    void CommandBuffer<T>::read_data(CommandQueue& queue, USZ buffer_id, USZ length) {
        if (length > size) {
            THROW("command buffer overflow");
        }

        if (buffer_id > count) {
            THROW("invalid buffer id");
        }

        cl_mem& buffer = buffers[buffer_id];
        S32 status = clEnqueueReadBuffer(queue.get_queue(), buffer, CL_TRUE, 0, sizeof(T) * length, output, 0, NULL, NULL);
        if (status != CL_SUCCESS) {
            LOGE("failed to read kernel output");
        }
    }

    template <typename T>
    void CommandBuffer<T>::flush() {
        memset(output, 0, sizeof(T) * size);
    }

    template <typename T>
    template <typename U>
    void CommandBuffer<T>::bind_kernel(Kernel& kernel, USZ idx, const U& val) const {
        S32 status = clSetKernelArg(kernel.get_kernel(), idx, sizeof(U), &val);
        if (status != CL_SUCCESS) {
            THROW("failed to pass kernel argument %i", status);
        }
    }

    template <typename T>
    cl_mem& CommandBuffer<T>::operator[](USZ buffer_id) {
        if (buffer_id > count) {
            THROW("invalid buffer id");
        }

        return buffers[buffer_id];
    }

    template <typename T>
    T*& CommandBuffer<T>::get_output_buffer() {
        return output;
    }

    template <typename T>
    U32 CommandBuffer<T>::get_size() const {
        return (U32) size;
    }
};

#endif