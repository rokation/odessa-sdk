#pragma once

#include "core/entity/entity.hpp"

namespace odessa::core::event {
struct EntitySpawned {
  entity::EntityId entity_id;
};

struct EntityMoved {
  entity::EntityId entity_id;
  entity::EntitySnapshot snapshot;
};

struct EntityDestroyed {
  entity::EntityId entity_id;
};

using Event = std::variant<EntitySpawned, EntityMoved, EntityDestroyed>;
}  // namespace odessa::core::event
