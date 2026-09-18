#pragma once

#include "core/component/lla.hpp"
#include "core/component/position.hpp"

namespace odessa::core::coordinate {
component::Position lla_to_enu(const component::LLA& lla,
                               const component::LLA& origin);
component::LLA enu_to_lla(const component::Position& position,
                          const component::LLA& origin);
}  // namespace odessa::core::coordinate
