#include <fstream>
#include <filesystem>
#include <iostream>
#include <map>
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
};

std::variant<std::monostate, NonOpeningToken, UnexpectedToken, MissingToken> CheckSyntax(std::string_view line)
{
    std::stack<char> scopes;
    for (auto token : line)
    {
        std::cout << token;
        if (token == '(' || token == '[' || token == '{' || token == '<')
        {
            scopes.push(token);
        }
        else if (token == ')' || token == ']' || token == '}' || token == '>')
        {
            if (scopes.size() == 0)
            {
                std::cout << std::endl;
                return NonOpeningToken(token);
            }

            char closing = GetClosingToken(scopes.top());

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
        return MissingToken(GetClosingToken(scopes.top()));
    }

    return {};
}

int main()
{
    const auto path = std::filesystem::current_path() / "input/day10.txt";
    std::ifstream input{path};

    std::map<char, unsigned int> unexpected_tokens;
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
            ++unexpected_tokens[std::get<UnexpectedToken>(result).found];
            std::cout << "UnexpectedToken: " << std::get<UnexpectedToken>(result).found << " expected: " << std::get<UnexpectedToken>(result).expected << std::endl;
        }
        break;
        case 3:
        {
            std::cout << "MissingToken: " << std::get<MissingToken>(result).missing << std::endl;
        }
        break;
        }
    }
    // ): 3 points.
    // ]: 57 points.
    // }: 1197 points.
    // >: 25137 points.
    uint64_t score = unexpected_tokens[')'] * 3ll + unexpected_tokens[']'] * 57ll + unexpected_tokens['}'] * 1197ll + unexpected_tokens['>'] * 25137ll;

    std::cout << score << std::endl;

    return 0;
}