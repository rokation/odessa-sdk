#include "server/server.hpp"

#include <thread>

namespace odessa::server {
void Server::run() {
  running_ = true;

  runtime_thread_ = std::thread([this]() { runtime_.run(); });
};

void Server::stop() {
  running_ = false;
  runtime_.stop();

  if (runtime_thread_.joinable()) {
    runtime_thread_.join();
  }
};

core::runtime::Runtime& Server::runtime() { return runtime_; }
}  // namespace odessa::server
