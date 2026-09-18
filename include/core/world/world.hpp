#pragma once

#include <optional>
#include <vector>

#include "core/attach/attach.hpp"
#include "core/component/lla.hpp"
#include "core/component/position.hpp"
#include "core/component/velocity.hpp"
#include "core/entity/entity.hpp"
#include "core/event/event.hpp"

namespace odessa::core::world {
using WorldId = boost::uuids::uuid;
class World {
 public:
  World();
  WorldId id() const;
  std::optional<entity::EntitySnapshot> find(entity::EntityId entity_id);

  entity::EntityId spawn(entity::EntityType entity_type);

  void update(double dt);
  bool move(entity::EntityId entity_id, component::Position position);
  bool destroy(entity::EntityId entity_id);

  void set_origin(component::LLA lla);

  bool set_lla(entity::EntityId entity_id, component::LLA lla);
  bool set_position(entity::EntityId entity_id, component::Position position);
  bool set_velocity(entity::EntityId entity_id, component::Velocity velocity);

  component::LLA origin() const;
  std::optional<component::LLA> get_lla(entity::EntityId entity_id) const;
  std::optional<component::Position> get_position(
      entity::EntityId entity_id) const;
  std::optional<component::Velocity> get_velocity(
      entity::EntityId entity_id) const;

  std::optional<entity::EntitySnapshot> snapshot(
      entity::EntityId entity_id) const;
  std::vector<entity::EntitySnapshot> snapshot_all() const;
  std::size_t entity_count() const;

  bool contains(entity::EntityId entity_id) const;
  std::vector<event::Event> consume_events();

  entity::EntityId attach(entity::EntityType entity_type,
                          std::string external_id);
  bool detach(const std::string& external_id);
  std::optional<entity::EntityId> find_by_external_id(
      const std::string& external_id) const;

  std::vector<entity::EntityId> query_radius(component::Position& center,
                                             double radius) const;
  std::vector<entity::EntityId> query_radius(
      component::Position& center, double radius,
      entity::EntityType entity_type) const;

  std::vector<entity::EntityId> query_bbox(component::Position& min,
                                           component::Position& max) const;
  std::vector<entity::EntityId> query_bbox(
      component::Position& min, component::Position& max,
      entity::EntityType entity_type) const;

  std::vector<entity::EntityId> query_type(
      entity::EntityType entity_type) const;

 private:
  entity::EntityId spawn_unlocked(entity::EntityType entity_type);
  std::vector<entity::EntityId> query(
      const std::function<bool(const entity::Entity&)>& predicate) const;
  void detach_attachment(entity::EntityId entity_id);

  mutable std::mutex mutex_;
  WorldId id_;
  component::LLA origin_;
  std::unordered_map<entity::EntityId, entity::Entity> entities_;
  std::unordered_map<std::string, attach::Attachment> attachments_;
  std::vector<event::Event> events_;
};
}  // namespace odessa::core::world
