#ifndef _CL_PROGRAM_HPP
#define _CL_PROGRAM_HPP

namespace OpenCL {

    struct Program {
    private:
        Context& context;
        cl_program program = nullptr;
        USZ program_len = 0;

    public:
        Program();
        Program(Context& ctx, const char* source);
        ~Program();

        /*
            returns openCL program instance reference
        */
        cl_program& get_program();

        /*
            reads file into a buffer, this will allocate to the size of the file
        */
        static char* read_file(const char* path);
    };

};

#endif