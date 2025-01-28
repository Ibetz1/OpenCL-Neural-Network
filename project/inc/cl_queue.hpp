#ifndef _CL_QUEUE_HPP
#define _CL_QUEUE_HPP

namespace OpenCL {

    struct CommandQueue {
    private:
        Context& context;
        cl_command_queue queue = nullptr;

    public:
        CommandQueue(Context& ctx);
        ~CommandQueue();

        /*
            returns the OpenCL instance of the internal queue
        */
        cl_command_queue& get_queue();

        /*
            waits for queue
        */
        void wait();

        /*
            returns queue context
        */  
        Context& get_context();
    };

};

#endif