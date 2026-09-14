#include "core/world/world.hpp"

#include <mutex>
#include <optional>

#include "core/attach/attach.hpp"
#include "core/components/lla.hpp"
#include "core/entity/entity.hpp"
#include "core/event/event.hpp"
#include "utils/uuid.hpp"

namespace odessa::core {
World::World() { id_ = util::generate_uuid(); }

EntityId World::spawn(EntityType entity_type) {
  std::lock_guard lock(mutex_);

  Entity entity(entity_type);
  EntityId entity_id = entity.id();

  entities_.emplace(entity_id, std::move(entity));
  events_.push_back(EntitySpawned{entity_id});

  return entity_id;
}

void World::update(double dt) {
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

  for (auto attachment_it = attachments_.begin();
       attachment_it != attachments_.end();) {
    if (attachment_it->second.entity_id == entity_id) {
      attachment_it = attachments_.erase(attachment_it);
    } else {
      attachment_it++;
    }
  }

  entities_.erase(it);
  events_.push_back(EntityDestroyed{entity_id});

  return true;
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

  const auto& entity = it->second;

  return EntitySnapshot{
      entity.id(),       entity.type(),     entity.lla(),
      entity.position(), entity.velocity(),
  };
}

std::vector<EntitySnapshot> World::snapshot_all() const {
  std::lock_guard lock(mutex_);

  std::vector<EntitySnapshot> snapshots;
  snapshots.reserve(entities_.size());

  for (const auto& [entity_id, entity] : entities_) {
    snapshots.push_back({entity.id(), entity.type(), entity.lla(),
                         entity.position(), entity.velocity()});
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
  auto it = attachments_.find(external_id);
  if (it != attachments_.end()) {
    return it->second.entity_id;
  }

  auto entity_id = spawn(entity_type);
  attachments_.emplace(external_id, Attachment{entity_id, external_id});
  return entity_id;
}

std::optional<EntityId> World::find_by_external_id(
    std::string& external_id) const {
  std::lock_guard lock(mutex_);
  auto it = attachments_.find(external_id);

  if (it == attachments_.end()) {
    return std::nullopt;
  }

  return it->second.entity_id;
}
}  // namespace odessa::core
