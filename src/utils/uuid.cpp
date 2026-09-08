
#include "utils/uuid.hpp"

namespace odesa::util {
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
}