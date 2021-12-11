#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <stack>
#include <variant>

char GetClosingToken(char opening)
{
    switch (opening)
    {
    case '(':
    {
        return ')';
    }
    break;
    case '[':
    {
        return ']';
    }
    break;
    case '{':
    {
        return '}';
    }
    break;
    case '<':
    {
        return '>';
    }
    break;
    }

    return 0;
}

struct NonOpeningToken
{
    char found;
};

struct UnexpectedToken
{
    char found;
    char expected;
};

struct MissingToken
{
    char missing;
    uint64_t score;
};

std::variant<std::monostate, NonOpeningToken, UnexpectedToken, MissingToken> CheckSyntax(std::string_view line)
{
    std::stack<char> scopes;
    for (auto token : line)
    {
        std::cout << token;
        if (token == '(' || token == '[' || token == '{' || token == '<')
        {
            scopes.push(GetClosingToken(token));
        }
        else if (token == ')' || token == ']' || token == '}' || token == '>')
        {
            if (scopes.size() == 0)
            {
                std::cout << std::endl;
                return NonOpeningToken(token);
            }

            const char closing = scopes.top();
            if (token != closing)
            {
                std::cout << std::endl;
                return UnexpectedToken(token, closing);
            }

            scopes.pop();
        }
        else
        {
            std::cout << std::endl;
            throw std::runtime_error("Unknown Token");
        }
    }

    std::cout << std::endl;
    if (scopes.size())
    {
        const char missing = scopes.top();
        uint64_t score = 0;
        do
        {
            const char closing = scopes.top();
            std::cout << closing;
            // ): 1 point.
            // ]: 2 points.
            // }: 3 points.
            // >: 4 points.
            score *= 5;
            switch (closing)
            {
            case ')':
            {
                score += 1;
            }
            break;
            case ']':
            {
                score += 2;
            }
            break;
            case '}':
            {
                score += 3;
            }
            break;
            case '>':
            {
                score += 4;
            }
            break;
            }

            scopes.pop();
        } while (scopes.size());

        std::cout << std::endl;
        return MissingToken(missing, score);
    }

    return {};
}

int main()
{
    const auto path = std::filesystem::current_path() / "input/day10.txt";
    std::ifstream input{path};

    std::vector<uint64_t> scores;
    for (std::string line(1024, '\0'); input.getline(line.data(), line.size());)
    {
        auto result = CheckSyntax(line.c_str());
        switch (result.index())
        {
        case 1:
        {
            std::cout << "NonOpeningToken: " << std::get<NonOpeningToken>(result).found << std::endl;
        }
        break;
        case 2:
        {
            std::cout << "UnexpectedToken: " << std::get<UnexpectedToken>(result).found << " expected: " << std::get<UnexpectedToken>(result).expected << std::endl;
        }
        break;
        case 3:
        {
            scores.push_back(std::get<MissingToken>(result).score);
            std::cout << "MissingToken: " << std::get<MissingToken>(result).missing << std::endl;
        }
        break;
        }
    }

    std::sort(scores.begin(), scores.end(), [](uint64_t lhs, uint64_t rhs)
              { return lhs < rhs; });

    std::cout << scores[(scores.size() - 1) / 2] << std::endl;

    return 0;
}