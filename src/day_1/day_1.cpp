#include <cmath>
#include <fstream>
#include <string>

inline int64_t sum_fuel(const int64_t fuel)
{
    int64_t new_fuel = (int64_t)floor((double)fuel / 3.) - 2;

    if (new_fuel <= 0)
    {
        return fuel;
    }

    return sum_fuel(new_fuel) + fuel;
}

int main()
{
    std::ifstream input("/home/michelle/AoC_2019/Day_1/input.txt");
    std::string line;
    int64_t fuel_required = 0;
    int64_t sum_fuel_required = 0;

    while (std::getline(input, line) && !line.empty())
    {
        int64_t fuel = (int64_t)floor(std::stod(line) / 3.) - 2;

        fuel_required += fuel;
        sum_fuel_required += sum_fuel(fuel);
    }

    printf("Fuel required: %lu\n", fuel_required);
    printf("Sum of fuel required: %lu\n", sum_fuel_required);
}
