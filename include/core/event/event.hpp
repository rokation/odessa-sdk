#pragma once

#include "core/entity/entity.hpp"

namespace odessa::core {
struct EntitySpawned {
  EntityId entity_id;
};

struct EntityMoved {
  EntityId entity_id;
  EntitySnapshot snapshot;
};

struct EntityDestroyed {
  EntityId entity_id;
};

using Event = std::variant<EntitySpawned, EntityMoved, EntityDestroyed>;
}  // namespace odessa::core
