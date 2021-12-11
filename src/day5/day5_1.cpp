#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <cstdio>
#include <map>

struct Point
{
    int x;
    int y;

    auto operator<=>(const Point &rhs) const noexcept = default;
};

void MarkVent(std::map<Point, int> &field, Point &start, int &p1, int p2)
{
    ++field[start];

    int difference = p2 - p1;
    const int step = difference > 0 ? 1 : -1;
    while (difference)
    {
        p1 += step;

        ++field[start];

        difference -= step;
    }
}

int main()
{
    const auto path = std::filesystem::current_path() / "input/day5.txt";
    std::ifstream input{path};

    std::map<Point, int> field;
    for (std::string line(1024, '\0'); input.getline(line.data(), line.size());)
    {
        Point start, end;
        std::sscanf(line.c_str(), "%d,%d -> %d,%d", &start.x, &start.y, &end.x, &end.y);

        if (start.y == end.y)
        {
            MarkVent(field, start, start.x, end.x);
        }
        else if (start.x == end.x)
        {
            MarkVent(field, start, start.y, end.y);
        }
    }

    unsigned int count = 0;
    for (auto [point, value] : field)
    {
        if (value >= 2)
        {
            ++count;
        }
    }

    std::cout << count << std::endl;

    std::ofstream out("day5_1.txt");
    for (int y = 0; y < 1000; y++)
    {
        for (int x = 0; x < 1000; x++)
        {
            out << field[{x, y}];
        }
        out << "\n";
    }

    return 0;
}