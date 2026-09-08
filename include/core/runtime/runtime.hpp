#pragma once

#include "core/world/world.hpp"

namespace odessa::core {
class Runtime {
 public:
  void run();
  void stop();
  void update(double dt);
  World& world();

 private:
  std::atomic<bool> running_{false};
  World world_;
};
}  // namespace odessa::core