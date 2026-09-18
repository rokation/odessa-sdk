#include <boost/uuid/uuid_io.hpp>
#include <iomanip>
#include <iostream>
#include <thread>

#include "core/entity/entity.hpp"
#include "core/runtime/runtime.hpp"
#include "core/serialization/entity_json.hpp"
#include "core/world/world.hpp"

int main() {
  std::cout << std::fixed << std::setprecision(2) << std::endl;
  odessa::core::runtime::Runtime runtime;
  auto& world = runtime.world();
  auto drone =
      world.attach(odessa::core::entity::EntityType::DRONE, "DRONE-001");
  world.set_velocity(drone, {1.0, 0.0, 0.0});
  std::thread t1([&]() {
    runtime.run();
    runtime.update(1.0);
  });

  std::this_thread::sleep_for(std::chrono::duration<double>(1));
  auto snapshots = runtime.world().snapshot_all();

  for (const auto snapshot : snapshots) {
    nlohmann::json j = snapshot;
    std::cout << j.dump(2) << std::endl;
  }
  t1.join();

  return 0;
}
