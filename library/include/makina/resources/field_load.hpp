#ifndef MAKINA_RESOURCES_FIELD_LOAD_HPP_
#define MAKINA_RESOURCES_FIELD_LOAD_HPP_

#define H5_USE_BOOST

#include <fstream>
#include <optional>
#include <string>

#include <highfive/H5File.hpp>
#include <ra/load.hpp>

#include <makina/core/logger.hpp>
#include <makina/resources/field.hpp>

namespace mak
{
template <typename type, std::size_t dimensions>
struct MAKINA_EXPORT selection
{
  std::array<std::size_t, dimensions>        offset      ;
  std::array<std::size_t, dimensions>        size        ;
  std::array<std::size_t, dimensions>        stride      ;
};

template <typename type, std::size_t dimensions>
struct MAKINA_EXPORT hdf5_description
{
  std::string                                filepath    ;
  std::string                                dataset_name;
  std::string                                spacing_name;
  std::optional<selection<type, dimensions>> selection   ;
};
}

namespace ra
{
template <typename type, std::size_t dimensions>
void load(const mak::hdf5_description<type, dimensions>& description, mak::field<type, dimensions>* field)
{
  if (!field)
  {
    mak::logger->error("Failed to load field: Output is nullptr.");
    return;
  }

  if (!std::ifstream(description.filepath).good())
  {
    mak::logger->error("Failed to load field: File does not exist.");
    return;
  }

  const HighFive::File file(description.filepath);

  field->set_name(description.dataset_name);

  if (description.selection)
  {
    std::vector<std::size_t> offset(dimensions, 0), size(dimensions, 1), stride(dimensions, 1);
    std::copy(description.selection->offset.begin(), description.selection->offset.end(), offset.begin());
    std::copy(description.selection->size  .begin(), description.selection->size  .end(), size  .begin());
    std::copy(description.selection->stride.begin(), description.selection->stride.end(), stride.begin());
    file.getDataSet(description.dataset_name).select(offset, size, stride).read(field->data); 
  }
  else
    file.getDataSet(description.dataset_name).read(field->data);

  if (file.hasAttribute(description.spacing_name))
  {
    std::vector<float> spacing;
    file.getAttribute(description.spacing_name).read(spacing);
    std::copy(spacing.begin(), spacing.end(), field->spacing.begin());
  }
  else
    std::fill(field->spacing.begin(), field->spacing.end(), 1.0f);
}
}

#endif