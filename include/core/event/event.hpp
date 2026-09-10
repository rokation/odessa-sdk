#pragma once

#include <iostream>

#include "core/entity/entity.hpp"
#include "utils/uuid.hpp"

namespace odessa::core {
enum class EventType { ENTITY_SPAWNED, ENTITY_MOVED, ENTITY_DESTROYED };

struct EntitySpawned {
  EntityId event_id;
};

struct EntityMoved {
  EntityId event_id;
  Position position;
};

struct EntityDestroyed {
  EntityId event_id;
};

struct EventHandler {
  void operator()(const EntitySpawned& event) const {
    std::cout << "[" << util::uuid_string_from_raw(event.event_id) << "] "
              << "Spawned" << std::endl;
  }

  void operator()(const EntityMoved& event) const {
    std::cout << "[" << util::uuid_string_from_raw(event.event_id) << "] "
              << "Moved (" << event.position.x << ", " << event.position.y
              << ", " << event.position.z << ")" << std::endl;
  }

  void operator()(const EntityDestroyed& event) const {
    std::cout << "Destroyed: " << util::uuid_string_from_raw(event.event_id)
              << std::endl;
  }
};

using Event = std::variant<EntitySpawned, EntityMoved, EntityDestroyed>;
}  // namespace odessa::core
