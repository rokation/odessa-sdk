#include "core/runtime/runtime.hpp"

#include <chrono>
#include <thread>

#include "core/entity/entity.hpp"

void test_runtime_run() {
  odessa::core::Runtime runtime;
  auto id = runtime.world().spawn(odessa::core::EntityType::DRONE);
  runtime.world().set_velocity(id, {10.0, 0.0, 0.0});

  std::thread thread([&runtime] { runtime.run(); });

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  runtime.stop();
  thread.join();

  auto snapshot = runtime.world().snapshot(id);
  assert(snapshot.has_value());
  assert(snapshot->position.x > 0.0);
}

void test_runtime_update() {
  odessa::core::Runtime runtime;
  auto id = runtime.world().spawn(odessa::core::EntityType::DRONE);
  runtime.world().set_velocity(id, {10.0, 0.0, 0.0});
  runtime.update(0.1);

  auto snapshot = runtime.world().snapshot(id);

  assert(snapshot.has_value());
  assert(snapshot->position.x == 1.0);
}

int main() {
  test_runtime_run();
  test_runtime_update();
  return 0;
}
