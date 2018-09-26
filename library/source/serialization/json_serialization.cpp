#include <makina/serialization/json_serialization.hpp>

#include <cstdint>
#include <functional>
#include <optional>
#include <string>

namespace mak
{
nlohmann::json to_json  (                            const rttr::instance& instance)
{
        std::function<nlohmann::json               (const rttr::instance&                  )> to_json_recursive             ;
        std::function<nlohmann::json               (                   const rttr::variant&)> variant_to_json               ;
  const std::function<std::optional<nlohmann::json>(const rttr::type&, const rttr::variant&)> atomic_to_json                = [&] (const rttr::type& type, const rttr::variant& variant)
  {
    std::optional<nlohmann::json> json;
    if      (type.is_arithmetic ())
    {
      if      (type == rttr::type::get<bool>         ())
        json = variant.to_bool  ();
      else if (type == rttr::type::get<char>         ())
        json = variant.to_uint8 ();
      else if (type == rttr::type::get<std::int8_t>  ())
        json = variant.to_int8  ();
      else if (type == rttr::type::get<std::int16_t> ())
        json = variant.to_int16 ();
      else if (type == rttr::type::get<std::int32_t> ())
        json = variant.to_int32 ();
      else if (type == rttr::type::get<std::int64_t> ())
        json = variant.to_int64 ();
      else if (type == rttr::type::get<std::uint8_t> ())
        json = variant.to_uint8 ();
      else if (type == rttr::type::get<std::uint16_t>())
        json = variant.to_uint16();
      else if (type == rttr::type::get<std::uint32_t>())
        json = variant.to_uint32();
      else if (type == rttr::type::get<std::uint64_t>())
        json = variant.to_uint64();
      else if (type == rttr::type::get<float>        ())
        json = variant.to_double();
      else if (type == rttr::type::get<double>       ())
        json = variant.to_double();
    }
    else if (type.is_enumeration())
    {
      auto valid  = false;
      auto string = variant.to_string(&valid);
      if (valid) json = string;
      else
      {
        auto integer = variant.to_uint64(&valid);
        if (valid) json = integer;
      }
    }
    else if (type == rttr::type::get<std::string>())
      json = variant.to_string();
    return json;
  };
  const std::function<nlohmann::json               (const rttr::variant_sequential_view&   )> sequential_container_to_json  = [&] (const rttr::variant_sequential_view&  view)
  {
    auto json = nlohmann::json::array();
    for (const auto& item : view)
    {
      if (item.is_sequential_container())
        json.push_back(sequential_container_to_json(item.create_sequential_view()));
      else
      {
        auto value = item .extract_wrapped_value();
        auto type  = value.get_type             ();

        if (type.is_arithmetic() || type == rttr::type::get<std::string>() || type.is_enumeration())
        {
          auto result = atomic_to_json(type, value);
          if  (result) json.push_back(result.value());
        }
        else json.push_back(to_json_recursive(value));
      }
    }
    return json;
  };
  const std::function<nlohmann::json               (const rttr::variant_associative_view&  )> associative_container_to_json = [&] (const rttr::variant_associative_view& view)
  {
    auto json = nlohmann::json::array();
    if (view.is_key_only_type()) 
      for (auto& item : view) 
        json.push_back(variant_to_json(item.first));
    else
    {
      for (auto& item : view)
      {
        auto object = nlohmann::json::object();
        object["key"  ] = variant_to_json(item.first );
        object["value"] = variant_to_json(item.second);
        json.push_back(object);
      }
    }
    return json;
  };
  to_json_recursive = [&] (const rttr::instance& current_instance)
  {
    nlohmann::json json;
    auto raw_instance = current_instance.get_type().get_raw_type().is_wrapper() ? current_instance.get_wrapped_instance() : current_instance;
    for (auto property : raw_instance.get_derived_type().get_properties())
    {
      const auto name   = property.get_name ();
      const auto value  = property.get_value(raw_instance);
      if (property.get_metadata("no_serialize") || !value) continue;
      json[name.data()] = variant_to_json(value);
    }
    return json;
  };
  variant_to_json   = [&] (const rttr::variant&  variant         )
  {
    nlohmann::json json;
    const auto type   = variant.get_type().is_wrapper() ? variant.get_type().get_wrapped_type() : variant.get_type();
    const auto result = atomic_to_json(type, variant.get_type().is_wrapper() ? variant.extract_wrapped_value() : variant);
    if  (result)
      json = result.value();
    else if (variant.is_sequential_container ())
      json = sequential_container_to_json (variant.create_sequential_view ());
    else if (variant.is_associative_container())
      json = associative_container_to_json(variant.create_associative_view());
    else
      json = !type.get_properties().empty() ? to_json_recursive(variant) : variant.to_string();
    return json;
  };
  return to_json_recursive(instance);
}
void           from_json(const nlohmann::json& json, const rttr::instance& instance)
{
        std::function<void         (const nlohmann::json&, rttr::instance                 )> from_json_recursive             ;
  const std::function<rttr::variant(const nlohmann::json&                                 )> atomic_from_json                = [&] (const nlohmann::json& json)
  {
    if      (json.is_string         ())
      return rttr::variant(json.get<std::string>  ());
    else if (json.is_boolean        ())
      return rttr::variant(json.get<bool>         ());
    else if (json.is_number_integer ())
      return rttr::variant(json.get<std::int32_t> ());
    else if (json.is_number_unsigned())
      return rttr::variant(json.get<std::uint32_t>());
    else if (json.is_number_float   ())
      return rttr::variant(json.get<float>        ());
    return rttr::variant();
  }; 
  const std::function<rttr::variant(const nlohmann::json&, const rttr::type&              )> atomic_from_json_as_type        = [&] (const nlohmann::json& json, const rttr::type& type)
  {
    auto variant     = atomic_from_json(json);
    auto convertable = variant.convert(type);
    if (!convertable && json.is_object())
    {
      auto constructor = type.get_constructor ();
      for (auto& item  : type.get_constructors())
        if (item.get_instantiated_type() == type)
          constructor = item;
      variant = constructor.invoke();
      from_json_recursive(json, variant);
    }
    return variant;
  };
  const std::function<void         (const nlohmann::json&, rttr::variant_sequential_view& )> sequential_container_from_json  = [&] (const nlohmann::json& json, rttr::variant_sequential_view&  view)
  {
    view.set_size(json.size());
    for (auto i = 0; i < json.size(); ++i)
    {
      auto& iteratee = json[i];
      if      (iteratee.is_array ())
      {
        auto subview = view.get_value(i).create_sequential_view();
        sequential_container_from_json(iteratee, subview);
      }
      else if (iteratee.is_object())
      {
        auto variant = view.get_value(i).extract_wrapped_value();
        from_json_recursive(iteratee, variant);
        view.set_value(i, variant);
      }
      else
      {
        auto variant = atomic_from_json(iteratee);
        if (variant.convert(view.get_rank_type(i)))
          view.set_value(i, variant);
      }
    }
  }; 
  const std::function<void         (const nlohmann::json&, rttr::variant_associative_view&)> associative_container_from_json = [&] (const nlohmann::json& json, rttr::variant_associative_view& view)
  {
    for (auto i = 0; i < json.size(); ++i)
    {
      auto& iteratee = json[i];
      if (iteratee.is_object())
      {
        if (!iteratee.count("key") || !iteratee.count("value"))
          continue;
        auto key_variant   = atomic_from_json_as_type(iteratee["key"  ], view.get_key_type  ());
        auto value_variant = atomic_from_json_as_type(iteratee["value"], view.get_value_type());
        if  (key_variant && value_variant)
          view.insert(key_variant, value_variant);
      }
      else
      {
        auto variant = atomic_from_json(json);
        if  (variant && variant.convert(view.get_key_type()))
          view.insert(variant);
      }
    }
  };
  from_json_recursive = [&] (const nlohmann::json& json, rttr::instance current_instance)
  {
    auto raw_instance = current_instance.get_type().get_raw_type().is_wrapper() ? current_instance.get_wrapped_instance() : current_instance;
    for (auto& property : raw_instance.get_derived_type().get_properties())
    {
      if (!json.count(property.get_name().data())) continue;
      
      const auto type  = property.get_type();
      auto       value = json.at(property.get_name().data());

      if      (value.is_array())
      {
        if      (type.is_sequential_container ())
        {
          auto variant = property.get_value             (raw_instance);
          auto view    = variant .create_sequential_view();
          sequential_container_from_json(value, view);
          property.set_value(raw_instance, variant);
        }
        else if (type.is_associative_container())
        {
          auto variant = property.get_value              (raw_instance);
          auto view    = variant .create_associative_view();
          associative_container_from_json(value, view);
          property.set_value(raw_instance, variant);
        }
      }
      else if (value.is_object())
      {
        auto variant = property.get_value(raw_instance);
        from_json_recursive(value, variant);
        property.set_value(raw_instance, variant);
      }
      else
      {
        auto variant = atomic_from_json(value);
        if (variant.convert(type))
          property.set_value(raw_instance, variant);
      }
    }
  };
  from_json_recursive(json, instance);
}
}