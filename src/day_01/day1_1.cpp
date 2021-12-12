#include <fstream>
#include <filesystem>
#include <iterator>
#include <algorithm>
#include <iostream>

int main()
{
    const auto path = std::filesystem::current_path() / "input/day_01.txt";
    std::ifstream input(path);

    int previous = -1;
    int number_of_increases = 0;
    std::for_each(std::istream_iterator<int>(input), std::istream_iterator<int>(), [&previous, &number_of_increases](int current)
                  {
                      if (previous != -1 && current > previous)
                      {
                          ++number_of_increases;
                      }

                      previous = current;
                  });

    std::cout << number_of_increases << std::endl;

    return 0;
}