#pragma once

#include <variant>

#include "core/command/command.hpp"
#include "core/component/position.hpp"
#include "core/component/velocity.hpp"
#include "core/entity/entity.hpp"
#include "nlohmann/json.hpp"

namespace odessa::core::command {
struct SpawnEntity {
  entity::EntityType entity_type;
};

struct DestroyEntity {
  entity::EntityId entity_id;
};

struct SetPosition {
  entity::EntityId entity_id;
  component::Position position;
};

struct SetVelocity {
  entity::EntityId entity_id;
  component::Velocity velocity;
};

using Command =
    std::variant<SpawnEntity, DestroyEntity, SetPosition, SetVelocity>;

void from_json(const nlohmann::json& json, Command& command);
void from_json(const nlohmann::json& json, SetPosition& command);
};  // namespace odessa::core::command
