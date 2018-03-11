#ifndef MAKINA_CORE_FRAME_TIMER_HPP_
#define MAKINA_CORE_FRAME_TIMER_HPP_

#include <chrono>

#include <makina/export.hpp>

namespace mak
{
// Frame timer keeps track of time and the delta between two consecutive calls to tick().
template<typename type = float, typename period = std::milli>
class MAKINA_EXPORT frame_timer
{
public:
  using clock      = std::chrono::high_resolution_clock;
  using duration   = std::chrono::duration<type, period>;
  using time_point = std::chrono::time_point<clock, duration>;

  frame_timer           () : delta_time_(0), time_(clock::now()) { }
  frame_timer           (const frame_timer&  that) = default;
  frame_timer           (      frame_timer&& temp) = default;
 ~frame_timer           ()                         = default;
  frame_timer& operator=(const frame_timer&  that) = default;
  frame_timer& operator=(      frame_timer&& temp) = default;
  
  void       tick       ()
  {
    auto time   = clock::now();
    delta_time_ = time - time_;
    time_       = time;
  }
                        
  duration   delta_time () const
  {
    return delta_time_;
  }
  time_point time       () const
  {
    return time_;
  }

protected:
  duration   delta_time_;
  time_point time_      ;
};
}

#endif