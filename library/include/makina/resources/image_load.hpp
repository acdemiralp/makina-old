#ifndef MAKINA_RESOURCES_IMAGE_LOAD_HPP_
#define MAKINA_RESOURCES_IMAGE_LOAD_HPP_

#include <ra/load.hpp>

#include <cstdint>
#include <optional>
#include <string>

#include <makina/core/logger.hpp>
#include <makina/resources/image.hpp>

struct image_description
{
  std::string  filepath      ;
  std::uint8_t bits_per_pixel;
};

template<>
inline void ra::load(const std::string&       filepath   , std::optional<mak::image>* image)
{
  if (!image)
  {
    mak::logger->error("Failed to load image: Output is nullptr.");
    return;
  }

  image->emplace(filepath);
  image->value().to_32_bits();
}
template<>
inline void ra::load(const image_description& description, std::optional<mak::image>* image)
{
  if (!image)
  {
    mak::logger->error("Failed to load image: Output is nullptr.");
    return;
  }

  image->emplace(description.filepath);

  if      (description.bits_per_pixel == 4 ) image->value().to_4_bits     ();
  else if (description.bits_per_pixel == 8 ) image->value().to_8_bits     ();
  else if (description.bits_per_pixel == 16) image->value().to_16_bits_555();
  else if (description.bits_per_pixel == 24) image->value().to_24_bits    ();
  else                                       image->value().to_32_bits    ();
}

#endif