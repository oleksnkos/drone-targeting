#pragma once

#include "Vec2.h"

// interface
class ITrajectorySource {
    public:
        virtual ~ITrajectorySource() = default;

        virtual Vec2 release(const Vec2& impact, double altitude_m, const Vec2& wind) const = 0;
};

// release = impact - wind * t
class HoverTrajectory : public ITrajectorySource {
    public:
        Vec2 release(const Vec2& impact, double altitude_m, const Vec2& wind) const override;
};

// release = impact - (wind + droneVelocity) * t
class ForwardThrowTrajectory : public ITrajectorySource {
    public:
        explicit ForwardThrowTrajectory(const Vec2& droneVelocity);

        Vec2 release(const Vec2& impact, double altitude_m, const Vec2& wind) const override;

    private:
        Vec2 droneVelocity_;
};