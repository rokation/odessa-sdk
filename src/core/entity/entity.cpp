#include "core/entity/entity.hpp"
#include "utils/uuid.hpp"

odessa::core::Entity::Entity(EntityType type) : type_(type) {
  id_ = odessa::util::generate_uuid();
}

const boost::uuids::uuid odessa::core::Entity::id() const {
  return id_;
}