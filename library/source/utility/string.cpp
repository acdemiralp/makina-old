#include <makina/utility/string.hpp>

#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/zlib.hpp>

namespace mak
{
std::string compress     (const std::string& string)
{
  std::stringstream input, output;
  boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
  input << string;
  buffer.push(boost::iostreams::zlib_compressor());
  buffer.push(input);
  boost::iostreams::copy(buffer, output);
  return output.str();
}
std::string decompress   (const std::string& string)
{
  std::stringstream input, output;
  boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
  input << string;
  buffer.push(boost::iostreams::zlib_decompressor());
  buffer.push(input);
  boost::iostreams::copy(buffer, output);
  return output.str();
}

std::string base64_encode(const std::string& string)
{
  using namespace boost::archive::iterators;
  using iterator = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
  auto temporary = std::string(iterator(std::begin(string)), iterator(std::end(string)));
  return temporary.append((3 - string.size() % 3) % 3, '=');
}
std::string base64_decode(const std::string& string)
{
  using namespace boost::archive::iterators;
  using iterator = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
  return boost::algorithm::trim_right_copy_if(std::string(iterator(std::begin(string)), iterator(std::end(string))), [](const char c) { return c == '\0'; });
}
}
