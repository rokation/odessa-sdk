#pragma once

#include <iostream>

#include "core/entity/entity.hpp"
#include "utils/uuid.hpp"

namespace odessa::core {
enum class EventType { ENTITY_SPAWNED, ENTITY_MOVED, ENTITY_DESTROYED };

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

struct EventHandler {
  void operator()(const EntitySpawned& event) const {
    std::cout << "[" << util::uuid_string_from_raw(event.entity_id) << "] "
              << "Spawned" << std::endl;
  }

  void operator()(const EntityMoved& event) const {
    std::cout << "[" << util::uuid_string_from_raw(event.entity_id) << "] "
              << "Moved (" << event.snapshot.position.x << ", "
              << event.snapshot.position.y << ", " << event.snapshot.position.z
              << ")" << std::endl;
  }

  void operator()(const EntityDestroyed& event) const {
    std::cout << "Destroyed: " << util::uuid_string_from_raw(event.entity_id)
              << std::endl;
  }
};

using Event = std::variant<EntitySpawned, EntityMoved, EntityDestroyed>;
}  // namespace odessa::core
