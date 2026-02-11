#ifndef _CL_BUFFER_HPP
#define _CL_BUFFER_HPP

namespace OpenCL {

    template <typename T>
    struct CommandBuffer {
    private:
        Context& context;
        std::vector<cl_mem> buffers;
        USZ size = 0;
        USZ count = 2;
        U8* output = nullptr;

    public:
        CommandBuffer(Context& ctx, USZ size, USZ count = 2);
        ~CommandBuffer();

        /*
            push data onto a command queue from the command buffer
        */
        void queue_data(Queue& queue, USZ buffer_id, T* data, USZ length);

        /*
            read data popped from output buffer
        */
        void read_data(Queue& queue, USZ buffer_id, USZ length);

        /*
            flushes the output buffer
        */
        void flush();

        /*
            returns input buffer at index
        */
        cl_mem& operator[](USZ idx);

        /*
            get the current output buffer
        */
        T*& get_output_buffer();

        /*
            size of the command buffer
        */
        U32 get_size() const;
    };

};

#include "cl_buffer.inl"
#endif