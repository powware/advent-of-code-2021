#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string_view>

struct Octopus
{
    unsigned int energy_level;
    bool flashed;
};

void IncrementOctopus(std::vector<Octopus> &octos, uint64_t &flashes, size_t x, size_t y)
{
    size_t index = y * 10 + x;
    auto &octopus = octos[index];
    if (octopus.flashed || ++octopus.energy_level <= 9)
    {
        return;
    }

    octopus.flashed = true;
    octopus.energy_level = 0;

    ++flashes;

    const bool up = y > 0;
    const bool down = y < 9;
    const bool left = x > 0;
    const bool right = x < 9;
    if (up)
    {
        IncrementOctopus(octos, flashes, x, y - 1);
    }
    if (up && right)
    {
        IncrementOctopus(octos, flashes, x + 1, y - 1);
    }
    if (right)
    {
        IncrementOctopus(octos, flashes, x + 1, y);
    }
    if (right && down)
    {
        IncrementOctopus(octos, flashes, x + 1, y + 1);
    }
    if (down)
    {
        IncrementOctopus(octos, flashes, x, y + 1);
    }
    if (down && left)
    {
        IncrementOctopus(octos, flashes, x - 1, y + 1);
    }
    if (left)
    {
        IncrementOctopus(octos, flashes, x - 1, y);
    }
    if (left && up)
    {
        IncrementOctopus(octos, flashes, x - 1, y - 1);
    }
}

int main()
{
    const auto path = std::filesystem::current_path() / "input/day_11.txt";
    std::ifstream input{path};

    std::vector<Octopus> octos;
    for (std::string line(1024, '\0'); input.getline(line.data(), line.size());)
    {
        for (auto number : std::string_view(line.c_str()))
        {
            octos.emplace_back(number - '0', false);
        }
    }

    uint64_t flashes = 0;
    for (size_t i = 0; i < 100; ++i)
    {
        for (size_t y = 0; y < 10; ++y)
        {
            for (size_t x = 0; x < 10; ++x)
            {
                IncrementOctopus(octos, flashes, x, y);
            }
        }

        std::for_each(octos.begin(), octos.end(), [](Octopus &octopus)
                      { octopus.flashed = false; });
    }

    std::cout << flashes << std::endl;

    return 0;
}