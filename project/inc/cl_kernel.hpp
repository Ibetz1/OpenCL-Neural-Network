#ifndef _CL_KERNAL_HPP
#define _CL_KERNAL_HPP

namespace OpenCL {

    struct Kernel {
    private:
        Program& program;
        cl_kernel kernel = nullptr;

    public:
        Kernel(Program& program, const char* name);
        ~Kernel();

        /*
            returns openCL instance of the kernel
        */
        cl_kernel& get_kernel();
    };

};

#endif