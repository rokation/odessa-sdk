#include <iostream>
#include "core/entity/entity.hpp"
#include "utils/uuid.hpp"

int main() {
  odessa::core::Entity entity{odessa::core::EntityType::DRONE};
  std::cout << entity.id() << std::endl;
  return 0;
}
