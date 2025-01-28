#ifndef _CL_BUFFER_HPP
#define _CL_BUFFER_HPP

namespace OpenCL {

    template <typename T>
    struct CommandBuffer {
    private:
        Context& context;
        cl_mem cl_input = nullptr;
        cl_mem cl_output = nullptr;
        USZ size = 0;
        T* output = nullptr;

    public:
        CommandBuffer(Context& ctx, USZ size);

        ~CommandBuffer();

        /*
            push data onto a command queue via the command buffer
        */
        void queue_data(CommandQueue& queue, T* data, USZ length);

        /*
            read data popped from output buffer
        */
        void read_data(CommandQueue& queue, USZ length);

        /*
            bind kernel to command buffer
        */
        void bind_kernel(Kernel& kernel);

        /*
            push data queued to kernel onto the kernel device
        */
        void push_to_kernel(Kernel& kernel, CommandQueue& queue, USZ& local_size, USZ& global_size);

        /*
            get the OpenCL memory instance of the output buffer
        */
        cl_mem& get_output_mem();

        /*
            get the OpenCL memory instance of the input buffer
        */
        cl_mem& get_input_mem();

        /*
            get the current output buffer
        */
        T*& get_output_buffer();
    };

};

#include "cl_buffer.inl"
#endif