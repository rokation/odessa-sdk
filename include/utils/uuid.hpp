#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>

namespace odessa::util {
boost::uuids::uuid generate_uuid();
std::string generate_uuid_string();
std::string uuid_string_from_raw(boost::uuids::uuid);
}  // namespace odessa::util
