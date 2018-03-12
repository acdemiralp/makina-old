#ifndef MAKINA_CORE_FRAME_TIMER_HPP_
#define MAKINA_CORE_FRAME_TIMER_HPP_

#include <chrono>

#include <makina/export.hpp>

namespace mak
{
// Frame timer keeps track of time and the delta between two consecutive calls to tick().
template<typename type = float, typename period = std::milli>
class MAKINA_EXPORT _frame_timer
{
public:
  using clock      = std::chrono::high_resolution_clock;
  using duration   = std::chrono::duration<type, period>;
  using time_point = std::chrono::time_point<clock, duration>;

  _frame_timer           () : delta_time_(0), time_(clock::now()) { }
  _frame_timer           (const _frame_timer&  that) = default;
  _frame_timer           (      _frame_timer&& temp) = default;
 ~_frame_timer           ()                          = default;
  _frame_timer& operator=(const _frame_timer&  that) = default;
  _frame_timer& operator=(      _frame_timer&& temp) = default;
  
  void       tick        ()
  {
    auto time   = clock::now();
    delta_time_ = time - time_;
    time_       = time;
  }
                         
  duration   delta_time  () const
  {
    return delta_time_;
  }
  time_point time        () const
  {
    return time_;
  }

protected:
  duration   delta_time_;
  time_point time_      ;
};

using temporal_type   = float;
using temporal_period = std::milli;
using frame_timer     = _frame_timer<temporal_type, temporal_period>;
}

#endif