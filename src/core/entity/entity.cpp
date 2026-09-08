#include "core/entity/entity.hpp"
#include "utils/uuid.hpp"

odesa::core::Entity::Entity(EntityType type) : type_(type) {
  id_ = odesa::util::generate_uuid();
}

const boost::uuids::uuid odesa::core::Entity::id() const {
  return id_;
}