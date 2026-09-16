#include "core/serialization/entity_json.hpp"

#include "core/entity/entity.hpp"
#include "utils/uuid.hpp"
namespace odessa::core {
nlohmann::json to_json(const odessa::core::EntitySnapshot& snapshot) {
  return {
      {"id", boost::uuids::to_string(snapshot.id)},
      {"type", entity_type_to_string(snapshot.type)},
      {"lla",
       {
           {"lat", snapshot.lla.lat},
           {"lon", snapshot.lla.lon},
           {"alt", snapshot.lla.alt},
       }},
      {"position",
       {
           {"x", snapshot.position.x},
           {"y", snapshot.position.y},
           {"z", snapshot.position.z},
       }},
      {"velocity",
       {
           {"x", snapshot.velocity.x},
           {"y", snapshot.velocity.y},
           {"z", snapshot.velocity.z},
       }},
  };
}

std::string entity_type_to_string(EntityType type) {
  switch (type) {
    case EntityType::DRONE:
      return "drone";
    case EntityType::ROBOT:
      return "robot";
    case EntityType::CAMERA:
      return "camera";
    case EntityType::VEHICLE:
      return "vehicle";
    case EntityType::VESSEL:
      return "vessel";
    case EntityType::SENSOR:
      return "sensor";
  }

  return "unknown";
}
}  // namespace odessa::core
