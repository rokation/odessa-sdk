#pragma once

#include "core/components/lla.hpp"
#include "core/components/velocity.hpp"
#include "core/entity/entity.hpp"
#include "core/event/event.hpp"
#include "nlohmann/json.hpp"

namespace odessa::core {
std::string entity_type_to_string(EntityType type);
EntityType entity_type_from_string(const std::string& type);

void to_json(nlohmann::json& json, Position position);
void from_json(const nlohmann::json& json, Position& position);

void to_json(nlohmann::json& json, Velocity velocity);
void from_json(const nlohmann::json& json, Velocity& velocity);

void to_json(nlohmann::json& json, LLA lla);
void from_json(const nlohmann::json& json, LLA& lla);

void to_json(nlohmann::json& j, const EntitySnapshot& snapshot);
void from_json(const nlohmann::json& j, EntitySnapshot& snapshot);

void to_json(nlohmann::json& j, const EntityMoved& event);
void to_json(nlohmann::json& j, const EntitySpawned& event);
void to_json(nlohmann::json& j, const EntityDestroyed& event);
}  // namespace odessa::core
