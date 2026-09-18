#pragma once

#include "core/command/command.hpp"
#include "core/world/world.hpp"
namespace odessa::core::runtime {
class Runtime {
 public:
  void run();
  void stop();
  void update(double dt);
  world::World& world();
  void execute(command::Command command);

 private:
  std::atomic<bool> running_{false};
  world::World world_;
};
}  // namespace odessa::core::runtime
