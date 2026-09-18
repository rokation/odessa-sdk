#include "core/runtime/runtime.hpp"

#include <thread>

#include "core/world/world.hpp"

namespace odessa::core::runtime {
void Runtime::run() {
  running_ = true;

  auto previous = std::chrono::steady_clock::now();

  while (running_) {
    auto current = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = current - previous;

    double dt = elapsed.count();

    previous = current;

    update(dt);

    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
}

void Runtime::stop() { running_ = false; }

void Runtime::update(double dt) { world().update(dt); }

world::World& Runtime::world() { return world_; }
}  // namespace odessa::core::runtime
