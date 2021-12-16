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

    Grid<uint8_t> original_risks;
    for (char risk; input.get(risk);)
    {
        if (risk != '\n')
        {
            original_risks.push_back(risk - '0');
        }
    }

    original_risks.CalculateSideLength();
    Grid<int8_t> risks;
    risks.resize(25 * original_risks.size());
    risks.CalculateSideLength();
    for (size_t y = 0; y < *risks.side_length; ++y)
    {
        for (size_t x = 0; x < *risks.side_length; ++x)
        {
            auto extended_risk = original_risks[{x % (*original_risks.side_length), y % (*original_risks.side_length)}];
            extended_risk += x / (*original_risks.side_length);
            extended_risk += y / (*original_risks.side_length);
            risks[{x, y}] = extended_risk > 9 ? extended_risk - 9 : extended_risk;
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