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

    void Normalize() // works specific to this puzzle
    {
        if (x)
        {
            x /= std::abs(x);
        }

        if (y)
        {
            y /= std::abs(y);
        }
    }

    auto operator+(const Point &rhs)
    {
        return Point(x + rhs.x, y + rhs.y);
    }

    auto operator-(const Point &rhs)
    {
        return Point(x - rhs.x, y - rhs.y);
    }

    auto &operator+=(const Point &rhs)
    {
        *this = *this + rhs;

        return *this;
    }

    auto &operator-=(const Point &rhs)
    {
        *this = *this - rhs;

        return *this;
    }

    auto operator<=>(const Point &rhs) const noexcept = default;

    operator bool()
    {
        return x || y;
    }
};

void MarkVent(std::map<Point, int> &field, Point start, Point end)
{
    ++field[start];

    auto difference = end - start;
    auto step = difference;
    step.Normalize();
    while (difference)
    {
        start += step;

        ++field[start];

        difference -= step;
    }
}

int main()
{
    const auto path = std::filesystem::current_path() / "input/day_05.txt";
    std::ifstream input{path};

    std::map<Point, int> field;
    for (std::string line(1024, '\0'); input.getline(line.data(), line.size());)
    {
        Point start, end;
        std::sscanf(line.c_str(), "%d,%d -> %d,%d", &start.x, &start.y, &end.x, &end.y);

        MarkVent(field, start, end);
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

    std::ofstream out("day5_2.txt");
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