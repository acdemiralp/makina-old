#ifndef MAKINA_UTILITY_STRING_HPP_
#define MAKINA_UTILITY_STRING_HPP_

#include <string>

#include <makina/export.hpp>

namespace mak
{
MAKINA_EXPORT std::string compress     (const std::string& string);
MAKINA_EXPORT std::string decompress   (const std::string& string);

MAKINA_EXPORT std::string base64_encode(const std::string& string);
MAKINA_EXPORT std::string base64_decode(const std::string& string);
}

#endif