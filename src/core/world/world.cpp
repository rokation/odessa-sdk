#include "core/world/world.hpp"

#include "utils/uuid.hpp"

namespace odessa::core {
World::World() { id_ = util::generate_uuid(); }

void World::spawn(EntityType type) {
  entities_.emplace_back(std::make_unique<Entity>(type));
}

std::vector<std::unique_ptr<Entity>>& World::entities() { return entities_; }
}  // namespace odessa::core