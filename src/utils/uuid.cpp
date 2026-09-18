#include "utils/uuid.hpp"

namespace odessa::util {
boost::uuids::uuid generate_uuid() {
  boost::uuids::random_generator gen;
  boost::uuids::uuid id = gen();

  return id;
}

std::string generate_uuid_string() {
  boost::uuids::random_generator gen;
  boost::uuids::uuid id = gen();

  std::string uuid_str = boost::uuids::to_string(id);
  return uuid_str;
}

std::string uuid_to_string(boost::uuids::uuid uuid) {
  return boost::uuids::to_string(uuid);
}

boost::uuids::uuid string_to_uuid(const std::string& value) {
  boost::uuids::string_generator generator;
  return generator(value);
}
}  // namespace odessa::util
