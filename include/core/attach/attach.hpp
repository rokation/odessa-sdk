#pragma once

#include <optional>

#include "core/entity/entity.hpp"

namespace odessa::core::attach {
struct Attachment {
  odessa::core::entity::EntityId entity_id;
  std::string external_id;
};
}  // namespace odessa::core::attach
