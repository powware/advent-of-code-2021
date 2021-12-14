#include <fstream>
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <tuple>

int main()
{
    const auto path = std::filesystem::current_path() / "input/day_14.txt";
    std::ifstream input{path};

    std::map<char, size_t> occurences;
    std::map<std::pair<char, char>, std::optional<char>> transitions;
    std::string polymer_string;
    input >> polymer_string;
    for (std::string line(1024, '\0'); input.getline(line.data(), line.size());)
    {
        if (std::string(line.c_str()).find('-') == std::string::npos)
        {
            continue;
        }

        std::pair<char, char> pair;
        char insertion;
        std::sscanf(line.c_str(), "%c%c -> %c", &std::get<0>(pair), &std::get<1>(pair), &insertion);

        transitions[pair] = insertion;
    }

    std::map<std::pair<char, char>, size_t> polymer;
    for (size_t i = 0; i < polymer_string.size(); i++)
    {
        ++occurences[polymer_string[i]];
        if (i < polymer_string.size() - 1)
        {
            ++polymer[{polymer_string[i], polymer_string[i + 1]}];
        }
    }

    for (size_t step = 0; step < 40; ++step)
    {
        std::map<std::pair<char, char>, size_t> new_polymer;
        for (auto [pair, count] : polymer)
        {
            if (auto insertion = transitions[pair])
            {
                new_polymer[{std::get<0>(pair), *insertion}] += count;
                new_polymer[{*insertion, std::get<1>(pair)}] += count;

                occurences[*insertion] += count;
            }
            else
            {
                new_polymer[pair] = polymer[pair];
            }
        }

        polymer = std::move(new_polymer);
    }

    size_t min = -1, max = 0;
    for (auto [_, occurence] : occurences)
    {
        if (occurence > max)
        {
            max = occurence;
        }
        if (occurence < min)
        {
            min = occurence;
        }
    }

    std::cout << (max - min) << std::endl;

    return 0;
}