#include "utils.hpp"

std::string htos (uint16_t n) {
    std::stringstream ss;
    ss << n;
    return ss.str ();
}
