#pragma once

#include "core/entity/entity.hpp"

namespace odessa::core {
enum class EventType { ENTITY_SPAWNED, ENTITY_MOVED, ENTITY_DESTROYED };

struct Event {
  EventType type;
  EntityId event_id;
};
}  // namespace odessa::core