#pragma once

#include <atomic>
#include <thread>

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
  std::thread runtime_thread_;
};
}  // namespace odessa::server
