#ifndef MAKINA_CORE_ENGINE_HPP_
#define MAKINA_CORE_ENGINE_HPP_

#include <algorithm>
#include <memory>
#include <type_traits>
#include <vector>

#include <makina/export.hpp>
#include <makina/core/frame_timer.hpp>
#include <makina/core/logger.hpp>
#include <makina/core/scene.hpp>
#include <makina/core/system.hpp>

namespace mak
{
// An engine is a container of and is responsible for the updating of systems.
class MAKINA_EXPORT engine
{
public:
  engine           ()                    = default;
  engine           (const engine&  that) = delete ;
  engine           (      engine&& temp) = default;
  virtual ~engine  ()                    = default;
  engine& operator=(const engine&  that) = delete ;
  engine& operator=(      engine&& temp) = default;

  template<typename system_type, typename... system_arguments>
  system_type* add_system   (system_arguments&&... arguments)
  {
    static_assert(std::is_base_of<system, system_type>::value, "The type does not inherit from system.");

    systems_.push_back(std::make_unique<system_type>(arguments...));
    return static_cast<system_type*>(systems_.back().get());
  }
  template<typename system_type>
  system_type* get_system   ()
  {
    static_assert(std::is_base_of<system, system_type>::value, "The type does not inherit from system.");

    auto iterator = std::find_if(systems_.begin(), systems_.end(), system_type_predicate<system_type>);
    if  (iterator == systems_.end())
    {
      logger->warn("System does not exist within the engine.");
      return nullptr;
    }
    return static_cast<system_type*>(iterator->get());
  }
  template<typename system_type>
  void         remove_system()
  {
    static_assert(std::is_base_of<system, system_type>::value, "The type does not inherit from system.");

    auto iterator = std::remove_if(systems_.begin(), systems_.end(), system_type_predicate<system_type>);
    if  (iterator == systems_.end())
    {
      logger->warn("System does not exist within the engine.");
      return;
    }
    systems_.erase(iterator, systems_.end());
  }

  void run       ();
  void stop      ();
  bool is_running() const;

protected:
  template<typename system_type>
  static bool system_type_predicate(const std::unique_ptr<system>& iteratee)
  {
    return typeid(system_type) == typeid(*iteratee.get());
  }

  std::vector<std::unique_ptr<system>> systems_    ;
  scene                                scene_      ;
  frame_timer                          frame_timer_;
  bool                                 is_running_ = false;
};
}

#endif