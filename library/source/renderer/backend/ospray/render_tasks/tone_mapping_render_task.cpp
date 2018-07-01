#include <makina/renderer/backend/ospray/render_tasks/tone_mapping_render_task.hpp>

#include <ospray/ospray_cpp.h>

namespace mak
{
namespace ospray
{
fg::render_task<tone_mapping_task_data>* add_tone_mapping_render_task(renderer* renderer, framebuffer_resource* target, tone_mapping_settings settings)
{
  return renderer->add_render_task<tone_mapping_task_data>(
    "Tone Mapping Pass",
    [&] (      tone_mapping_task_data& data, fg::render_task_builder& builder)
    {
      data.target = builder.write(target);
    },
    [=] (const tone_mapping_task_data& data)
    {
      ::ospray::cpp::PixelOp tone_mapper("tonemapper");
      tone_mapper.set("contrast" , settings.contrast  );
      tone_mapper.set("shoulder" , settings.shoulder  );
      tone_mapper.set("midIn"    , settings.mid_in    );
      tone_mapper.set("midOut"   , settings.mid_out   );
      tone_mapper.set("hdrMax"   , settings.hdr_max   );
      tone_mapper.set("acesColor", settings.aces_color);
      data.target->actual()->setPixelOp(tone_mapper);
    });
}
}
}