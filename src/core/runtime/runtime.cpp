#include "core/runtime/runtime.hpp"

#include <algorithm>
#include <thread>
#include <type_traits>
#include <variant>

#include "core/command/command.hpp"
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

void Runtime::execute(command::Command command) {
  std::visit(
      [this](auto&& command) {
        using T = std::decay_t<decltype(command)>;

        if constexpr (std::is_same_v<T, command::SpawnEntity>) {
          world_.spawn(command.entity_type);
        } else if constexpr (std::is_same_v<T, command::DestroyEntity>) {
          world_.destroy(command.entity_id);
        } else if constexpr (std::is_same_v<T, command::SetPosition>) {
          world_.set_position(command.entity_id, command.position);
        } else if constexpr (std::is_same_v<T, command::SetVelocity>) {
          world_.set_velocity(command.entity_id, command.velocity);
        }
      },
      std::move(command));
}

}  // namespace odessa::core::runtime
