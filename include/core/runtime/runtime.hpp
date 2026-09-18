#pragma once

#include "core/world/world.hpp"
namespace odessa::core::runtime {
class Runtime {
 public:
  void run();
  void stop();
  void update(double dt);
  world::World& world();

 private:
  std::atomic<bool> running_{false};
  world::World world_;
};
}  // namespace odessa::core::runtime
