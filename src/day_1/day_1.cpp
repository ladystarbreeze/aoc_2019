#include <fstream>
#include <string>

// small function to compute the required amount of fuel
inline int64_t get_fuel(const int64_t fuel)
{
    return fuel / 3 - 2;
}

// small function to recursively compute how much additional fuel is required
inline int64_t get_fuel_sum(const int64_t fuel)
{
    int64_t new_fuel = get_fuel(fuel);

    if (new_fuel <= 0)
    {
        return fuel;
    }

    return get_fuel_sum(new_fuel) + fuel;
}

int main()
{
    std::ifstream input("/home/michelle/AoC_2019/Day_1/input.txt");
    std::string line;
    int64_t fuel_required = 0;
    int64_t sum_fuel_required = 0;

    // traverses the text file line-by-line and computes FR and SFR on the fly
    while (std::getline(input, line) && !line.empty())
    {
        int64_t fuel = get_fuel(std::stoll(line));

        fuel_required += fuel;
        sum_fuel_required += get_fuel_sum(fuel);
    }

    // tada - print out the results
    printf("Fuel required: %lu\n", fuel_required);
    printf("Sum of fuel required: %lu\n", sum_fuel_required);
}
