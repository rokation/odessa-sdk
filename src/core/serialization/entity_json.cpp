#include "core/serialization/entity_json.hpp"

#include "boost/uuid.hpp"
#include "core/component/lla.hpp"
#include "core/component/position.hpp"
#include "core/component/velocity.hpp"
#include "core/entity/entity.hpp"
#include "core/event/event.hpp"

namespace odessa::core::serialization {
std::string entity_type_to_string(entity::EntityType type) {
  if (type == entity::EntityType::DRONE) return "drone";
  if (type == entity::EntityType::ROBOT) return "robot";
  if (type == entity::EntityType::SENSOR) return "sensor";
  if (type == entity::EntityType::VEHICLE) return "vehicle";
  if (type == entity::EntityType::CAMERA) return "camera";
  if (type == entity::EntityType::VESSEL) return "vessel";

  return "unknown";
}

entity::EntityType entity_type_from_string(const std::string& type) {
  if (type == "drone") return entity::EntityType::DRONE;
  if (type == "robot") return entity::EntityType::ROBOT;
  if (type == "sensor") return entity::EntityType::SENSOR;
  if (type == "vehicle") return entity::EntityType::VEHICLE;
  if (type == "camera") return entity::EntityType::CAMERA;
  if (type == "vessel") return entity::EntityType::VESSEL;

  return entity::EntityType::UNKNOWN;
}
}  // namespace odessa::core::serialization

namespace odessa::core::component {
void to_json(nlohmann::json& json, component::Position position) {
  json = {
      {"x", position.x},
      {"y", position.y},
      {"z", position.z},
  };
}

void to_json(nlohmann::json& json, component::Velocity velocity) {
  json = {
      {"x", velocity.x},
      {"y", velocity.y},
      {"z", velocity.z},
  };
}

void to_json(nlohmann::json& json, component::LLA lla) {
  json = {
      {"lat", lla.lat},
      {"lon", lla.lon},
      {"alt", lla.alt},
  };
}

void from_json(const nlohmann::json& json, component::Position& position) {
  position.x = json.at("x").get<double>();
  position.y = json.at("y").get<double>();
  position.z = json.at("z").get<double>();
}

void from_json(const nlohmann::json& json, component::Velocity& velocity) {
  velocity.x = json.at("x").get<double>();
  velocity.y = json.at("y").get<double>();
  velocity.z = json.at("z").get<double>();
}

void from_json(const nlohmann::json& json, component::LLA& lla) {
  lla.lat = json.at("lat").get<double>();
  lla.lon = json.at("lon").get<double>();
  lla.alt = json.at("alt").get<double>();
}
}  // namespace odessa::core::component

namespace odessa::core::entity {
void to_json(nlohmann::json& json, const entity::EntitySnapshot& snapshot) {
  json = {
      {"id", boost::uuids::to_string(snapshot.id)},
      {"type", serialization::entity_type_to_string(snapshot.type)},
      {"lla", snapshot.lla},
      {"position", snapshot.position},
      {"velocity", snapshot.velocity},
  };
}

void from_json(const nlohmann::json& json, entity::EntitySnapshot& snapshot) {
  snapshot.id =
      boost::uuids::string_generator{}(json.at("id").get<std::string>());

  snapshot.type = serialization::entity_type_from_string(
      json.at("type").get<std::string>());

  snapshot.lla = json.at("lla").get<component::LLA>();
  snapshot.position = json.at("position").get<component::Position>();
  snapshot.velocity = json.at("velocity").get<component::Velocity>();
}
}  // namespace odessa::core::entity

namespace odessa::core::event {
void to_json(nlohmann::json& json, const event::EntityMoved& event) {
  json = {{"event", "entity_moved"},
          {"entity_id", event.entity_id},
          {"snapshot", event.snapshot}};
}

void to_json(nlohmann::json& json, const event::EntitySpawned& event) {
  json = {{"event", "entity_spawned"}, {"entity_id", event.entity_id}};
}

void to_json(nlohmann::json& json, const event::EntityDestroyed& event) {
  json = {{"event", "entity_destroyed"}, {"entity_id", event.entity_id}};
}
}  // namespace odessa::core::event
