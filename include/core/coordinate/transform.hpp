#pragma once

#include "core/components/lla.hpp"
#include "core/components/position.hpp"

namespace odessa::core {
Position lla_to_enu(const LLA& lla, const LLA& origin);
LLA enu_to_lla(const Position& position, const LLA& origin);
}  // namespace odessa::core
