#include "core/serialization/entity_json.hpp"

#include <ios>

#include "boost/uuid.hpp"
#include "core/components/position.hpp"
#include "core/components/velocity.hpp"
#include "core/entity/entity.hpp"

namespace odessa::core {

std::string entity_type_to_string(EntityType type) {
  if (type == EntityType::DRONE) return "drone";
  if (type == EntityType::ROBOT) return "robot";
  if (type == EntityType::SENSOR) return "sensor";
  if (type == EntityType::VEHICLE) return "vehicle";
  if (type == EntityType::CAMERA) return "camera";
  if (type == EntityType::VESSEL) return "vessel";

  return "unknown";
}

EntityType entity_type_from_string(const std::string& type) {
  if (type == "drone") return EntityType::DRONE;
  if (type == "robot") return EntityType::ROBOT;
  if (type == "sensor") return EntityType::SENSOR;
  if (type == "vehicle") return EntityType::VEHICLE;
  if (type == "camera") return EntityType::CAMERA;
  if (type == "vessel") return EntityType::VESSEL;

  return EntityType::UNKNOWN;
}

void to_json(nlohmann::json& json, Position position) {
  json = {
      {"x", position.x},
      {"y", position.y},
      {"z", position.z},
  };
}

void from_json(const nlohmann::json& json, Position& position) {
  position.x = json.at("x").get<double>();
  position.y = json.at("y").get<double>();
  position.z = json.at("z").get<double>();
}

void to_json(nlohmann::json& json, Velocity velocity) {
  json = {
      {"x", velocity.x},
      {"y", velocity.y},
      {"z", velocity.z},
  };
}

void from_json(const nlohmann::json& json, Velocity& velocity) {
  velocity.x = json.at("x").get<double>();
  velocity.y = json.at("y").get<double>();
  velocity.z = json.at("z").get<double>();
}

void to_json(nlohmann::json& json, LLA lla) {
  json = {
      {"lat", lla.lat},
      {"lon", lla.lon},
      {"alt", lla.alt},
  };
}

void from_json(const nlohmann::json& json, LLA& lla) {
  lla.lat = json.at("lat").get<double>();
  lla.lon = json.at("lon").get<double>();
  lla.alt = json.at("alt").get<double>();
}

void to_json(nlohmann::json& j, const EntitySnapshot& snapshot) {
  j = {
      {"id", boost::uuids::to_string(snapshot.id)},
      {"type", entity_type_to_string(snapshot.type)},
      {"lla", snapshot.lla},
      {"position", snapshot.position},
      {"velocity", snapshot.velocity},
  };
}

void from_json(const nlohmann::json& j, EntitySnapshot& snapshot) {
  snapshot.id = boost::uuids::string_generator{}(j.at("id").get<std::string>());

  snapshot.type = entity_type_from_string(j.at("type").get<std::string>());

  snapshot.lla = j.at("lla").get<LLA>();
  snapshot.position = j.at("position").get<Position>();
  snapshot.velocity = j.at("velocity").get<Velocity>();
}

void to_json(nlohmann::json& j, const EntityMoved& event) {
  j = {{"event", "entity_moved"},
       {"entity_id", event.entity_id},
       {"snapshot", event.snapshot}};
}

void to_json(nlohmann::json& j, const EntitySpawned& event) {
  j = {{"event", "entity_spawned"}, {"entity_id", event.entity_id}};
}

void to_json(nlohmann::json& j, const EntityDestroyed& event) {
  j = {{"event", "entity_destroyed"}, {"entity_id", event.entity_id}};
}
}  // namespace odessa::core
