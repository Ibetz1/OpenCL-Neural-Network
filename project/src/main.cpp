#include "main.hpp"

using namespace OpenCL;

/*
    TODO:

    make work group class containing:

    command queue
    context
    sqr_kernel
    command buffer
    and an execution interface

    workflow:
        KernelSource source("path/to/source");                   << loads sqr_kernel source from file
        WorkGroup work_group<[type]>(size);                      << creates a work group of a given size
        work_group.load_kernel([name], <KernelSource> [source]); << loads a sqr_kernel with a name from source code
        work_group.push([name], [size]);                         << pushes data to sqr_kernel inside of work group
        const [type]* out = work_group.pop();                    << reads data from work group output

        // ...
*/

template <typename T>
struct KernelGroup {
    const Device& device;
    Context context;
    CommandQueue queue;
    Program program;
    USZ global_size = 0;
    USZ local_size = 0;
    char* source = nullptr;

    // YUCK
    KernelGroup(Device& device, const char* source_path, USZ buffer_size) : 
        device(device), 
        context(Context(device)),
        queue(CommandQueue(context)),
        global_size(buffer_size),
        source(Program::read_file(source_path)),
        program(Program(context, source))
    {}

    ~KernelGroup() {
        free(source);
    }

    void create_kernel() {

    }

    void configure_kernel() {

    }
};

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

    char* kernelSource = Program::read_file("square.cl");

    Context context(device);
    CommandQueue queue(context);
    Program program(context, kernelSource);

    CommandBuffer<F32> buffer(context, global_work_size, 4);

    // make square kernel
    Kernel sqr_kernel(program, "square");
    buffer.bind_kernel(sqr_kernel, 0, buffer[0]);
    buffer.bind_kernel(sqr_kernel, 1, buffer[1]);
    buffer.bind_kernel(sqr_kernel, 2, buffer.get_size());

    // make mul kernel
    Kernel mul_kernel(program, "mul");
    buffer.bind_kernel(mul_kernel, 0, buffer[1]);
    buffer.bind_kernel(mul_kernel, 1, buffer[2]);
    buffer.bind_kernel(mul_kernel, 2, buffer.get_size());

    // make add kernel
    Kernel add_kernel(program, "add");
    buffer.bind_kernel(add_kernel, 0, buffer[2]);
    buffer.bind_kernel(add_kernel, 1, buffer[3]);
    buffer.bind_kernel(add_kernel, 2, buffer.get_size());

    mul_kernel.chain(sqr_kernel); // chair mul to square
    add_kernel.chain(mul_kernel); // chain add to mul

    /*
        run square kernel
    */
    buffer.queue_data(queue, 0, global_data, global_work_size);
    sqr_kernel.push_to_device(queue, local_work_size, global_work_size);
    mul_kernel.push_to_device(queue, local_work_size, global_work_size);
    add_kernel.push_to_device(queue, local_work_size, global_work_size);
    buffer.read_data(queue, 3, global_work_size);
    queue.wait();

    F32* data = buffer.get_output_buffer();

    U32 same = 0;
    for (int i = 0; i < global_work_size; i++) {
        if (data[i] == 123 + 2 * global_data[i] * global_data[i]) {
            ++same;
        }
    }

    LOGI("got %i/%i same values", same, global_work_size);
    buffer.flush();
}