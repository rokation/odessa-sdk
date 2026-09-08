#pragma once

#include <boost/uuid/uuid.hpp>

#include "core/components/lla.hpp"
#include "core/components/position.hpp"
#include "core/components/velocity.hpp"

namespace odessa::core {
enum class EntityType { DRONE, ROBOT, SENSOR, VEHICLE, CAMERA, VESSEL };
using EntityId = boost::uuids::uuid;
class Entity {
 public:
  explicit Entity(EntityType type);
  boost::uuids::uuid id() const;
  LLA lla();
  Position position();
  Velocity velocity();
  void set_lla(LLA lla);
  void set_position(Position position);
  void set_velocity(Velocity velocity);

 private:
  boost::uuids::uuid id_;
  EntityType type_;
  LLA lla_;
  Position position_;
  Velocity velocity_;
};
}  // namespace odessa::core