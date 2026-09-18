#include "core/world/world.hpp"

#include <cmath>
#include <mutex>
#include <optional>
#include <vector>

#include "core/attach/attach.hpp"
#include "core/components/lla.hpp"
#include "core/entity/entity.hpp"
#include "core/event/event.hpp"
#include "utils/uuid.hpp"

namespace odessa::core {
World::World() : origin_{} { id_ = util::generate_uuid(); }

WorldId World::id() const { return id_; }

EntityId World::spawn(EntityType entity_type) {
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
      events_.push_back(EntityMoved{entity_id, entity.snapshot()});
    }
  }
}

bool World::destroy(EntityId entity_id) {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);

  if (it == entities_.end()) {
    return false;
  }

  detach_attachment(entity_id);

  entities_.erase(it);
  events_.push_back(EntityDestroyed{entity_id});

  return true;
}

LLA World::origin() const { return origin_; }

void World::set_origin(LLA origin) {
  std::lock_guard lock(mutex_);

  origin_ = origin;
}

bool World::set_lla(EntityId entity_id, LLA lla) {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);
  if (it == entities_.end()) {
    return false;
  }

  auto& entity = it->second;
  entity.set_lla(lla);
  events_.push_back(EntityMoved{entity_id, entity.snapshot()});
  return true;
}

bool World::set_position(EntityId entity_id, Position position) {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);
  if (it == entities_.end()) {
    return false;
  }

  auto& entity = it->second;
  entity.set_position(position);
  events_.push_back(EntityMoved{entity_id, entity.snapshot()});

  return true;
}

bool World::set_velocity(EntityId entity_id, Velocity velocity) {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);
  if (it == entities_.end()) {
    return false;
  }

  it->second.set_velocity(velocity);

  return true;
}

std::optional<LLA> World::get_lla(EntityId entity_id) const {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);
  if (it == entities_.end()) {
    return std::nullopt;
  }

  return it->second.lla();
}

std::optional<Position> World::get_position(EntityId entity_id) const {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);
  if (it == entities_.end()) {
    return std::nullopt;
  }

  return it->second.position();
}

std::optional<Velocity> World::get_velocity(EntityId entity_id) const {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);
  if (it == entities_.end()) {
    return std::nullopt;
  }

  return it->second.velocity();
}

std::optional<EntitySnapshot> World::snapshot(EntityId entity_id) const {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);

  if (it == entities_.end()) {
    return std::nullopt;
  }

  return it->second.snapshot();
}

std::vector<EntitySnapshot> World::snapshot_all() const {
  std::lock_guard lock(mutex_);

  std::vector<EntitySnapshot> snapshots;
  snapshots.reserve(entities_.size());

  for (const auto& [entity_id, entity] : entities_) {
    snapshots.push_back(entity.snapshot());
  }

  return snapshots;
}

std::vector<Event> World::consume_events() {
  std::lock_guard lock(mutex_);

  std::vector<Event> events;
  events.swap(events_);

  return events;
}

bool World::contains(EntityId entity_id) const {
  std::lock_guard lock(mutex_);

  return entities_.contains(entity_id);
}

std::size_t World::entity_count() const {
  std::lock_guard lock(mutex_);

  return entities_.size();
}

EntityId World::attach(EntityType entity_type, std::string external_id) {
  std::lock_guard lock(mutex_);

  auto it = attachments_.find(external_id);
  if (it != attachments_.end()) {
    return it->second.entity_id;
  }

  auto entity_id = spawn_unlocked(entity_type);
  attachments_.emplace(external_id, Attachment{entity_id, external_id});
  return entity_id;
}

void World::detach_attachment(EntityId entity_id) {
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

std::optional<EntityId> World::find_by_external_id(
    const std::string& external_id) const {
  std::lock_guard lock(mutex_);
  auto it = attachments_.find(external_id);

  if (it == attachments_.end()) {
    return std::nullopt;
  }

  return it->second.entity_id;
}

std::vector<EntityId> World::query(
    const std::function<bool(const Entity&)>& predicate) const {
  std::vector<EntityId> result;

  for (const auto [entity_id, entity] : entities_) {
    if (predicate(entity)) {
      result.push_back(entity_id);
    }
  }

  return result;
}

std::vector<EntityId> World::query_radius(Position& center,
                                          double radius) const {
  if (radius < 0.0) {
    return {};
  }

  double radius_squared = radius * radius;

  std::lock_guard lock(mutex_);
  return query([&](const Entity& entity) {
    const auto& position = entity.position();

    const double dx = position.x - center.x;
    const double dy = position.y - center.y;
    const double dz = position.z - center.z;

    const double distance_squared = dx * dx + dy * dy + dz * dz;

    return distance_squared <= radius_squared;
  });
}

std::vector<EntityId> World::query_radius(Position& center, double radius,
                                          EntityType entity_type) const {
  if (radius < 0.0) {
    return {};
  }

  double radius_squared = radius * radius;

  std::lock_guard lock(mutex_);
  return query([&](const Entity& entity) {
    if (entity.type() != entity_type) {
      return false;
    }

    const auto& position = entity.position();

    const double dx = position.x - center.x;
    const double dy = position.y - center.y;
    const double dz = position.z - center.z;

    const double distance_squared = dx * dx + dy * dy + dz * dz;

    return distance_squared <= radius_squared;
  });
}

std::vector<EntityId> World::query_bbox(Position& min, Position& max) const {
  if (min.x > max.x || min.y > max.y || min.z > max.z) {
    return {};
  }

  std::lock_guard lock(mutex_);
  return query([&](const Entity& entity) {
    const auto& position = entity.position();

    return position.x >= min.x && position.x <= max.x && position.y >= min.y &&
           position.y <= max.y && position.z >= min.z && position.z <= max.z;
  });
}

std::vector<EntityId> World::query_bbox(Position& min, Position& max,
                                        EntityType entity_type) const {
  if (min.x > max.x || min.y > max.y || min.z > max.z) {
    return {};
  }

  std::lock_guard lock(mutex_);
  return query([&](const Entity& entity) {
    if (entity.type() != entity_type) {
      return false;
    }
    const auto& position = entity.position();

    return position.x >= min.x && position.x <= max.x && position.y >= min.y &&
           position.y <= max.y && position.z >= min.z && position.z <= max.z;
  });
}

std::vector<EntityId> World::query_type(EntityType entity_type) const {
  std::lock_guard lock(mutex_);

  return query(
      [&](const Entity& entity) { return entity.type() == entity_type; });
}

std::optional<EntitySnapshot> World::find(EntityId entity_id) {
  std::lock_guard lock(mutex_);

  auto it = entities_.find(entity_id);

  if (it == entities_.end()) {
    return std::nullopt;
  }

  return it->second.snapshot();
}

EntityId World::spawn_unlocked(EntityType entity_type) {
  Entity entity(entity_type);
  EntityId entity_id = entity.id();

  entities_.emplace(entity_id, std::move(entity));
  events_.push_back(EntitySpawned{entity_id});

  return entity_id;
}
}  // namespace odessa::core
