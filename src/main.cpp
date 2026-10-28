#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "Mission.h"
#include "StrikePlanner.h"
#include "Trajectory.h"

namespace {
    const Vec2 DRONE_CRUISE_VELOCITY{12.0, 0.0};  // m/s, along +X

    std::unique_ptr<ITrajectorySource> makeTrajectory(const std::string& name) {
        if (name == "hover") {
            return std::make_unique<HoverTrajectory>();
        }
        if (name == "forward") {
            return std::make_unique<ForwardThrowTrajectory>(DRONE_CRUISE_VELOCITY);
        }
        return nullptr;
    }

    void writeReport(std::ostream& out, const StrikePlan& plan) {
        out << std::fixed << std::setprecision(2)
            << plan.impact.x << ' ' << plan.impact.y << '\n'
            << plan.release.x << ' ' << plan.release.y << '\n'
            << plan.ammoName << '\n'
            << plan.destroyed << ' ' << plan.valueSum << ' ' << plan.price << ' ' << plan.net;
    }

}

int main(int argc, char* argv[]) {
    std::string inputFile;
    std::string outputFile;
    std::string trajectoryName = "hover";

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--input-file" && i + 1 < argc) {
            inputFile = argv[++i];
        } else if (arg == "--output-file" && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (arg == "--trajectory" && i + 1 < argc) {
            trajectoryName = argv[++i];
        } else {
            std::cerr << "Invalid argument: " << arg << '\n';
            return 1;
        }
    }

    if (inputFile.empty() || outputFile.empty()) {
        return 1;
    }

    std::unique_ptr<ITrajectorySource> trajectory = makeTrajectory(trajectoryName);
    if (!trajectory) {
        std::cerr << "Unknown trajectory: " << trajectoryName << '\n';
        return 1;
    }

    try {
        Mission mission = Mission::loadFromFile(inputFile);

        StrikePlanner planner(Ammo::arsenal(), *trajectory);
        StrikePlan plan = planner.plan(mission);

        std::ofstream out(outputFile);
        if (!out) {
            std::cerr << "Cannot open output file: " << outputFile << '\n';
            return 1;
        }
        writeReport(out, plan);
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}