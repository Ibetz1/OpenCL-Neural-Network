#ifndef _CL_DEVICE_HPP
#define _CL_DEVICE_HPP

namespace OpenCL {

    struct Device {
    private:
        cl_device_id device = nullptr;
        cl_platform_id* platforms = nullptr;
        U32 num_platforms = 0;
        U32 num_devices = 0;

    public:
        enum Type {
            GPU = CL_DEVICE_TYPE_GPU,
            CPU = CL_DEVICE_TYPE_CPU,
            ACCELERATOR = CL_DEVICE_TYPE_ACCELERATOR,
        };

        Device();
        ~Device();

        /*
            gets all available platforms on the PC
        */
        void load_platforms();

        /*
            selects a device type from an available platfrom (using its index from "load_platforms")
        */
        void select(U32 platform, cl_device_type type);

        /*
            returns the openCL device ID
        */
        cl_device_id& get_device();

        /*
            returns the openCL platform ID by an index
        */
        cl_platform_id& get_platform(U32 platform);
    };

};

#endif