#include "core/entity/entity.hpp"

#include "utils/uuid.hpp"

namespace odessa::core {
Entity::Entity(EntityType type) : id_(util::generate_uuid()), type_(type), lla_{}, position_{}, velocity_{}  {
}

boost::uuids::uuid Entity::id() const { return id_; }

LLA& Entity::lla() { return lla_; }

Position& Entity::position() { return position_; }

Velocity& Entity::velocity() { return velocity_; }

void Entity::set_lla(LLA lla) { lla_ = lla; }

void Entity::set_position(Position position) { position_ = position; }

void Entity::set_velocity(Velocity velocity) { velocity_ = velocity; }
}  // namespace odessa::core
