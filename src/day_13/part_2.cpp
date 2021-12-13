#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <optional>
#include <map>
#include <set>
#include <chrono>

struct Dot
{
    unsigned int x;
    unsigned int y;

    inline friend auto operator<=>(const Dot &lhs, const Dot &rhs) = default;
};

using Paper = std::set<Dot>;

struct Fold
{
    enum
    {
        X,
        Y
    } type;

    unsigned int position;
};

void PrintPaper(const Paper &paper, std::optional<Fold> fold = std::nullopt)
{
    Dot lower_right(0, 0);
    for (auto dot : paper)
    {
        if (dot.x > lower_right.x)
        {
            lower_right.x = dot.x;
        }

        if (dot.y > lower_right.y)
        {
            lower_right.y = dot.y;
        }
    }

    for (unsigned int y = 0; y <= lower_right.y; ++y)
    {
        for (unsigned int x = 0; x <= lower_right.x; ++x)
        {
            if (fold && (fold->type == Fold::X ? x : y) == fold->position)
            {
                std::cout << (fold->type == Fold::X ? '|' : '-');
            }
            else
            {
                std::cout << (paper.contains({x, y}) ? '#' : '.');
            }
        }
        std::cout << std::endl;
    }
}

int main()
{
    const auto path = std::filesystem::current_path() / "input/day_13.txt";
    std::ifstream input{path};

    Paper paper;
    std::vector<Fold> folds;
    for (std::string line(1024, '\0'); input.getline(line.data(), line.size());)
    {
        std::string line_content(line.c_str());
        if (line_content.find(',') != std::string::npos)
        {
            Dot dot;
            std::sscanf(line.c_str(), "%u,%u", &dot.x, &dot.y);
            paper.insert(dot);
        }
        else if (line_content.find("fold") != std::string::npos)
        {
            Fold fold;
            if (line_content.find('x') != std::string::npos)
            {
                fold.type = Fold::X;
            }
            else
            {
                fold.type = Fold::Y;
            }

            fold.position = std::stoul(line_content.substr(line_content.find('=') + 1));

            folds.push_back(fold);
        }
    }

    auto start = std::chrono::steady_clock::now();
    for (auto fold : folds)
    {
        Paper folded_paper;
        for (auto dot : paper)
        {
            auto &affected_position = fold.type == Fold::X ? dot.x : dot.y;
            if (affected_position > fold.position)
            {
                affected_position = fold.position - (affected_position - fold.position);
            }
            folded_paper.insert(dot);
        }

        //PrintPaper(paper, fold);

        paper = std::move(folded_paper);
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us" << std::endl;

    PrintPaper(paper);

    return 0;
}