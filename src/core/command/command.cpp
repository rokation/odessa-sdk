#include "core/command/command.hpp"

#include <stdexcept>
#include <string>

#include "core/component/position.hpp"
#include "core/component/velocity.hpp"
#include "core/serialization/entity_json.hpp"
#include "nlohmann/json.hpp"
#include "utils/uuid.hpp"

namespace odessa::core::command {
void from_json(const nlohmann::json& json, SpawnEntity& command) {
  command.entity_type = json.at("entity_type");
}

void from_json(const nlohmann::json& json, DestroyEntity& command) {
  command.entity_id =
      util::string_to_uuid(json.at("entity_id").get<std::string>());
}

void from_json(const nlohmann::json& json, SetPosition& command) {
  command.entity_id =
      util::string_to_uuid(json.at("entity_id").get<std::string>());

  command.position = json.at("position").get<component::Position>();
}

void from_json(const nlohmann::json& json, SetVelocity& command) {
  command.entity_id =
      util::string_to_uuid(json.at("entity_id").get<std::string>());

  command.velocity = json.at("velocity").get<component::Velocity>();
}

void from_json(const nlohmann::json& json, Command& command) {
  const auto type = json.at("type").get<std::string>();

  if (type == "spawn_entity") {
    command = json.get<DestroyEntity>();
    return;
  }

  if (type == "destroy_entity") {
    command = json.get<DestroyEntity>();
    return;
  }

  if (type == "set_position") {
    command = json.get<SetPosition>();
    return;
  }

  if (type == "set_velocity") {
    command = json.get<SetVelocity>();
    return;
  }

  throw std::invalid_argument("unknown command type: " + type);
}
}  // namespace odessa::core::command
