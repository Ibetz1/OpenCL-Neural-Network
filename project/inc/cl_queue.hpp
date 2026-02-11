#ifndef _CL_QUEUE_HPP
#define _CL_QUEUE_HPP

namespace OpenCL {

    struct Queue {
    private:
        const Context& context;
        cl_command_queue queue = nullptr;

    public:
        Queue(const Context& ctx);
        ~Queue();

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
        const Context& get_context() const;
    };

};

#endif