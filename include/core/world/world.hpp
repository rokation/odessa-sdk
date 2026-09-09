#pragma once

#include "core/entity/entity.hpp"
#include "core/entity/entity_snapshot.hpp"

namespace odessa::core {
using WorldId = boost::uuids::uuid;
class World {
 public:
  World();
  EntityId spawn(EntityType type);
  void for_each(const std::function<void(Entity&)>& fn);

  void update(double dt);
  bool move(EntityId entity_id, LLA lla);
  bool destroy(EntityId entity_id);

  bool set_position(EntityId entity_id, LLA lla);
  bool set_position(EntityId entity_id, Position position);
  bool set_velocity(EntityId entity_id, Velocity velocity);

  std::optional<LLA> get_lla(EntityId entity_id) const;
  std::optional<Position> get_position(EntityId entity_id) const;
  std::optional<Velocity> get_velocity(EntityId entity_id) const;

  std::optional<EntitySnapshot> snapshot(EntityId entity_id) const;
  std::size_t entity_count() const;

 private:
  mutable std::mutex mutex_;
  WorldId id_;
  std::unordered_map<EntityId, Entity> entities_;
};
}  // namespace odessa::core