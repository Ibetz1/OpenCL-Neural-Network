// OpenCL kernel. Each work item takes care of one element of c
__kernel void vecAdd(  __global double *a,                    
                       __global double *b,                   
                       __global double *c,                     
                       const unsigned int n)                   
{                                                             
    int id = get_global_id(0);
    if (id < n)                                                
        c[id] = a[id] + b[id];
}