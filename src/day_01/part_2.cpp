#include <fstream>
#include <filesystem>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>

int main()
{
    const auto path = std::filesystem::current_path() / "input/day_01.txt";
    std::ifstream input{path};

    int previous = -1;
    int number_of_increases = 0;
    std::vector<int> measurements{std::istream_iterator<int>(input), std::istream_iterator<int>()};
    for (auto iterator = measurements.cbegin(); iterator != measurements.cend(); ++iterator)
    {
        const int current = std::accumulate(iterator, iterator + 3, 0);
        if (previous != -1 && current > previous)
        {
            ++number_of_increases;
        }

        previous = current;
    }

    std::cout << number_of_increases << std::endl;

    return number_of_increases;
}