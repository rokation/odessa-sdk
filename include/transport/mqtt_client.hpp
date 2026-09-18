#pragma once

#include <string>

class MqttClient {
 public:
  MqttClient(const std::string& client_id);
  ~MqttClient();

  bool connect(const std::string& host, int port);
  bool publish(const std::string& topic, const std::string& message);

 private:
  void* client_;
};
