#pragma once

#include "core/entity/entity.hpp"
#include "nlohmann/json.hpp"

namespace odessa::core {
nlohmann::json to_json(const EntitySnapshot& snapshot);
std::string entity_type_to_string(EntityType type);
}  // namespace odessa::core
