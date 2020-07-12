#include <stdint.h>
#include <DuEngine/visibility.hpp>
namespace Random {
//Gen random uint;
DUENGEXT uint_fast32_t get();
//Gen random float [0.0, 1.0]
DUENGEXT float getf();
}  // namespace Random