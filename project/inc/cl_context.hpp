#ifndef _CL_CONTEXT_HPP
#define _CL_CONTEXT_HPP

namespace OpenCL {

    struct Context {
    private:
        cl_context context = nullptr;
        Device& device;

    public:
        Context(Device& dev);
        ~Context();

        /*
            returns the OpenCL device ID from its contained device
        */
        cl_device_id& get_device();

        /*
            returns the OpenCL context instance
        */
        cl_context& get_context();
    };

};

#endif