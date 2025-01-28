__kernel void square(
   __global float* input,
   __global float* output,
   const unsigned int count
)
{
   int i = get_global_id(0);
   if(i < count)
       output[i] = input[i] * input[i];
}

__kernel void mul(
   __global float* input,
   __global float* output,
   const unsigned int count
)
{
   int i = get_global_id(0);
   if(i < count)
       output[i] = 2 * input[i];
}

__kernel void add(
   __global float* input,
   __global float* output,
   const unsigned int count
)
{
   int i = get_global_id(0);
   if(i < count)
       output[i] = 123 + input[i];
}