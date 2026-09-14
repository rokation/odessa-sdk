#include "core/entity/entity.hpp"

#include "utils/uuid.hpp"

namespace odessa::core {
Entity::Entity(EntityType type)
    : id_(util::generate_uuid()),
      type_(type),
      lla_{},
      position_{},
      velocity_{} {}

boost::uuids::uuid Entity::id() const { return id_; }

EntityType Entity::type() const { return type_; }

LLA Entity::lla() const { return lla_; }

Position Entity::position() const { return position_; }

Velocity Entity::velocity() const { return velocity_; }

void Entity::set_lla(LLA lla) { lla_ = lla; }

void Entity::set_position(Position position) { position_ = position; }

void Entity::set_velocity(Velocity velocity) { velocity_ = velocity; }

void Entity::update(double dt) {
  position_.x += velocity_.x * dt;
  position_.y += velocity_.y * dt;
  position_.z += velocity_.z * dt;
}

EntitySnapshot Entity::snapshot() const {
  return EntitySnapshot { id_, type_, lla_, position_, velocity_ }
}

}  // namespace odessa::core
