#include <fstream>
#include <filesystem>
#include <iostream>
#include <array>
#include <numeric>

int main()
{
    const auto path = std::filesystem::current_path() / "input/day_06.txt";
    std::ifstream input{path};

    std::array<uint64_t, 9> timers = {0};
    for (std::string line(2, '\0'); input.getline(line.data(), line.size(), ',');)
    {
        ++timers[std::stoul(line)];
    }

    for (size_t day = 0; day < 80; ++day)
    {
        std::array<uint64_t, 9> next_timers;
        std::cout << timers[0] << " ";
        for (size_t i = 0; i < 8; ++i)
        {
            std::cout << timers[i + 1] << " ";
            next_timers[i] = timers[i + 1];
        }
        std::cout << "\n";

        next_timers[6] += timers[0];
        next_timers[8] = timers[0];

        timers = std::move(next_timers);
    }

    std::cout << std::accumulate(timers.cbegin(), timers.cend(), uint64_t(0)) << std::endl;

    return 0;
}