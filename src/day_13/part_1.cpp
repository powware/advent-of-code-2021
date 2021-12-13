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

    const auto fold = folds.front();

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

    paper = std::move(folded_paper);

    std::cout << paper.size() << std::endl;

    return 0;
}