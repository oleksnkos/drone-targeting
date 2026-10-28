#include "StrikePlanner.h"

StrikePlanner::StrikePlanner(const std::vector<Ammo>& arsenal, const ITrajectorySource& trajectory)
    : arsenal_(arsenal), trajectory_(trajectory) {}

StrikePlan StrikePlanner::plan(const Mission& mission) const {
    const std::vector<Target>& targets = mission.targets();

    bool found = false;
    int bestAmmo = 0;
    int bestTarget = 0;
    StrikePlan best;

    for (int a = 0; a < static_cast<int>(arsenal_.size()); a++) {
        const Ammo& ammo = arsenal_[a];

        for (int i = 0; i < static_cast<int>(targets.size()); i++) {
            const Vec2& impact = targets[i].position();

            int valueSum = 0;
            int destroyed = 0;
            for (const Target& t : targets) {
                if (ammo.destroys(impact, t)) {
                    valueSum += t.value();
                    destroyed++;
                }
            }
            int net = valueSum - ammo.price();

            // higher net >> then cheaper ammo >> then lower target index
            bool better = !found // no best yet
                || net > best.net // net value is larger then the current best
                || (net == best.net && ammo.price() < arsenal_[bestAmmo].price()) // ammo price is lower
                || (net == best.net && ammo.price() == arsenal_[bestAmmo].price() && i < bestTarget); // index is lower

            if (better) {
                found = true;
                bestAmmo = a;
                bestTarget = i;
                best.impact = impact;
                best.ammoName = ammo.name();
                best.destroyed = destroyed;
                best.valueSum = valueSum;
                best.price = ammo.price();
                best.net = net;
            }
        }
    }

    best.release = trajectory_.release(best.impact, mission.altitude(), mission.wind());
    return best;
}