#ifndef _CL_KERNAL_HPP
#define _CL_KERNAL_HPP

namespace OpenCL {

    struct Kernel {
    private:
        Program& program;
        cl_kernel kernel = nullptr;
        cl_event internal_event = NULL;
        cl_event* bound_event = NULL;

    public:
        Kernel(Program& program, const char* name);
        ~Kernel();

        /*
            binds to another kernel for chaining
        */
        void chain(Kernel& binding);

        /*
            pushes queue to kernel running on device
        */
        void push_to_device(CommandQueue& queue, USZ& local_size, const USZ& global_size);

        /*
            returns size of the work group
        */
        USZ get_work_group_size(Device& device) const;

        /*
            returns openCL instance of the kernel
        */
        cl_kernel& get_kernel();

        /*
            returns pointer to internal event
        */
        cl_event* get_internal_event();

        /*
            returns pointer to event binding
        */
        cl_event* get_bound_event();



    };

};

#endif