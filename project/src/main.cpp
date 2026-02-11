#include "main.hpp"

using namespace OpenCL;

struct Kernel {
    const Device& device;
    Context* context;
    Queue* queue;
    Program* program;
    char* kernel_source;
    std::unordered_map<const char*, KernelInstance> kernels;

    Kernel(const Device& device, const char* source_path) : 
        device(device) 
    {
        context = new Context(device);
        queue = new Queue(*context);
        kernel_source = Program::read_file(source_path);
        program = new Program(*context, kernel_source);
    }

    ~Kernel() {
        free(kernel_source);
        delete program;
        delete queue;
        delete context;
    }

    /*
        kernel management
    */

    void load_kernel(const char* name) {
        KernelInstance kernel(*program, name);
        kernels.insert({name, kernel});
    }

    KernelInstance& find_kernel(const char* name) {
        auto it = kernels.find(name);
        if (it == kernels.end()) {
            THROW("failed to find kernel in kernel program");
        }
        return it->second;
    }

    KernelInstance& operator[](const char* name) {
        return find_kernel(name);
    }


    template <typename T>
    void bind_param(const char* name, USZ idx, const T& param) {
        KernelInstance& kernel = find_kernel(name);
        kernel.bind_param(idx, param);
    }

    /*
        buffer management
    */

    template <typename T>
    CommandBuffer<T> create_buffer(const USZ& size) {
        return CommandBuffer<T>(*context, size);
    }

    /*
        work load management
    */

    void push_work_load(const char* name, USZ &local_size, const USZ &global_size) {
        KernelInstance& kernel = find_kernel(name);
        kernel.push_to_device(*queue, local_size, global_size);
    }
};

/*
    ideal usage:

    Device device(Device::GPU);
    ClProgram cl_progam(device, "program.cl"); // creates a cl_program for creating work loads

    WorkLoad work_load = cl_program.create_work_load(size);
    CommandBuffer buf = work_load.new_buffer<F32>(3); // pass number of cl_buffers
    
    work_load.add_kernel("square"); // will create a kernel "square"
    work_load.push_param("square", buf[0]);
    work_load.push_param("square", buf[1]);
    work_load.push_param("square", buf.get_size());

    work_load.add_kernel("add"); // will create another kernel "add"
    work_load.push_param("add", buf[1]);
    work_load.push_param("add", buf[2]);
    work_load.push_param("add", buf.get_size());

    // ... more kernel creation for the work load

    work_load.push_mmap(data, data_size);
    work_load.read_mmap(buf, 2);
    work_load.wait();

    F32* output = work_load.get_output();

    // ... do stuff with buffer data

    work_load.flush();

    ###
    # this chaining system might not be the play
    ###

    KernelChain chain = work_load.kernel_chain({
        "square",
        "add",
        ...
    });

    // optional
    chain.push_param("square", buf[0]);
    chain.push_param("square", buf[1]);
    // ...

    chain.push_mmap(data, data_size);
    chain.read_mmap(buf, 2);
    chain.wait();

    // ... same as usual just with chain

    chain.flush();
*/

int main() {

    // data

    USZ global_work_size = KB(10);
    USZ local_work_size = 0;
    F32 global_data[global_work_size] = { 0 };
    for (int i = 0; i < global_work_size; i++) {
        global_data[i] = ((F32) i) / global_work_size;
    }

    // openCL
    Device device = Device(Device::GPU);
    Kernel kernel(device, "square.cl");
    CommandBuffer buffer = kernel.create_buffer<F32>(global_work_size);
    
    kernel.load_kernel("square");
    kernel.bind_param("square", 0, buffer[0]);
    kernel.bind_param("square", 1, buffer[1]);
    kernel.bind_param("square", 2, buffer.get_size());

    // run kernel chain
    buffer.queue_data(*kernel.queue, 0, global_data, global_work_size); // TODO: add kernel wrapper
    kernel.push_work_load("square", local_work_size, global_work_size);
    buffer.read_data(*kernel.queue, 1, global_work_size); // TODO: add kernel wrapper
    kernel.queue->wait(); // TODO: add kernel wrapper

    F32* data = buffer.get_output_buffer(); // TODO: add kernel wrapper

    U32 same = 0;
    for (int i = 0; i < global_work_size; i++) {
        if (data[i] == global_data[i] * global_data[i]) {
            ++same;
        }
    }

    LOGI("got %i/%i same values", same, global_work_size);

    buffer.flush();
}