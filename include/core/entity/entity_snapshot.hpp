#pragma once

#include "core/components/lla.hpp"
#include "core/components/position.hpp"
#include "core/components/velocity.hpp"
#include "core/entity/entity.hpp"

namespace odessa::core {
struct EntitySnapshot {
  EntityId id;
  EntityType type;

  LLA lla;
  Position position;
  Velocity velocity;
};
}  // namespace odessa::core
