#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <numeric>
#include <iterator>

int main()
{
    const auto path = std::filesystem::current_path() / "input/day8.txt";
    std::ifstream input{path};

    std::vector<std::vector<std::string>> inputs;
    std::vector<std::vector<std::string>> outputs;
    for (std::string line(2048, '\0'); input.getline(line.data(), line.size());)
    {
        inputs.emplace_back();
        outputs.emplace_back();

        std::istringstream isstream(line.c_str());
        bool delimiter = false;
        std::for_each(std::istream_iterator<std::string>(isstream), std::istream_iterator<std::string>(), [&inputs, &outputs, &delimiter](const std::string &number)
                      {
                          if (number == "|")
                          {
                              delimiter = true;
                              return;
                          }

                          if (!delimiter)
                          {
                              inputs.back().push_back(number);
                          }
                          else
                          {
                              outputs.back().push_back(number);
                          }
                      });
    }

    unsigned int unique_segmented_number_count = 0; // 1, 4, 7, 8

    for (auto &output : outputs)
    {
        for (auto number : output)
        {
            const auto segment_count = number.size();
            if (segment_count == 2 || segment_count == 4 || segment_count == 3 || segment_count == 7)
            {
                ++unique_segmented_number_count;
                std::cout << number << std::endl;
            }
        }
    }

    std::cout << unique_segmented_number_count << std::endl;

    return 0;
}