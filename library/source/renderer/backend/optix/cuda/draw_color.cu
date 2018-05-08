#include <optix.h>
#include <optixu/optixu_math_namespace.h>

using namespace optix;

rtDeclareVariable(uint2 , launch_index, rtLaunchIndex, );
rtDeclareVariable(float3, color       ,              , );

rtBuffer<float4, 2> output;

RT_PROGRAM void draw_color()
{
  output[launch_index] = make_float4(color, 0.f);
}