#include <makina/audio/fmod_context.hpp>

#include <mutex>

namespace mak
{
FMOD::System* fmod_context()
{
  static FMOD::System*  context  ;
  static std::once_flag once_flag;
  std::call_once(once_flag, [ ] 
  {
    FMOD::System_Create(&context);
    context->init(1024, FMOD_INIT_NORMAL, nullptr);
  });
  return context;
}
}