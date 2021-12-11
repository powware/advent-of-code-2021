#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>

enum class Direction
{
    none,
    above,
    left,
    right,
    below
};

unsigned int GetBasinSize(std::vector<uint8_t> &heightmap, size_t side_length, size_t location, Direction from = Direction::none)
{
    if (heightmap[location] >= 9)
    {
        return 0;
    }

    heightmap[location] = 'x';

    unsigned int size = 1;
    if (from != Direction::left && location % side_length != 0)
    {
        size += GetBasinSize(heightmap, side_length, location - 1, Direction::right);
    }
    if (from != Direction::right && location % side_length != side_length - 1)
    {
        size += GetBasinSize(heightmap, side_length, location + 1, Direction::left);
    }
    if (from != Direction::above && static_cast<int>(location) - static_cast<int>(side_length) > 0)
    {
        size += GetBasinSize(heightmap, side_length, location - side_length, Direction::below);
    }
    if (from != Direction::below && location + side_length < side_length * side_length - 1)
    {
        size += GetBasinSize(heightmap, side_length, location + side_length, Direction::above);
    }

    return size;
}

void print(std::ofstream &of, std::vector<uint8_t> &heightmap, size_t side_length)
{
    for (size_t y = 0; y < side_length; ++y)
    {
        for (size_t x = 0; x < side_length; ++x)
        {
            const size_t location = y * side_length + x;
            const auto height = heightmap[location];
            if (height == 'x')
            {
                of << 'x';
            }
            else
            {
                of << +height;
            }
        }
        of << std::endl;
    }
}

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

    std::array<unsigned int, 3> basins = {0, 0, 0};
    unsigned int sum = 0;
    const size_t side_length = static_cast<size_t>(std::sqrt(heightmap.size()));
    for (size_t y = 0; y < side_length; ++y)
    {
        for (size_t x = 0; x < side_length; ++x)
        {
            const size_t location = y * side_length + x;
            const auto height = heightmap[location];
            if (height >= 9)
            {
                continue;
            }

            bool low_point = true;
            if (x > 0 && heightmap[location - 1] <= height) // left
            {
                low_point = false;
            }
            if (x < (side_length - 1) && heightmap[location + 1] <= height) // right
            {
                low_point = false;
            }
            if (y > 0 && heightmap[location - side_length] <= height) // up
            {
                low_point = false;
            }
            if (y < (side_length - 1) && heightmap[location + side_length] <= height) // down
            {
                low_point = false;
            }

            if (low_point)
            {
                sum += height + 1;
                unsigned int current_basin = GetBasinSize(heightmap, side_length, y * side_length + x);
                //std::ofstream of("output/" + std::to_string(location) + ".txt");
                //print(of, heightmap, side_length);
                for (auto &basin : basins)
                {
                    if (current_basin > basin)
                    {
                        unsigned int temp = basin;
                        basin = current_basin;
                        current_basin = temp;
                    }
                }
            }
        }
    }

    size_t product = 1;
    for (auto basin : basins)
    {
        product *= basin;
        std::cout << basin << std::endl;
    }

    std::cout << product << std::endl;

    return 0;
}