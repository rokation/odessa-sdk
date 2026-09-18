#include "core/world/world.hpp"

#include <cmath>
#include <mutex>
#include <optional>
#include <vector>

#include "core/attach/attach.hpp"
#include "core/component/lla.hpp"
#include "core/entity/entity.hpp"
#include "core/event/event.hpp"
#include "utils/uuid.hpp"

namespace odessa::core::world {
World::World() : origin_{} { id_ = util::generate_uuid(); }

WorldId World::id() const { return id_; }

entity::EntityId World::spawn(entity::EntityType entity_type) {
  std::lock_guard lock(mutex_);
  return spawn_unlocked(entity_type);
}

void World::update(double dt) {
  if (!std::isfinite(dt) || dt < 0.0) {
    return;
  }

  std::lock_guard lock(mutex_);
  for (auto& [entity_id, entity] : entities_) {
    auto old_position = entity.position();

    entity.update(dt);

    auto new_position = entity.position();

    if (old_position.x != new_position.x || old_position.y != new_position.y ||
        old_position.z != new_position.z) {
      events_.push_back(event::EntityMoved{entity_id, entity.snapshot()});
    }
  }
}

bool World::destroy(entity::EntityId entity_id) {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);

  if (it == entities_.end()) {
    return false;
  }

  detach_attachment(entity_id);

  entities_.erase(it);
  events_.push_back(event::EntityDestroyed{entity_id});

  return true;
}

component::LLA World::origin() const {
  std::lock_guard lock(mutex_);

  return origin_;
}

void World::set_origin(component::LLA origin) {
  std::lock_guard lock(mutex_);

  origin_ = origin;
}

bool World::set_lla(entity::EntityId entity_id, component::LLA lla) {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);
  if (it == entities_.end()) {
    return false;
  }

  auto& entity = it->second;
  entity.set_lla(lla);
  events_.push_back(event::EntityMoved{entity_id, entity.snapshot()});
  return true;
}

bool World::set_position(entity::EntityId entity_id,
                         component::Position position) {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);
  if (it == entities_.end()) {
    return false;
  }

  auto& entity = it->second;
  entity.set_position(position);
  events_.push_back(event::EntityMoved{entity_id, entity.snapshot()});

  return true;
}

bool World::set_velocity(entity::EntityId entity_id,
                         component::Velocity velocity) {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);
  if (it == entities_.end()) {
    return false;
  }

  it->second.set_velocity(velocity);

  return true;
}

std::optional<component::LLA> World::get_lla(entity::EntityId entity_id) const {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);
  if (it == entities_.end()) {
    return std::nullopt;
  }

  return it->second.lla();
}

std::optional<component::Position> World::get_position(
    entity::EntityId entity_id) const {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);
  if (it == entities_.end()) {
    return std::nullopt;
  }

  return it->second.position();
}

std::optional<component::Velocity> World::get_velocity(
    entity::EntityId entity_id) const {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);
  if (it == entities_.end()) {
    return std::nullopt;
  }

  return it->second.velocity();
}

std::optional<entity::EntitySnapshot> World::snapshot(
    entity::EntityId entity_id) const {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);

  if (it == entities_.end()) {
    return std::nullopt;
  }

  return it->second.snapshot();
}

std::vector<entity::EntitySnapshot> World::snapshot_all() const {
  std::lock_guard lock(mutex_);

  std::vector<entity::EntitySnapshot> snapshots;
  snapshots.reserve(entities_.size());

  for (const auto& [entity_id, entity] : entities_) {
    snapshots.push_back(entity.snapshot());
  }

  return snapshots;
}

std::vector<event::Event> World::consume_events() {
  std::lock_guard lock(mutex_);

  std::vector<event::Event> events;
  events.swap(events_);

  return events;
}

bool World::contains(entity::EntityId entity_id) const {
  std::lock_guard lock(mutex_);

  return entities_.contains(entity_id);
}

std::size_t World::entity_count() const {
  std::lock_guard lock(mutex_);

  return entities_.size();
}

entity::EntityId World::attach(entity::EntityType entity_type,
                               std::string external_id) {
  std::lock_guard lock(mutex_);

  auto it = attachments_.find(external_id);
  if (it != attachments_.end()) {
    return it->second.entity_id;
  }

  auto entity_id = spawn_unlocked(entity_type);
  attachments_.emplace(external_id, attach::Attachment{entity_id, external_id});
  return entity_id;
}

