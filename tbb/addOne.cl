__kernel void addOne( __global int* input,  const unsigned int count)
{
   unsigned int i = get_global_id(0);
   if(i < count)
       input[i]+=1;
}
