#pragma once

#include "core/entity/entity.hpp"

namespace odessa::core {
using WorldId = boost::uuids::uuid;
class World {
 public:
  World();
  Entity* get(EntityId entity_id);
  const Entity* get(EntityId entity_id) const;
  Entity& spawn(EntityType type);
  bool destroy(EntityId entity_id);
  bool move(EntityId entity_id, LLA lla);

 private:
  WorldId id_;
  std::unordered_map<EntityId, Entity> entities_;
};
}  // namespace odessa::core