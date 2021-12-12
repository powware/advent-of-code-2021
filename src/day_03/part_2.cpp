#include <fstream>
#include <filesystem>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>
#include <map>
#include <tuple>
#include <functional>
#include <string>

std::string FindRating(std::vector<std::tuple<std::vector<std::string>, std::vector<std::string>>> &candidates, std::function<bool(size_t, size_t)> comparison_operator, size_t bit_value)
{
    size_t relevant_bit_value = bit_value;
    for (size_t i = 0;; ++i)
    {
        candidates.push_back(std::tuple<std::vector<std::string>, std::vector<std::string>>());
        const auto &relevant_candidates = relevant_bit_value ? std::get<1>(candidates[i]) : std::get<0>(candidates[i]);
        if (relevant_candidates.size() == 1)
        {
            return relevant_candidates.back();
        }

        std::for_each(relevant_candidates.cbegin(), relevant_candidates.cend(), [&candidates, i](const std::string &bit_field)
                      {
                          if (bit_field[i + 1] == '1')
                          {
                              std::get<1>(candidates[i + 1]).push_back(bit_field);
                          }
                          else
                          {
                              std::get<0>(candidates[i + 1]).push_back(bit_field);
                          }
                      });

        relevant_bit_value = comparison_operator(std::get<1>(candidates[i + 1]).size(), std::get<0>(candidates[i + 1]).size());
    }
}

int main()
{
    const auto path = std::filesystem::current_path() / "input/day_03.txt";
    std::ifstream input{path};

    std::vector<std::tuple<std::vector<std::string>, std::vector<std::string>>> oxygen_generator_rating_candidates;
    std::vector<std::tuple<std::vector<std::string>, std::vector<std::string>>> co2_scrubber_rating_candidates;

    oxygen_generator_rating_candidates.push_back(std::tuple<std::vector<std::string>, std::vector<std::string>>());

    std::for_each(std::istream_iterator<std::string>(input), std::istream_iterator<std::string>(), [&oxygen_generator_rating_candidates](const std::string &bit_field)
                  {
                      if (bit_field[0] == '1')
                      {
                          std::get<1>(oxygen_generator_rating_candidates[0]).push_back(bit_field);
                      }
                      else
                      {
                          std::get<0>(oxygen_generator_rating_candidates[0]).push_back(bit_field);
                      }
                  });

    co2_scrubber_rating_candidates = oxygen_generator_rating_candidates;

    size_t oxygen_candidate_list_index = std::get<1>(oxygen_generator_rating_candidates[0]).size() >= std::get<0>(oxygen_generator_rating_candidates[0]).size();

    std::string oxygen_generator_rating = FindRating(
        oxygen_generator_rating_candidates, [](size_t lhs, size_t rhs)
        { return lhs >= rhs; },
        oxygen_candidate_list_index);

    std::string co2_scrubber_rating = FindRating(
        co2_scrubber_rating_candidates, [](size_t lhs, size_t rhs)
        { return lhs < rhs; },
        ~oxygen_candidate_list_index & 1);

    std::cout << std::stoi(oxygen_generator_rating, nullptr, 2) * std::stoi(co2_scrubber_rating, nullptr, 2) << std::endl;

    return 0;
}