#include "server/server.hpp"

#include <thread>

namespace odessa::server {
Server::~Server() { stop(); }

void Server::run() {
  if (running_) {
    return;
  }

  running_ = true;

  runtime_thread_ = std::thread([this]() { runtime_.run(); });
};

void Server::stop() {
  if (!running_) {
    return;
  }

  running_ = false;
  runtime_.stop();

  if (runtime_thread_.joinable()) {
    runtime_thread_.join();
  }
};

core::runtime::Runtime& Server::runtime() { return runtime_; }
}  // namespace odessa::server
