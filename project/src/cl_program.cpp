#include "main.hpp"

namespace OpenCL {

    Program::Program(Context& ctx, const char* source) : context(ctx) {
        S32 status = 0;

        // create the program
        program_len = strlen(source);
        program = clCreateProgramWithSource(ctx.get_context(), 1, &source, &program_len, &status);
        if (status != CL_SUCCESS) {
            THROW("failed to create OpenCL program %i", status);
        } else {
            LOGI("created openCL program");
        }

        // build the program
        status = clBuildProgram(program, 1, &ctx.get_device(), NULL, NULL, NULL);
        if (status != CL_SUCCESS) {
            char output[KB(4)] = { 0 };
            USZ output_size = 0;
            clGetProgramInfo(program, CL_PROGRAM_BUILD_LOG, sizeof(output), output, &output_size);
            THROW("Program build error: %s", output);
        } else {
            LOGI("built openCL program");
        }
    }

    Program::~Program() {
        clReleaseProgram(program);
        LOGI("released kernel program");
    }

    cl_program& Program::get_program() {
        return program;
    }

};