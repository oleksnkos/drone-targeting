#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <vector>

const double G = 9.81;
const double PI = 3.14159;

double timeOfFall(double& altitude_m){
    return std::sqrt((2 * altitude_m) / G);
}

double dir(const double* release){
    return atan2(release[0], release[1]) * (180 / PI);
}

double distance(const double* release, const double* target){
    return std::sqrt(pow((release[0] - target[0]), 2) + pow((release[1] - target[1]), 2));
}

void step(double& dist, const double& step_m, const double& direction, const double* release, double* current){
    if(dist > step_m) {
        double direction_rad = direction * PI / 180.0;

        current[0] += step_m * sin(direction_rad);
        current[1] += step_m * cos(direction_rad);
    }
    else{
        current[0] = release[0];
        current[1] = release[1];
    }
}

struct Ammo{
    double radious;
    double damage;
    double price;
    std::string type;
};

struct Target{
    double target_x;
    double target_y;
    double hp;
    std::string type;
};

int getValue(const Target& t){
    if(t.type == "PERSONNEL"){
        return 10;
    }else if(t.type == "LOGISTICS"){
        return 20;
    }else if(t.type == "APC"){
        return 40;
    }else if(t.type == "TANK"){
        return 60;
    }else if(t.type == "RADAR"){
        return 90;
    }else{
        return 0;
    }
}

int main(int argc, char* argv[]) {

    std::string input_file;
    std::string output_file;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--input-file" && i + 1 < argc) {
            input_file = argv[++i];
        }
        else if (arg == "--output-file" && i + 1 < argc) {
            output_file = argv[++i];
        }
        else {
            std::cerr << "Invalid argument: " << arg << '\n';
            return 1;
        }
    }

    if (input_file.empty() || output_file.empty()) {
        std::cerr << "Usage: ./drop_calc --input-file <path> --output-file <path>\n";
        return 1;
    }

    Ammo Frag = {60, 25, 35, "Frag"};
    Ammo HE = {35, 55, 20, "HE"};
    Ammo HEAT = {15, 130, 45, "HEAT"};
    std::vector<Ammo> ammo = {Frag, HE, HEAT};
    
    std::ifstream in(input_file);
    if (!in) {
        std::cerr << "Cannot open input file: " << input_file << '\n';
        return 1;
    }

    double altitude_m, wind_x, wind_y;
    int numOfTargets;
    in >> altitude_m >> wind_x >> wind_y >> numOfTargets; //reading from the file

    std::vector<Target> targets(numOfTargets);
    for (int i = 0; i < numOfTargets; i++){
        in >> targets[i].target_x >> targets[i].target_y >> targets[i].hp >> targets[i].type;
    }

    std::vector<int> bestChoice(2);
    int best = 0;
    int destroyed = 0;

    for (int a = 0; a < 3; a++) {
        for (int i = 0; i < numOfTargets; i++) {

            int totalValue = 0;
            int dest = 0;

            for (int j = 0; j < numOfTargets; j++) {

                double point1[] = {targets[i].target_x, targets[i].target_y};
                double point2[] = {targets[j].target_x, targets[j].target_y};

                if(distance(point1, point2) <= ammo[a].radious && ammo[a].damage >= targets[j].hp){
                    totalValue += getValue(targets[j]);
                    dest++;
                }
            
            }

            int net = totalValue - ammo[a].price;

            if(best < net){
                bestChoice[0] = a;                
                bestChoice[1] = i;
                best = net;
                destroyed = dest;
            }else if(best == net){
                if(ammo[a].price < ammo[bestChoice[0]].price){
                    bestChoice[0] = a;
                    bestChoice[1] = i;
                }else if(ammo[a].price == ammo[bestChoice[0]].price){
                    bestChoice[1] = (i < bestChoice[1]) ? i : bestChoice[1];
                }
            }
        }
    }

    double impact_x = targets[bestChoice[1]].target_x;
    double impact_y = targets[bestChoice[1]].target_y;
    double release_x = impact_x - wind_x * timeOfFall(altitude_m);
    double release_y = impact_y - wind_y * timeOfFall(altitude_m);
    double price = ammo[bestChoice[0]].price;
    double value_sum = best + price;

    std::ofstream out(output_file);
    if (!out) {
        std::cerr << "Cannot open output file: " << output_file << '\n';
        return 1;
    }

    out << std::fixed << std::setprecision(2) << impact_x << ' ' << std::fixed << std::setprecision(2) << impact_y << '\n'
        << std::fixed << std::setprecision(2) << release_x << ' ' << std::fixed << std::setprecision(2) << release_y << '\n'
        << std::fixed << std::setprecision(2) << ammo[bestChoice[0]].type << '\n'
        << destroyed << ' ' << value_sum << ' ' << price << ' ' << best;

    return 0;
}
