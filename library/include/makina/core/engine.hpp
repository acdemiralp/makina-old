#ifndef MAKINA_CORE_ENGINE_HPP
#define MAKINA_CORE_ENGINE_HPP

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

#include <makina/core/entity_component_system/scene.hpp>
#include <makina/core/entity_component_system/system.hpp>
#include <makina/core/time/frame_timer.hpp>

#include <makina_export.hpp>

namespace mak
{
class MAKINA_EXPORT engine
{
public:
  engine()                    = default;
  engine(const engine&  that) = delete ;
  engine(      engine&& temp) = default;
  virtual ~engine()           = default;

  engine& operator=(const engine&  that) = delete ;
  engine& operator=(      engine&& temp) = default;

  std::vector<system*> systems() const;
  scene*               scene  () const;
  
  void  set_scene    (std::unique_ptr<mak::scene> scene);
           
  template <typename type, typename... args>
  type* add_system   (args&&...   arguments);
  template <typename type>
  type* copy_system  (const type& system   );
  template <typename type>
  type* get_system   () const;
  template <typename type>
  void  remove_system();
                   
  void  run          ();
  void  stop         ();
  bool  is_running   () const;
  
private:        
  void  initialize   ();
  void  update       (float time);
  void  terminate    ();
       
  frame_timer<float>                   frame_timer_;
  std::vector<std::unique_ptr<system>> systems_    ;
  std::unique_ptr<mak::scene>          scene_      ;
  bool                                 is_running_ = false;
};

template <typename type, typename ... args>
type* engine::add_system   (args&&...  arguments)
{
  systems_.emplace_back(new type(arguments...));
  auto system = systems_.back().get();
  system->owner_ = this;
  return static_cast<type*>(system);
}
template <typename type>
type* engine::copy_system  (const type& system  )
{
  systems_.emplace_back(new type(system));
  auto new_system = systems_.back().get();
  new_system->owner_ = this;
  return static_cast<type*>(new_system);
}
template <typename type>
type* engine::get_system   () const
{
  auto it = std::find_if(systems_.begin(), systems_.end(), 
  [](const std::unique_ptr<system>& iteratee)
  {
    return typeid(type) == typeid(*iteratee.get());
  });
  return it != systems_.end() ? static_cast<type*>(it->get()) : nullptr;
}
template <typename type>
void  engine::remove_system()
{
  systems_.erase(std::remove_if(systems_.begin(), systems_.end(), 
  [] (const std::unique_ptr<system>& iteratee)
  {
    return typeid(type) == typeid(*iteratee.get());
  }), systems_.end());
}
}

#endif
