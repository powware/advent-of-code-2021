#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <functional>
#include <optional>

struct Point
{
    size_t x;
    size_t y;

    auto operator<=>(const Point &rhs) const noexcept = default;
};

template <typename Type>
struct Grid : std::vector<Type>
{
    std::optional<size_t> side_length;

    void CalculateSideLength()
    {
        side_length = static_cast<size_t>(std::sqrt(std::vector<Type>::size()));
    }

    auto &operator[](const Point &point)
    {
        if (!side_length)
        {
            CalculateSideLength();
        }
        return std::vector<Type>::at(point.y * (*side_length) + point.x);
    }
};

struct Distance
{
    std::optional<unsigned int> distance;
    bool visited;
};

struct PointDistance
{
    Point point;
    unsigned int distance;

    friend constexpr auto operator<=>(const PointDistance &lhs, const PointDistance &rhs)
    {
        return lhs.distance <=> rhs.distance;
    }
};

int main()
{
    const auto path = std::filesystem::current_path() / "input/day_15.txt";
    std::ifstream input{path};

    Grid<uint8_t> risks;
    for (char risk; input.get(risk);)
    {
        if (risk != '\n')
        {
            risks.push_back(risk - '0');
        }
    }

    Grid<Distance> distances;
    distances.resize(risks.size(), Distance(std::nullopt, false));
    distances.CalculateSideLength();

    std::priority_queue<PointDistance, std::vector<PointDistance>, std::greater<PointDistance>> unvisited;
    unvisited.push({{0, 0}, 0});
    const Point finish(*distances.side_length - 1, *distances.side_length - 1);
    while (unvisited.size() && unvisited.top().point != finish)
    {
        auto current = unvisited.top();
        unvisited.pop();

        if (distances[current.point].visited)
        {
            continue;
        }

        distances[current.point].visited = true;
        distances[current.point].distance = current.distance;

        auto add_unvisited = [&unvisited, &distances, &risks, current_distance = current.distance](Point neighbor)
        {
            if (!distances[neighbor].visited)
            {
                auto distance_through_current = current_distance + risks[neighbor];
                if (!distances[neighbor].distance || distance_through_current < distances[neighbor].distance)
                {
                    distances[neighbor].distance = distance_through_current;
                    unvisited.push({neighbor, distance_through_current});
                }
            }
        };

        if (current.point.x > 0)
        {
            add_unvisited({current.point.x - 1, current.point.y});
        }
        if (current.point.x < *risks.side_length - 1)
        {
            add_unvisited({current.point.x + 1, current.point.y});
        }
        if (current.point.y > 0)
        {
            add_unvisited({current.point.x, current.point.y - 1});
        }
        if (current.point.y < *risks.side_length - 1)
        {
            add_unvisited({current.point.x, current.point.y + 1});
        }
    }

    std::cout << *distances[finish].distance << std::endl;

    return 0;
}