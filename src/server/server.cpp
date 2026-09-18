#include "server/server.hpp"

namespace odessa::server {
void Server::run() {
  running_ = true;
  runtime_.run();
};

void Server::stop() {
  running_ = false;
  runtime_.stop();
};

core::runtime::Runtime& Server::runtime() { return runtime_; }
}  // namespace odessa::server
