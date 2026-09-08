#pragma once

#include "core/entity/entity.hpp"

namespace odessa::core {
using WorldId = boost::uuids::uuid;
class World {
 public:
  World();
  void spawn(EntityType type);
  std::vector<std::unique_ptr<Entity>>& entities();
  const std::vector<std::unique_ptr<Entity>>& entities() const;
 private:
  WorldId id_;
  std::vector<std::unique_ptr<Entity>> entities_;
};
}  // namespace odessa::core