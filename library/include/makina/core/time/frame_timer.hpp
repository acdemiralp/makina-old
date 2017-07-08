#ifndef MAKINA_CORE_TIME_FRAME_TIMER_HPP_
#define MAKINA_CORE_TIME_FRAME_TIMER_HPP_

#include <chrono>

#include <makina_export.hpp>

namespace mak
{
template<typename precision = float>
class frame_timer final
{
  using clock      = std::chrono::high_resolution_clock;
  using duration   = std::chrono::duration<precision>;
  using time_point = std::chrono::time_point<clock, duration>;

public:
  void tick();

  template<typename unit = std::chrono::seconds>
  precision delta_time() const;

private:
  duration   delta_time_;
  time_point time_      ;
};

template <typename precision>
void frame_timer<precision>::tick()
{
  auto time   = clock::now();
  delta_time_ = time - time_;
  time_       = time;
}

template <typename precision>
template <typename unit>
precision frame_timer<precision>::delta_time() const
{
  return std::chrono::duration_cast<unit>(delta_time_).count();
}
}

#endif