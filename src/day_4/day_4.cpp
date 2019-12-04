/*
 * Note to self: add more comments!
 */

#include <string>
#include <vector>

// lower and upper bounds, this is our puzzle input
const uint32_t lower = 271973;
const uint32_t upper = 785961;

// this function checks if a password meets the old criteria
uint32_t meets_criteria(const std::string &pw)
{
    bool adjacent_same = false;

    for (size_t i = 1; i < 6; i++)
    {
        if (pw[i - 1u] > pw[i])
        {
            return 0;
        }

        if (pw[i - 1u] == pw[i])
        {
            adjacent_same = true;
        }
    }

    return (uint32_t)adjacent_same;
}

// okay, I think you now know what this one does.
uint32_t meets_new_criteria(const std::string &pw)
{
    std::vector<uint32_t> count(10, 0);

    for (size_t i = 1; i < 6; i++)
    {
        if (pw[i - 1u] > pw[i])
        {
            return 0;
        }
    }

    for (size_t i = 0; i < 6; i++)
    {
        ++count[pw[i] - '0'];
    }

    for (auto &i : count)
    {
        if (i == 2)
        {
            return 1;
        }
    }

    return 0;
}

int main()
{
    uint32_t valid_pw = 0;
    uint32_t valid_pw_new = 0;

    for (uint32_t i = lower; i <= upper; i++)
    {
        std::string pw = std::to_string(i);

        valid_pw += meets_criteria(pw);
        valid_pw_new += meets_new_criteria(pw);
    }

    // ...print out the results!
    printf("Number of valid passwords: %u\n", valid_pw);
    printf("Numbers of valid passwords that meet the new criteria: %u\n", valid_pw_new);
}
