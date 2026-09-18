#pragma once

#include <string>

#include "core/component/lla.hpp"
#include "core/component/position.hpp"
#include "core/component/velocity.hpp"
#include "core/entity/entity.hpp"
#include "core/event/event.hpp"
#include "nlohmann/json.hpp"

namespace odessa::core::serialization {
std::string entity_type_to_string(entity::EntityType type);
entity::EntityType entity_type_from_string(const std::string& type);
}  // namespace odessa::core::serialization

namespace odessa::core::component {
void to_json(nlohmann::json& json, component::Position position);
void to_json(nlohmann::json& json, component::Velocity velocity);
void to_json(nlohmann::json& json, component::LLA lla);
void from_json(const nlohmann::json& json, component::Position& position);
void from_json(const nlohmann::json& json, component::Velocity& velocity);
void from_json(const nlohmann::json& json, component::LLA& lla);
}  // namespace odessa::core::component

namespace odessa::core::entity {
void to_json(nlohmann::json& j, const entity::EntitySnapshot& snapshot);
void from_json(const nlohmann::json& j, entity::EntitySnapshot& snapshot);
}  // namespace odessa::core::entity

namespace odessa::core::event {
void to_json(nlohmann::json& j, const event::EntityMoved& event);
void to_json(nlohmann::json& j, const event::EntitySpawned& event);
void to_json(nlohmann::json& j, const event::EntityDestroyed& event);
}  // namespace odessa::core::event
