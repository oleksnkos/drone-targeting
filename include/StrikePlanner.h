#pragma once

#include <string>
#include <vector>

#include "Mission.h"
#include "Trajectory.h"
#include "Vec2.h"

struct StrikePlan {
    Vec2 impact;
    Vec2 release;
    std::string ammoName;
    int destroyed = 0;
    int valueSum = 0;
    int price = 0;
    int net = 0;
};

class StrikePlanner {
    public:
        StrikePlanner(const std::vector<Ammo>& arsenal, const ITrajectorySource& trajectory);

        StrikePlan plan(const Mission& mission) const;

    private:
        std::vector<Ammo> arsenal_;
        const ITrajectorySource& trajectory_;
};