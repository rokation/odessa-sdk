#include "core/world/world.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <variant>

#include "core/entity/entity.hpp"
#include "core/event/event.hpp"

void test_spawn() {
  odessa::core::World world;
  auto id = world.spawn(odessa::core::EntityType::DRONE);

  assert(world.entity_count() == 1);
  assert(world.contains(id));
}

void test_update() {
  odessa::core::World world;

  auto id = world.spawn(odessa::core::EntityType::DRONE);

  world.set_velocity(id, {10.0, 0.0, 0.0});

  world.update(0.1);

  auto snapshot = world.snapshot(id);

  assert(snapshot->position.x == 1.0);
  assert(snapshot->position.y == 0.0);
  assert(snapshot->position.z == 0.0);
}

void test_events() {
  odessa::core::World world;
  auto id = world.spawn(odessa::core::EntityType::DRONE);
  auto events = world.consume_events();

  const auto& spawned = std::get<odessa::core::EntitySpawned>(events[0]);
  assert(spawned.event_id == id);

  world.set_velocity(id, {10.0, 0.0, 0.0});
  world.update(0.1);

  events = world.consume_events();
  const auto& moved = std::get<odessa::core::EntityMoved>(events[0]);

  assert(moved.event_id == id);

  assert(world.destroy(id));

  events = world.consume_events();
  const auto& destoryed = std::get<odessa::core::EntityDestroyed>(events[0]);
  assert(destoryed.event_id == id);
}

void test_destroy() {
  odessa::core::World world;

  auto id = world.spawn(odessa::core::EntityType::DRONE);

  assert(world.destroy(id));
  assert(world.entity_count() == 0);
  assert(!world.contains(id));
}

int main() {
  test_spawn();
  test_update();
  test_events();
  test_destroy();

  std::cout << "All tests passed" << std::endl;

  return 0;
}
