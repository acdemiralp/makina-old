#include <makina/renderer/coordinate_system.hpp>

namespace mak
{
coordinate_system lh_coordinate_system = {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f,  1.0f}};
coordinate_system rh_coordinate_system = {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}};
}