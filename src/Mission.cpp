#include "Mission.h"

#include <fstream>
#include <stdexcept>
#include <utility>

namespace {
    struct TypeValue {
        const char* type;
        int value;
    };

    const TypeValue TARGET_VALUES[] = {
        {"PERSONNEL", 10},
        {"LOGISTICS", 20},
        {"APC",       40},
        {"TANK",      60},
        {"RADAR",     90},
    };
}

Target::Target(const Vec2& position, int hp, const std::string& type)
    : position_(position), hp_(hp), type_(type) {}

const Vec2& Target::position() const { 
    return position_; 
}

int Target::hp() const { 
    return hp_; 
}

const std::string& Target::type() const { 
    return type_; 
}

int Target::value() const {
    for (const TypeValue& tv : TARGET_VALUES) {
        if (type_ == tv.type) {
            return tv.value;
        }
    }
    return 0;
}

Ammo::Ammo(const std::string& name, double radius_m, int damage, int price)
    : name_(name), radius_(radius_m), damage_(damage), price_(price) {}

const std::string& Ammo::name() const { 
    return name_; 
}

int Ammo::price() const { 
    return price_; 
}

bool Ammo::destroys(const Vec2& impact, const Target& target) const {
    return distance(impact, target.position()) <= radius_ && damage_ >= target.hp();
}

std::vector<Ammo> Ammo::arsenal() {
    return {
        Ammo("Frag", 60.0,  25,    35),
        Ammo("HE",   35.0,  55,    20),
        Ammo("HEAT", 15.0,  130,   45),
    };
}

Mission::Mission(double altitude_m, const Vec2& wind, std::vector<Target> targets)
    : altitude_(altitude_m), wind_(wind), targets_(std::move(targets)) {}

double Mission::altitude() const { 
    return altitude_; 
}

const Vec2& Mission::wind() const { 
    return wind_; 
}

const std::vector<Target>& Mission::targets() const { 
    return targets_; 
}

Mission Mission::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Cannot open input file: " + path);
    }

    double altitude_m = 0.0;
    Vec2 wind;
    int count = 0;
    if (!(in >> altitude_m >> wind.x >> wind.y >> count)) {
        throw std::runtime_error("Invalid scenario header");
    }
    if (altitude_m <= 0.0) {
        throw std::runtime_error("altitude_m must be > 0");
    }

    std::vector<Target> targets;
    for (int i = 0; i < count; i++) {
        Vec2 position;
        int hp = 0;
        std::string type;
        if (!(in >> position.x >> position.y >> hp >> type)) {
            throw std::runtime_error("Invalid target line " + std::to_string(i + 1));
        }
        targets.emplace_back(position, hp, type);
    }

    return Mission(altitude_m, wind, std::move(targets));
}