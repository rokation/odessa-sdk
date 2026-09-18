#pragma once

#include <variant>

#include "core/component/position.hpp"
#include "core/component/velocity.hpp"
#include "core/entity/entity.hpp"

namespace odessa::core::command {
struct SpawnEntity {
  entity::EntityType type;
};

struct DestroyEntity {
  entity::EntityId entity_id;
};

struct SetPosition {
  entity::EntityId entity_id;
  component::Position position;
};

struct SetVelocity {
  entity::EntityId entity_id;
  component::Velocity velocity;
};

using Command =
    std::variant<SpawnEntity, DestroyEntity, SetPosition, SetVelocity>;
};  // namespace odessa::core::command
