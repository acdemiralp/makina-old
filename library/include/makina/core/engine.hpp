#ifndef MAKINA_CORE_ENGINE_HPP_
#define MAKINA_CORE_ENGINE_HPP_

#include <makina/export.hpp>
#include <makina/core/frame_timer.hpp>

namespace mak
{
class MAKINA_EXPORT engine
{
public:
  engine           ()                    = default;
  engine           (const engine&  that) = default;
  engine           (      engine&& temp) = default;
  virtual ~engine  ()                    = default;
  engine& operator=(const engine&  that) = default;
  engine& operator=(      engine&& temp) = default;

protected:
  frame_timer<float, std::milli> frame_timer_;
};
}

#endif