#include "core/coordinate/transform.hpp"

#include <cmath>

namespace odessa::core {
namespace {
constexpr double k_pi = 3.14159265358979323846;
constexpr double k_wgs84_a = 6378137.0;
constexpr double k_wgs84_e2 = 6.69437999014e-3;

struct Ecef {
  double x;
  double y;
  double z;
};

Ecef lla_to_ecef(const LLA& lla) {
  const double lat = lla.lat * M_PI / 180.0;
  const double lon = lla.lon * M_PI / 180.0;

  const double sin_lat = std::sin(lat);
  const double cos_lat = std::cos(lat);

  const double n = k_wgs84_a / std::sqrt(1.0 - k_wgs84_e2 * sin_lat);

  return {(n + lla.alt) * cos_lat * std::cos(lon),
          (n + lla.alt) * cos_lat * std::sin(lon),
          (n * (1.0 - k_wgs84_e2) + lla.alt) * sin_lat};
}

Ecef enu_to_ecef(const Position& position, const Ecef& origin,
                 const LLA& origin_lla) {
  const double lat = origin_lla.lat * k_pi / 180.0;
  const double lon = origin_lla.lon * k_pi / 180.0;

  const double east = position.x;
  const double north = position.y;
  const double up = position.z;

  return {
      origin.x - std::sin(lon) * east - std::sin(lat) * std::cos(lon) * north +
          std::cos(lat) * std::cos(lon) * up,

      origin.y + std::cos(lon) * east - std::sin(lat) * std::sin(lon) * north +
          std::cos(lat) * std::sin(lon) * up,

      origin.z + std::cos(lat) * north + std::sin(lat) * up};
}

Position ecef_to_enu(const Ecef& target, const Ecef& origin,
                     const LLA& origin_lla) {
  const double lat = origin_lla.lat * k_pi / 180.0;
  const double lon = origin_lla.lon * k_pi / 180.0;

  const double dx = target.x - origin.x;
  const double dy = target.y - origin.y;
  const double dz = target.z - origin.z;

  return {-std::sin(lon) * dx + std::cos(lon) * dy,
          -std::sin(lat) * std::cos(lon) * dx -
              std::sin(lat) * std::sin(lon) * dy + std::cos(lat) * dz,
          std::cos(lat) * std::cos(lon) * dx +
              std::cos(lat) * std::sin(lon) * dy + std::sin(lat) * dz};
}

LLA ecef_to_lla(const Ecef& ecef) {
  constexpr double a = k_wgs84_a;
  constexpr double e2 = k_wgs84_e2;

  const double p = std::sqrt(ecef.x * ecef.x + ecef.y * ecef.y);

  const double lon = std::atan2(ecef.y, ecef.x);

  double lat = std::atan2(ecef.z, p * (1.0 - e2));

  double alt = 0.0;

  for (int i = 0; i < 10; ++i) {
    const double sin_lat = std::sin(lat);

    const double n = a / std::sqrt(1.0 - e2 * sin_lat * sin_lat);

    alt = p / std::cos(lat) - n;

    lat = std::atan2(ecef.z, p * (1.0 - e2 * n / (n + alt)));
  }

  return {lat * 180.0 / k_pi, lon * 180.0 / k_pi, alt};
}
}  // namespace

Position lla_to_enu(const LLA& lla, const LLA& origin) {
  const auto target_ecef = lla_to_ecef(lla);
  const auto origin_ecef = lla_to_ecef(origin);

  return ecef_to_enu(target_ecef, origin_ecef, origin);
}

LLA enu_to_lla(const Position& position, const LLA& origin) {
  const auto origin_ecef = lla_to_ecef(origin);

  const auto target_ecef = enu_to_ecef(position, origin_ecef, origin);

  return ecef_to_lla(target_ecef);
}
}  // namespace odessa::core