void World::detach_attachment(entity::EntityId entity_id) {
  for (auto it = attachments_.begin(); it != attachments_.end();) {
    if (it->second.entity_id == entity_id) {
      it = attachments_.erase(it);
    } else {
      ++it;
    }
  }
}

bool World::detach(const std::string& external_id) {
  std::lock_guard lock(mutex_);
  return attachments_.erase(external_id) > 0;
}

std::optional<entity::EntityId> World::find_by_external_id(
    const std::string& external_id) const {
  std::lock_guard lock(mutex_);
  auto it = attachments_.find(external_id);

  if (it == attachments_.end()) {
    return std::nullopt;
  }

  return it->second.entity_id;
}

// std::vector<entity::EntityId> World::query(
//     const std::function<bool(const entity::Entity&)>& predicate) const {
//   std::vector<entity::EntityId> result;

//   for (const auto [entity_id, entity] : entities_) {
//     if (predicate(entity)) {
//       result.push_back(entity_id);
//     }
//   }

//   return result;
// }

// std::vector<entity::EntityId> World::query_radius(component::Position&
// center,
//                                                   double radius) const {
//   if (radius < 0.0) {
//     return {};
//   }

//   double radius_squared = radius * radius;

//   std::lock_guard lock(mutex_);
//   return query([&](const entity::Entity& entity) {
//     const auto& position = entity.position();

//     const double dx = position.x - center.x;
//     const double dy = position.y - center.y;
//     const double dz = position.z - center.z;

//     const double distance_squared = dx * dx + dy * dy + dz * dz;

//     return distance_squared <= radius_squared;
//   });
// }

// std::vector<entity::EntityId> World::query_radius(
//     component::Position& center, double radius,
//     entity::EntityType entity_type) const {
//   if (radius < 0.0) {
//     return {};
//   }

//   double radius_squared = radius * radius;

//   std::lock_guard lock(mutex_);
//   return query([&](const entity::Entity& entity) {
//     if (entity.type() != entity_type) {
//       return false;
//     }

//     const auto& position = entity.position();

//     const double dx = position.x - center.x;
//     const double dy = position.y - center.y;
//     const double dz = position.z - center.z;

//     const double distance_squared = dx * dx + dy * dy + dz * dz;

//     return distance_squared <= radius_squared;
//   });
// }

// std::vector<entity::EntityId> World::query_bbox(
//     component::Position& min, component::Position& max) const {
//   if (min.x > max.x || min.y > max.y || min.z > max.z) {
//     return {};
//   }

//   std::lock_guard lock(mutex_);
//   return query([&](const entity::Entity& entity) {
//     const auto& position = entity.position();

//     return position.x >= min.x && position.x <= max.x && position.y >= min.y
//     &&
//            position.y <= max.y && position.z >= min.z && position.z <= max.z;
//   });
// }

// std::vector<entity::EntityId> World::query_bbox(
//     component::Position& min, component::Position& max,
//     entity::EntityType entity_type) const {
//   if (min.x > max.x || min.y > max.y || min.z > max.z) {
//     return {};
//   }

//   std::lock_guard lock(mutex_);
//   return query([&](const entity::Entity& entity) {
//     if (entity.type() != entity_type) {
//       return false;
//     }
//     const auto& position = entity.position();

//     return position.x >= min.x && position.x <= max.x && position.y >= min.y
//     &&
//            position.y <= max.y && position.z >= min.z && position.z <= max.z;
//   });
// }

// std::vector<entity::EntityId> World::query_type(
//     entity::EntityType entity_type) const {
//   std::lock_guard lock(mutex_);

//   return query([&](const entity::Entity& entity) {
//     return entity.type() == entity_type;
//   });
// }

entity::EntityId World::spawn_unlocked(entity::EntityType entity_type) {
  entity::Entity entity(entity_type);
  entity::EntityId entity_id = entity.id();

  entities_.emplace(entity_id, std::move(entity));
  events_.push_back(event::EntitySpawned{entity_id});

  return entity_id;
}
}  // namespace odessa::core::world
