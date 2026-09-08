#include "core/world/world.hpp"

#include <iostream>

#include "utils/uuid.hpp"

namespace odessa::core {
World::World() { id_ = util::generate_uuid(); }

Entity* World::get(EntityId entity_id) {
  auto it = entities_.find(entity_id);

  if (it == entities_.end()) {
    return nullptr;
  }
  return &it->second;
}

const Entity* World::get(EntityId entity_id) const {
  auto it = entities_.find(entity_id);

  if (it == entities_.end()) {
    return nullptr;
  }
  return &it->second;
}

Entity& World::spawn(EntityType type) {
  Entity entity(type);
  EntityId entity_id = entity.id();

  auto [it, inserted] = entities_.emplace(entity_id, std::move(entity));

  return it->second;
}

bool World::destroy(EntityId entity_id) {
  auto it = entities_.find(entity_id);

  if (it == entities_.end()) {
    return false;
  }

  entities_.erase(it);
  return true;
}

bool World::move(EntityId entity_id, LLA lla) {
  auto* entity = get(entity_id);

  std::cout << entity->id() << std::endl;

  if (entity == nullptr) {
    return false;
  }

  entity->set_lla(lla);
  return true;
}
}  // namespace odessa::core