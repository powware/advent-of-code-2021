#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <cmath>

int main()
{
    const auto path = std::filesystem::current_path() / "input/day9.txt";
    std::ifstream input{path};

    std::vector<uint8_t> heightmap;
    for (char height; input.get(height);)
    {
        if (height != '\n')
        {
            heightmap.push_back(height - '0');
        }
    }

    std::array<unsigned int, 3> basins;
    unsigned int sum = 0;
    const size_t side_length = static_cast<size_t>(std::sqrt(heightmap.size()));
    for (size_t y = 0; y < side_length; ++y)
    {
        for (size_t x = 0; x < side_length; ++x)
        {
            uint8_t height = heightmap[y * side_length + x];
            bool low_point = true;
            if (y > 0 && heightmap[(y - 1) * side_length + x] <= height)
            {
                low_point = false;
            }
            if (x > 0 && heightmap[y * side_length + x - 1] <= height)
            {
                low_point = false;
            }
            if (x < (side_length - 1) && heightmap[y * side_length + x + 1] <= height)
            {
                low_point = false;
            }
            if (y < (side_length - 1) && heightmap[(y + 1) * side_length + x] <= height)
            {
                low_point = false;
            }

            if (low_point)
            {
                sum += height + 1;
            }
        }
    }

    std::cout << sum << std::endl;

    return 0;
}