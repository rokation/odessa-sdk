#pragma once

#include <boost/uuid/uuid.hpp>

#include "core/components/lla.hpp"
#include "core/components/position.hpp"
#include "core/components/velocity.hpp"

namespace odessa::core {
struct EntitySnapshot;
enum class EntityType { DRONE, ROBOT, SENSOR, VEHICLE, CAMERA, VESSEL };
using EntityId = boost::uuids::uuid;
class Entity {
 public:
  explicit Entity(EntityType type);
  boost::uuids::uuid id() const;
  EntityType type() const;

  LLA lla() const;
  void set_lla(LLA lla);

  Position position() const;
  void set_position(Position position);

  Velocity velocity() const;
  void set_velocity(Velocity velocity);

  EntitySnapshot snapshot() const;
  void update(double dt);

 private:
  boost::uuids::uuid id_;
  EntityType type_;
  LLA lla_;
  Position position_;
  Velocity velocity_;
};

struct EntitySnapshot {
  EntityId id;
  EntityType type;

  LLA lla;
  Position position;
  Velocity velocity;
};
}  // namespace odessa::core
