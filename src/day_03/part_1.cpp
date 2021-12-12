#include <fstream>
#include <filesystem>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>
#include <map>

int main()
{
    const auto path = std::filesystem::current_path() / "input/day_03.txt";
    std::ifstream input{path};

    size_t bit_count = 0;
    unsigned int total = 0;
    std::map<size_t, unsigned int> counts_per_bit;
    std::for_each(std::istream_iterator<std::string>(input), std::istream_iterator<std::string>(), [&bit_count, &total, &counts_per_bit](const std::string &bit_field)
                  {
                      if (bit_count == 0)
                      {
                          bit_count = bit_field.size();
                      }

                      for (size_t i = 0; i < bit_count; ++i)
                      {
                          if (bit_field[bit_count - 1 - i] == '1')
                          {
                              ++counts_per_bit[i];
                          }
                      }

                      ++total;
                  });

    unsigned int gamma_rate = 0;
    for (size_t i = 0; i < bit_count; ++i)
    {
        gamma_rate |= (counts_per_bit[i] >= total / 2) << i;
    }

    const unsigned int epsilon_rate = ~gamma_rate & ((1 << bit_count) - 1);

    std::cout << gamma_rate * epsilon_rate << std::endl;

    return 0;
}