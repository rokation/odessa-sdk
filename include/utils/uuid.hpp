#pragma once

#include <boost/uuid/uuid.hpp>
#include <string>

namespace odessa::util {
boost::uuids::uuid generate_uuid();
std::string generate_uuid_string();
}