#pragma once

#include <atomic>

#include "core/runtime/runtime.hpp"
namespace odessa::server {
class Server {
 public:
  void run();
  void stop();

  core::runtime::Runtime& runtime();

 private:
  std::atomic<bool> running_{false};
  odessa::core::runtime::Runtime runtime_;
};
}  // namespace odessa::server
