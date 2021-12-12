#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <chrono>

struct Cave;
using ContainerCave = std::reference_wrapper<const Cave>;

struct Cave
{
    std::string name;
    bool big;
    std::vector<ContainerCave> neighbors;

    bool operator==(const Cave &rhs) const noexcept
    {
        return name == rhs.name;
    }
};

struct Path
{
    std::vector<ContainerCave> caves;
    bool small_cave_visited_twice = false;

    bool contains(const Cave &cave)
    {
        for (auto visited : caves)
        {
            if (visited.get() == cave)
            {
                return true;
            }
        }

        return false;
    }
};

void FindPaths(std::vector<Path> &paths, Path path, const Cave &cave, const Cave &end)
{

    path.caves.push_back(cave);
    if (cave == end)
    {
        paths.push_back(path);
        return;
    }

    for (auto &neighbor : cave.neighbors)
    {
        if (neighbor.get().big || !path.contains(neighbor))
        {
            FindPaths(paths, path, neighbor, end);
        }
    }
}

int main()
{
    auto start = std::chrono::steady_clock::now();
    const auto path = std::filesystem::current_path() / "input/day_12.txt";
    std::ifstream input{path};

    std::map<std::string, Cave> cavesystem;
    for (std::string line(1024, '\0'); input.getline(line.data(), line.size());)
    {
        auto connection = std::string(line.c_str());
        auto delimiter_position = connection.find('-');
        auto from_name = connection.substr(0, delimiter_position);
        auto to_name = connection.substr(delimiter_position + 1, connection.size() - delimiter_position);

        auto &from = cavesystem[from_name];
        auto &to = cavesystem[to_name];

        if (from.name.empty())
        {
            from.name = from_name;
            from.big = std::isupper(from.name[0]);
        }

        if (to.name.empty())
        {
            to.name = to_name;
            to.big = std::isupper(to.name[0]);
        }

        from.neighbors.push_back(std::cref(to));
        to.neighbors.push_back(std::cref(from));
    }

    std::vector<Path> paths;
    FindPaths(paths, {}, cavesystem["start"], cavesystem["end"]);
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    std::cout << paths.size() << std::endl;

    return 0;
}