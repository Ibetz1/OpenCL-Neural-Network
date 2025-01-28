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
        status = clBuildProgram(program, 1, &ctx.get_device().get_id(), NULL, NULL, NULL);
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

    char* Program::read_file(const char* path) {
        // get kernal source
        FILE* file = fopen(path, "rb+");
        if (!file) {
            THROW("failed to open file %s", path);
        }

        // get file content
        struct stat file_stats;
        
        if (fstat(fileno(file), &file_stats) != 0) {
            fclose(file);
            THROW("failed to get file stats: %s", path);
        }

        long fsize = file_stats.st_size;
        char* buf = (char*) malloc(fsize);

        if (!buf) {
            fclose(file);
            THROW("no mem");
        }

        fread(buf, 1, fsize, file);
        buf[fsize] = '\0';
        fclose(file);

        return buf;
    }

};