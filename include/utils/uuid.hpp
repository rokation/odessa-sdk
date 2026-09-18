#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>

namespace odessa::util {
boost::uuids::uuid generate_uuid();
std::string generate_uuid_string();
std::string uuid_to_string(boost::uuids::uuid);
boost::uuids::uuid string_to_uuid(const std::string& value);
}  // namespace odessa::util
