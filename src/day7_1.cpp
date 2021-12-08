#include <fstream>
#include <filesystem>
#include <iostream>
#include <map>
#include <numeric>

int main()
{
    const auto path = std::filesystem::current_path() / "input/day7.txt";
    std::ifstream input{path};

    std::map<unsigned int, unsigned int> distances;
    unsigned int min_distance = -1, max_distance = 0;
    for (std::string line(10, '\0'); input.getline(line.data(), line.size(), ',');)
    {
        const unsigned int distance = std::stoul(line);
        if (distance > max_distance)
        {
            max_distance = distance;
        }

        if (distance < min_distance)
        {
            min_distance = distance;
        }

        ++distances[distance];
    }

    unsigned int min_fuel = -1;
    for (unsigned int i = min_distance; i <= max_distance; ++i)
    {
        unsigned int fuel = 0;
        for (auto [distance, occurances] : distances)
        {
            fuel += unsigned(std::abs(int(distance) - int(i))) * occurances;
        }

        if (fuel < min_fuel)
        {
            min_fuel = fuel;
        }
    }

    std::cout << min_fuel << std::endl;

    return 0;
}