// #include "transport/mqtt_client.hpp"

// #include <mosquitto.h>
// bool MqttClient::connect(const std::string& host, int port) {
//   return mosquitto_connect(client_, host.c_str(), port, 60) ==
//   MOSQ_ERR_SUCCESS;
// }

// bool MqttClient::publish(const std::string& topic, const std::string&
// message) {
//   return mosquitto_publish(client_, nullptr, topic.c_str(),
//                            static_cast<int>(message.size()), message.data(),
//                            0, false) == MOSQ_ERR_SUCCESS;
// }
