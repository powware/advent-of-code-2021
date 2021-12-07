#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <cstdio>

int main()
{
    const auto path = std::filesystem::current_path() / "input/day5.txt";
    std::ifstream input{path};

    for (std::string line(1024, '\0'); input.getline(line.data(), line.size());)
    {
        unsigned int x1, y1, x2, y2;
        std::sscanf(line.c_str(), "%u,%u -> %u,%u", &x1, &y1, &x2, &y2);

        std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
    }

    return 0;
}