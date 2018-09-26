#ifndef MAKINA_SERIALIZATION_JSON_SERIALIZATION_HPP_
#define MAKINA_SERIALIZATION_JSON_SERIALIZATION_HPP_

#include <nlohmann/json.hpp>
#include <rttr/type.h>

#include <makina/export.hpp>

namespace mak
{
MAKINA_EXPORT nlohmann::json to_json  (                            const rttr::instance& instance); 
MAKINA_EXPORT void           from_json(const nlohmann::json& json, const rttr::instance& instance);
}

#endif