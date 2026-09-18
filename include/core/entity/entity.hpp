#pragma once

#include <boost/uuid/uuid.hpp>

#include "core/component/lla.hpp"
#include "core/component/position.hpp"
#include "core/component/velocity.hpp"

namespace odessa::core::entity {
struct EntitySnapshot;
enum class EntityType {
  DRONE,
  ROBOT,
  SENSOR,
  VEHICLE,
  CAMERA,
  VESSEL,
  UNKNOWN
};
using EntityId = boost::uuids::uuid;
class Entity {
 public:
  explicit Entity(EntityType type);
  EntityId id() const;
  EntityType type() const;

  component::LLA lla() const;
  void set_lla(component::LLA lla);

  component::Position position() const;
  void set_position(component::Position position);

  component::Velocity velocity() const;
  void set_velocity(component::Velocity velocity);

  EntitySnapshot snapshot() const;
  void update(double dt);

 private:
  EntityId id_;
  EntityType type_;
  component::LLA lla_;
  component::Position position_;
  component::Velocity velocity_;
};

struct EntitySnapshot {
  EntityId id;
  EntityType type;

  component::LLA lla;
  component::Position position;
  component::Velocity velocity;
};
}  // namespace odessa::core::entity
