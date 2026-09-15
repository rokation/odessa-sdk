#pragma once

#include <vector>

#include "core/attach/attach.hpp"
#include "core/entity/entity.hpp"
#include "core/event/event.hpp"

namespace odessa::core {
using WorldId = boost::uuids::uuid;
class World {
 public:
  World();
  WorldId id() const;

  EntityId spawn(EntityType type);

  void update(double dt);
  bool move(EntityId entity_id, Position position);
  bool destroy(EntityId entity_id);

  bool set_lla(EntityId entity_id, LLA lla);
  bool set_position(EntityId entity_id, Position position);
  bool set_velocity(EntityId entity_id, Velocity velocity);

  std::optional<LLA> get_lla(EntityId entity_id) const;
  std::optional<Position> get_position(EntityId entity_id) const;
  std::optional<Velocity> get_velocity(EntityId entity_id) const;

  std::optional<EntitySnapshot> snapshot(EntityId entity_id) const;
  std::vector<EntitySnapshot> snapshot_all() const;
  std::size_t entity_count() const;

  bool contains(EntityId entity_id) const;
  std::vector<Event> consume_events();

  EntityId attach(EntityType entity_type, std::string external_id);
  std::optional<EntityId> find_by_external_id(std::string& external_id) const;

  std::vector<EntityId> query_radius(Position& center, double radius) const;
  std::vector<EntityId> query_radius(Position& center, double radius,
                                     EntityType type) const;

 private:
  mutable std::mutex mutex_;
  WorldId id_;
  std::unordered_map<EntityId, Entity> entities_;
  std::unordered_map<std::string, Attachment> attachments_;
  std::vector<Event> events_;
};
}  // namespace odessa::core
