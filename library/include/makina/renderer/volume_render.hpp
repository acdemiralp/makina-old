#ifndef MAKINA_RENDERER_VOLUME_RENDER_HPP_
#define MAKINA_RENDERER_VOLUME_RENDER_HPP_

#include <makina/resources/field.hpp>
#include <makina/resources/transfer_function.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT volume_render
{
  field<float, 3>*   volume           ;
  transfer_function* transfer_function;
};
}

#endif