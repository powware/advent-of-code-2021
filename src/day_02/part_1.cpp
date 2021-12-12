#include <fstream>
#include <filesystem>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>

struct Command
{
    std::string direction;
    unsigned int units;

    inline friend auto &operator>>(std::istream &is, Command &rhs)
    {
        return is >> rhs.direction >> rhs.units;
    }
};

struct Position
{
    unsigned int hoizontal;
    unsigned int vertical;
};

int main()
{
    const auto path = std::filesystem::current_path() / "input/day_02.txt";
    std::ifstream input{path};

    Position position{0, 0};
    std::for_each(std::istream_iterator<Command>(input), std::istream_iterator<Command>(), [&position](const Command &command)
                  {
                      if (command.direction == "forward")
                      {
                          position.hoizontal += command.units;
                      }
                      else if (command.direction == "up")
                      {
                          position.vertical -= command.units;
                      }
                      else if (command.direction == "down")
                      {
                          position.vertical += command.units;
                      }
                  });

    std::cout << (position.hoizontal * position.vertical) << std::endl;

    return 0;
}