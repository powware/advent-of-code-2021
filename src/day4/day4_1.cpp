#include <fstream>
#include <filesystem>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>
#include <tuple>
#include <functional>
#include <string>
#include <optional>

struct BingoBoard
{
    static constexpr size_t kWidth{5};
    static constexpr size_t kHeight{5};

    std::array<int, kWidth * kHeight> entries_;

    std::optional<int> Mark(int number)
    {
        for (auto &entry : entries_)
        {
            if (entry == number)
            {
                entry = -1;
            }
        }

        if (Validate())
        {
            int sum = 0;
            for (auto &entry : entries_)
            {
                if (entry != -1)
                {
                    sum += entry;
                }
            }

            return sum * number;
        }

        return std::nullopt;
    }

    bool Validate()
    {
        for (size_t x = 0; x < kWidth; ++x)
        {
            if (ValidateColumn(x))
            {
                return true;
            }
        }

        for (size_t y = 0; y < kHeight; ++y)
        {
            if (ValidateRow(y))
            {
                return true;
            }
        }

        return false;
    }

    bool ValidateRow(size_t row)
    {
        size_t start = row * kWidth;
        return std::accumulate(&entries_[start], &entries_[start] + kWidth, 0) == -5;
    }

    bool ValidateColumn(size_t column)
    {
        int sum = 0;
        for (size_t y = 0; y < kWidth; ++y)
        {
            sum += entries_[column + y * kWidth];
        }

        return sum == -5;
    }

    inline friend auto &
    operator>>(std::istream &is, BingoBoard &rhs)
    {
        for (auto &entry : rhs.entries_)
        {
            is >> entry;
        }

        return is;
    }
};

int main()
{
    const auto path = std::filesystem::current_path() / "input/day4.txt";
    std::ifstream input{path};

    std::string first_line(1024, '\0');
    input.getline(first_line.data(), first_line.size());

    std::vector<int> drawn_numbers;
    size_t start = 0;
    size_t end = 0;
    do
    {
        end = first_line.find(',', start);
        std::string drawn_number = first_line.substr(start, end - start);
        drawn_numbers.push_back(std::stoi(drawn_number, nullptr));
        start = end + 1;
    } while (end != std::string::npos);

    std::vector<BingoBoard> boards;
    std::for_each(std::istream_iterator<BingoBoard>(input), std::istream_iterator<BingoBoard>(), [&boards](const BingoBoard &board)
                  { boards.push_back(board); });

    while (true)
    {
        for (auto drawn_number : drawn_numbers)
        {
            for (auto &board : boards)
            {
                if (auto solution = board.Mark(drawn_number))
                {
                    std::cout << *solution << std::endl;
                    return 0;
                }
            }
        }
    }

    return 1;
}