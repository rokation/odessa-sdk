#pragma once

#include <string>

#include "core/entity/entity.hpp"

namespace odessa::core {
struct Attachment {
  EntityId entity_id;
  std::string external_id;
};
}  // namespace odessa::core
