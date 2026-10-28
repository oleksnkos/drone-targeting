#include "Trajectory.h"

#include <cmath>

// noname namespace; visible only within this file
namespace {
    const double G = 9.81;

    double fallTime(double altitude_m) {
        return std::sqrt(2.0 * altitude_m / G);
    }
}

Vec2 HoverTrajectory::release(const Vec2& impact, double altitude_m, const Vec2& wind) const {
    return impact - wind * fallTime(altitude_m);
}

// constructor
ForwardThrowTrajectory::ForwardThrowTrajectory(const Vec2& droneVelocity)
    : droneVelocity_(droneVelocity) {}

Vec2 ForwardThrowTrajectory::release(const Vec2& impact, double altitude_m, const Vec2& wind) const {
    return impact - (wind + droneVelocity_) * fallTime(altitude_m);
}