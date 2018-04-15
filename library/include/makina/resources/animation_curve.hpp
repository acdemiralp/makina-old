#ifndef MAKINA_RESOURCES_ANIMATION_CURVE_HPP_
#define MAKINA_RESOURCES_ANIMATION_CURVE_HPP_

#include <vector>

#include <makina/resources/keyframe.hpp>
#include <makina/export.hpp>

namespace mak
{
template<typename type>
struct MAKINA_EXPORT animation_curve
{
  type evaluate(type time)
  {
    keyframe<type>* start_frame = nullptr;
    keyframe<type>* end_frame   = nullptr;
    for (auto i = 0; i < keyframes.size(); ++i)
    {
      if (keyframes[i].time < time && i < keyframes.size() - 1)
      {
        start_frame = &keyframes[i];
        end_frame   = &keyframes[i + 1];
        break;
      }
    }
    if (!start_frame || !end_frame) return type(0);

    // Evaluate Cubic Hermitian spline.
    auto l  = end_frame->time - start_frame->time;
    auto t  = (time - start_frame->time) / l;
    auto t2 = std::pow(t, type(2));
    auto t3 = std::pow(t, type(3));
    auto h0 =  type(2) * t3 - type(3) * t2 + type(1);
    auto h1 =  t3 - type(2) * t2 + t;
    auto h2 =  t3 - t2;
    auto h3 = -type(2) * t3 + type(3) * t2;
    return h0 * start_frame->value           +
           h1 * start_frame->out_tangent * l +
           h2 * end_frame  ->in_tangent  * l +
           h3 * end_frame  ->value           ;
  }

  std::vector<keyframe<type>> keyframes;
};
}

#endif