#include <boost/uuid/uuid_io.hpp>
#include <iomanip>
#include <iostream>
#include <thread>

#include "core/entity/entity.hpp"
#include "core/runtime/runtime.hpp"
#include "core/world/world.hpp"

int main() {
  std::cout << std::fixed << std::setprecision(2) << std::endl;
  odessa::core::Runtime runtime;

  odessa::core::EntityId drone_id =
      runtime.world().spawn(odessa::core::EntityType::DRONE);
  odessa::core::Position position(10.0, 0.0, 0.0);
  odessa::core::Velocity velocity(1.0, 0.0, 0.0);

  runtime.world().set_position(drone_id, position);
  runtime.world().set_velocity(drone_id, velocity);

  for (int i = 0; i < 10; ++i) {
    runtime.update(0.1);
    auto drone_snapshot = runtime.world().snapshot(drone_id);

    if (drone_snapshot) {
      std::cout << drone_snapshot->position.x << ", "
                << drone_snapshot->position.y << ", "
                << drone_snapshot->position.z << std::endl;
    }
  }

  // std::thread runtime_thread([&runtime] { runtime.run(); });
  // std::this_thread::sleep_for(std::chrono::seconds(5));

  // runtime.stop();

  // runtime_thread.join();
  return 0;
}
