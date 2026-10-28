#pragma once

#include <string>
#include <vector>

#include "Vec2.h"

class Target {
    public:
        Target(const Vec2& position, int hp, const std::string& type);

        const Vec2& position() const;
        int hp() const;
        const std::string& type() const;
        int value() const; // looked up by type

    private:
        Vec2 position_;
        int hp_;
        std::string type_;
};

class Ammo {
    public:
        Ammo(const std::string& name, double radius_m, int damage, int price);

        const std::string& name() const;
        int price() const;

        // Target is inside the blast radius around impact and damage >= hp
        bool destroys(const Vec2& impact, const Target& target) const;

        static std::vector<Ammo> arsenal();

    private:
        std::string name_;
        double radius_;
        int damage_;
        int price_;
};

class Mission {
    public:
        // Reads and validates the scenario file
        static Mission loadFromFile(const std::string& path);

        double altitude() const;
        const Vec2& wind() const;
        const std::vector<Target>& targets() const;

    private:
        Mission(double altitude_m, const Vec2& wind, std::vector<Target> targets);

        double altitude_;
        Vec2 wind_;
        std::vector<Target> targets_;
};