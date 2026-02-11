#ifndef _CL_CONTEXT_HPP
#define _CL_CONTEXT_HPP

namespace OpenCL {

    struct Context {
    private:
        cl_context context = nullptr;
        const Device& device;

    public:
        Context(const Device& dev);
        ~Context();

        /*
            returns the OpenCL device ID from its contained device
        */
        const cl_device_id& get_device_id() const;

        /*
            returns the OpenCL context instance
        */
        const cl_context& get_context() const;
    };

};

#endif