#include "astra/util/time.hpp"

std::string astra::timestamp() {
    return timestamp("{:%Y-%m-%d_%H-%M-%S}");
}
