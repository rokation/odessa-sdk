#pragma once

#include <boost/uuid/uuid.hpp>

namespace odesa::core {
enum class EntityType { DRONE, ROBOT, SENSOR, VEHICLE, CAMERA, VESSEL };
class Entity {
public:
  Entity(EntityType type);
  const boost::uuids::uuid id() const;
private:
  boost::uuids::uuid id_;
  EntityType type_;
};
} // namespace odesa::core