#pragma once

#include <optional>
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
  std::optional<EntitySnapshot> find(EntityId entity_id);

  EntityId spawn(EntityType entity_type);

  void update(double dt);
  bool move(EntityId entity_id, Position position);
  bool destroy(EntityId entity_id);

  void set_origin(LLA lla);

  bool set_lla(EntityId entity_id, LLA lla);
  bool set_position(EntityId entity_id, Position position);
  bool set_velocity(EntityId entity_id, Velocity velocity);

  LLA origin() const;
  std::optional<LLA> get_lla(EntityId entity_id) const;
  std::optional<Position> get_position(EntityId entity_id) const;
  std::optional<Velocity> get_velocity(EntityId entity_id) const;

  std::optional<EntitySnapshot> snapshot(EntityId entity_id) const;
  std::vector<EntitySnapshot> snapshot_all() const;
  std::size_t entity_count() const;

  bool contains(EntityId entity_id) const;
  std::vector<Event> consume_events();

  EntityId attach(EntityType entity_type, std::string external_id);
  bool detach(const std::string& external_id);
  std::optional<EntityId> find_by_external_id(
      const std::string& external_id) const;

  std::vector<EntityId> query_radius(Position& center, double radius) const;
  std::vector<EntityId> query_radius(Position& center, double radius,
                                     EntityType entity_type) const;

  std::vector<EntityId> query_bbox(Position& min, Position& max) const;
  std::vector<EntityId> query_bbox(Position& min, Position& max,
                                   EntityType entity_type) const;

  std::vector<EntityId> query_type(EntityType entity_type) const;

 private:
  EntityId spawn_unlocked(EntityType entity_type);
  std::vector<EntityId> query(
      const std::function<bool(const Entity&)>& predicate) const;
  void detach_attachment(EntityId entity_id);

  mutable std::mutex mutex_;
  WorldId id_;
  LLA origin_;
  std::unordered_map<EntityId, Entity> entities_;
  std::unordered_map<std::string, Attachment> attachments_;
  std::vector<Event> events_;
};
}  // namespace odessa::core
