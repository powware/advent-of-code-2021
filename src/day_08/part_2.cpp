#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <numeric>
#include <iterator>
#include <cassert>

void MarkSegments(std::array<std::list<char>, 7> &segments, const std::vector<size_t> &marked_segments, const std::string &digit)
{
    auto marked_segment = marked_segments.cbegin();
    for (size_t i = 0; i < segments.size(); ++i)
    {
        bool marked = false;
        if (i == *marked_segment)
        {
            marked = true;
            ++marked_segment;
        }

        auto &segment = segments[i];
        segment.erase(std::remove_if(segment.begin(), segment.end(), [&digit, marked](const char &character)
                                     {
                                         auto found = digit.find(character);
                                         return marked ? found == std::string::npos : found != std::string::npos;
                                     }),
                      segment.end());
    }
}

void RemoveDuplicateOfUniqueSegments(std::array<std::list<char>, 7> &segments)
{
    for (size_t j = 0; j < segments.size(); ++j)
    {
        if (segments[j].size() == 1)
        {
            for (size_t k = 0; k < segments.size(); ++k)
            {
                if (j == k)
                {
                    continue;
                }

                auto &segment = segments[k];
                char marked = segments[j].back();
                segment.erase(std::remove_if(segment.begin(), segment.end(), [marked](const char &character)
                                             { return character == marked; }),
                              segment.end());
            }
        }
    }
}

void MarkCommonSegments(std::array<std::list<char>, 7> &segments, const std::vector<size_t> &marked_segments, const std::string &digit)
{
    auto marked_segment = marked_segments.cbegin();
    for (size_t i = 0; i < segments.size(); ++i)
    {
        if (i != *marked_segment)
        {
            continue;
        }

        auto &segment = segments[i];
        segment.erase(std::remove_if(segment.begin(), segment.end(), [&digit](const char &character)
                                     { return digit.find(character) == std::string::npos; }),
                      segment.end());
        ++marked_segment;
    }

    RemoveDuplicateOfUniqueSegments(segments);
}

std::string DecodeDigit(const std::array<std::list<char>, 7> &segments, std::string digit)
{
    const auto segment_count = digit.size();
    switch (segment_count)
    {
    case 2: // 1
    {
        return "1";
    }
    break;
    case 3: // 7
    {
        return "7";
    }
    break;
    case 4: // 4
    {
        return "4";
    }
    break;
    case 5: // 2, 3, 5
    {
        if (digit.find(segments[5].back()) == std::string::npos)
        {
            return "2";
        }
        else if (digit.find(segments[2].back()) == std::string::npos)
        {
            return "5";
        }
        else
        {
            return "3";
        }
    }
    break;
    case 6: // 0, 6, 9
    {
        if (digit.find(segments[2].back()) == std::string::npos)
        {
            return "6";
        }
        else if (digit.find(segments[4].back()) == std::string::npos)
        {
            return "9";
        }
        else
        {
            return "0";
        }
    }
    break;
    case 7: // 8
    {
        return "8";
    }
    break;
    default:
    {
        std::terminate();
    }
    break;
    }
}

int main()
{
    const auto path = std::filesystem::current_path() / "input/day_08.txt";
    std::ifstream input{path};

    std::vector<std::vector<std::string>> inputs;
    std::vector<std::vector<std::string>> outputs;
    for (std::string line(2048, '\0'); input.getline(line.data(), line.size());)
    {
        inputs.emplace_back();
        outputs.emplace_back();

        std::istringstream isstream(line.c_str());
        bool delimiter = false;
        std::for_each(std::istream_iterator<std::string>(isstream), std::istream_iterator<std::string>(), [&inputs, &outputs, &delimiter](const std::string &digit)
                      {
                          if (digit == "|")
                          {
                              delimiter = true;
                              return;
                          }

                          if (!delimiter)
                          {
                              inputs.back().push_back(digit);
                          }
                          else
                          {
                              outputs.back().push_back(digit);
                          }
                      });
    }

    unsigned int sum = 0;
    for (size_t i = 0; i < outputs.size(); ++i)
    {
        //   000
        //  1   2
        //   333
        //  4   5
        //   666
        std::array<std::list<char>, 7> segments;
        for (auto &segment : segments)
        {
            segment = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
        }

        auto combined = inputs[i];
        combined.insert(combined.cend(), outputs[i].cbegin(), outputs[i].cend());

        for (auto &digit : combined)
        {
            const auto segment_count = digit.size();
            if (!(segment_count == 2 || segment_count == 3 || segment_count == 4))
            {
                continue;
            }

            switch (segment_count)
            {
            case 2: // 1
            {
                MarkSegments(segments, {2, 5}, digit);
            }
            break;
            case 4: // 4
            {
                MarkSegments(segments, {1, 2, 3, 5}, digit);
            }
            break;
            case 3: // 7
            {
                MarkSegments(segments, {0, 2, 5}, digit);
            }
            break;
            }
        }

        for (auto &digit : combined)
        {
            const auto segment_count = digit.size();
            if (segment_count != 5)
            {
                continue;
            }

            MarkCommonSegments(segments, {0, 3, 6}, digit); // digit 2, 3, 5 have common segment 3, isolated digit 0's segments
        }

        for (auto &digit : combined)
        {
            const auto segment_count = digit.size();
            if (!(segment_count == 5 && (segments[1].size() == 1 && digit.find(segments[1].back()) != std::string::npos))) // digit 5
            {
                continue;
            }

            MarkSegments(segments, {0, 1, 3, 5, 6}, digit); // digit 5 can distinguish between segment 2 and 5
        }

        std::string number;
        for (auto &digit : outputs[i])
        {
            number += DecodeDigit(segments, digit);
        }
        sum += std::stoul(number);
    }

    std::cout << sum << std::endl;

    return 0;
}