#include "main.hpp"

constexpr USZ BUFFER_LEN = 16;
constexpr USZ DATA_SIZE = BUFFER_LEN * sizeof(F32);
constexpr F32 initial_data[] = {
    0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.10, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16
};

char kernelSource[] = 
"__kernel void calcSin(global float *data) {\n"
"   int id = get_global_id(0);\n"
"   data[id] = sin(data[id]);\n"
"}\n"
;

void cpu_test() {
    LOGI("--cpu test--");

    F32 data[BUFFER_LEN] = { 0 };
    memcpy(data, initial_data, DATA_SIZE);

    for (int i = 0; i < BUFFER_LEN; i++) {
        data[i] = sin(data[i]);
    }

    for (int i = 0; i < BUFFER_LEN; i++) {
        printf("%.4f\n", data[i]);
    }
}

struct Device {
private:
    cl_device_id device = nullptr;
    cl_platform_id* platforms = nullptr;
    U32 num_platforms = 0;
    U32 num_devices = 0;

public:
    Device() {}
    ~Device() {
        if (platforms != nullptr) {
            free(platforms);
        }
    }

    void load_platforms() {

        // get number of platforms
        S32 status = clGetPlatformIDs(0, NULL, &num_platforms);

        if (status != CL_SUCCESS || num_platforms == 0) {
            THROW("no OpenCL platforms found cnt: %i", num_platforms);
        }

        // load in platform ids
        platforms = (cl_platform_id*) malloc(num_platforms);
        clGetPlatformIDs(num_platforms, platforms, NULL);

        for (U32 i = 0; i < num_platforms; i++) {
            char platform_name[256] = { 0 };
            clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(platform_name), platform_name, NULL);
            LOGI("found platform %s: idx %i", platform_name, i);
        }
    }

    void select(U32 platform, cl_device_type type) {
        if (platforms == nullptr) {
            THROW("platforms have not been loaded");
        }

        if (platform >= num_platforms) {
            THROW("invalid device platform");
        }

        S32 status = clGetDeviceIDs(platforms[platform], type, 1, &device, &num_devices);

        if (status != CL_SUCCESS) {
            THROW("unable to find platform ID: sts: %i cnt: %i", status, num_devices);
        } else {
            LOGI("selected OpenCL device %i platform");
        }
    }

    cl_device_id& get_device() {
        return device;
    }

    cl_platform_id& get_platform(U32 platform) {
        if (platform >= num_platforms) {
            THROW("invalid device platform");
        }
        return platforms[platform];
    }
};

struct Context {
private:
    cl_context context = nullptr;
    Device& device;

public:
    Context(Device& dev) : device(dev) {
        S32 status = 0;
        context = clCreateContext(NULL, 1, &device.get_device(), NULL, NULL, &status);
        if (status != CL_SUCCESS) {
            THROW("failed to create OpenCL context: sts %i", status);
        } else {
            LOGI("created OpenCL context");
        }
    }

    ~Context() {
        clReleaseContext(context);
        LOGI("released OpenCL context");
    }

    cl_device_id& get_device() const {
        return device.get_device();
    }

    cl_context& get_context() {
        return context;
    }
};

struct CommandQueue {
private:
    cl_command_queue queue;
    Context& context;

public:
    CommandQueue(Context& ctx) : context(ctx) {
        S32 status = CL_SUCCESS;
        cl_command_queue queue = clCreateCommandQueueWithProperties(ctx.get_context(), ctx.get_device(), 0, &status);
        if (status != CL_SUCCESS) {
            THROW("failed to create command queue: %i", status);
        } else {
            LOGI("created command queue");
        }
    }
    ~CommandQueue() {
        // is released with context
    }
};

template <typename T>
struct CommandBuffer {
private:
    Context& context;
    cl_mem cl_input = nullptr;
    cl_mem cl_output = nullptr;
    USZ size = 0;
    T* input = nullptr;
    T* output = nullptr;

public:
    CommandBuffer(Context& ctx, USZ size) : context(ctx) {
        input = (T*) malloc(size * sizeof(T));
        output = (T*) malloc(size * sizeof(T));

        // Create output memory buffer
        S32 status = 0;
        cl_output = clCreateBuffer(ctx.get_context(), CL_MEM_WRITE_ONLY, size * sizeof(T), NULL, &status);
        if (status != CL_SUCCESS) {
            THROW("failed to create write buffer: %i", status);
        } else {
            LOGI("created output buffer size %i", size);
        }

        // create input memory buffer
        cl_input = clCreateBuffer(ctx.get_context(), CL_MEM_READ_ONLY, size * sizeof(T), NULL, &status);
        if (status != CL_SUCCESS) {
            THROW("failed to create read buffer: %i", status);
        } else {
            LOGI("created input buffer size %i", size);
        }
    }

    ~CommandBuffer() {
        // release with context
        free(output);
        free(input);
    }
};

int main() {
    Device device = Device();
    device.load_platforms();
    device.select(0, CL_DEVICE_TYPE_GPU);

    Context context(device);
    CommandQueue queue(context);
    CommandBuffer<F32> commands(context, 16);

    // context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    // queue = clCreateCommandQueueWithProperties(context, device, (cl_command_queue_properties)0, NULL);

    // /*
    //     create and compile kernels
    // */
    // cl_program prgm;
    // cl_kernel kernel;

    // prgm = clCreateProgramWithSource(context, 1, (const char**) &kernelSource, NULL, NULL);
    // status = clBuildProgram(prgm, 0, NULL, NULL, NULL, NULL);

    // if (status != CL_SUCCESS) {
    //     size_t log_size = 0;
    //     clGetProgramBuildInfo(prgm, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
    //     printf("%i\n", log_size);
    //     // char log[log_size] = { 0 };
    //     // clGetProgramBuildInfo(prgm, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
    //     // THROW("CL_ERROR: %s", log);
    // }

    // kernel = clCreateKernel(prgm, "calcSin", NULL);

    // /*
    //     interface with kernel
    // */
    // cl_mem buffer;

    // F32 data[BUFFER_LEN] = { 0 };
    // memcpy(data, initial_data, DATA_SIZE);

    // buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, DATA_SIZE, NULL, NULL);
    // clEnqueueWriteBuffer(queue, buffer, CL_FALSE, 0, DATA_SIZE, data, 0, NULL, NULL);

    // // push buffer to kernal
    // clSetKernelArg(kernel, 0, sizeof(buffer), &buffer);
    // USZ global_dimensions[] = {BUFFER_LEN,0,0};
    // clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_dimensions, NULL, 0, NULL, NULL);
    // clEnqueueReadBuffer(queue, buffer, CL_FALSE, 0, sizeof(F32) * BUFFER_LEN, data, 0, NULL, NULL);

    // clFinish(queue);

    // cpu_test();

    // LOGI("--gpu test--");

    // for (int i = 0; i < BUFFER_LEN; i++) {
    //     printf("%.4f\n", data[i]);
    // }

    return 0;
}