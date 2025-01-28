#include "main.hpp"

using namespace OpenCL;

/*
    TODO:

    make work group class containing:

    command queue
    context
    kernel
    command buffer
    and an execution interface

    workflow:
        KernelSource source("path/to/source");                   << loads kernel source from file
        WorkGroup work_group<[type]>(size);                      << creates a work group of a given size
        work_group.load_kernel([name], <KernelSource> [source]); << loads a kernel with a name from source code
        work_group.push([name], [size]);                         << pushes data to kernel inside of work group
        const [type]* out = work_group.pop();                    << reads data from work group output

        // ...
*/

int main() {

    // data

    USZ global_work_size = KB(10);
    USZ local_work_size = 0;
    F32 global_data[global_work_size] = { 0 };
    for (int i = 0; i < global_work_size; i++) {
        global_data[i] = ((F32) i) / global_work_size;
    }

    const char* kernelSource = 
    "__kernel void square(                   \n" \
    "   __global float* input,               \n" \
    "   __global float* output,              \n" \
    "   const unsigned int count)            \n" \
    "{                                       \n" \
    "   int i = get_global_id(0);            \n" \
    "   if(i < count)                        \n" \
    "       output[i] = input[i] * input[i]; \n" \
    "}                                       \n" \
    "\n";

    // openCL

    Device device = Device();
    device.load_platforms();
    device.select(0, Device::GPU);

    Context context(device);
    CommandQueue queue(context);
    Program program(context, kernelSource);
    Kernel kernel(program, "square");

    CommandBuffer<F32> buffer(context, global_work_size);
    buffer.queue_data(queue, global_data, global_work_size);
    buffer.bind_kernel(kernel);
    buffer.push_to_kernel(kernel, queue, local_work_size, global_work_size);
    buffer.read_data(queue, global_work_size);

    F32* data = buffer.get_output_buffer();

    U32 same = 0;
    for (int i = 0; i < global_work_size; i++) {
        if (data[i] == global_data[i] * global_data[i]) {
            ++same;
        }
    }

    LOGI("got %i/%i same values", same, global_work_size);

}