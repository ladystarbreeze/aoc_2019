/*
 * Note to self: this is incredibly slow! (takes ~2 minutes on my computer)
 */

#include <fstream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

// cute enumeration
enum Direction
{
    Left,
    Right,
    Up,
    Down
};

// my own tuple struct, I can't stand std::tuple (how the fuck do people use it)
struct Tuple
{
    Tuple(int16_t x, int16_t y, uint64_t steps) : x(x), y(y), steps(steps)
    {

    }

    int16_t x;
    int16_t y;
    uint64_t steps;
};

// this is what our wires look like
struct Wire
{
    Wire() : x(0), y(0), steps(0)
    {}

    std::vector<std::pair<Direction, int16_t>> instrs;

    int16_t x;
    int16_t y;
    uint64_t steps;
};

// input parser!
std::vector<std::pair<Direction, int16_t>> parse_line(const std::string &line)
{
    std::vector<std::pair<Direction, int16_t>> vec;
    std::istringstream s_stream(line);
    std::string token;

    while (std::getline(s_stream, token, ','))
    {
        Direction dir;

        switch (token[0])
        {
            case 'L':
                dir = Direction::Left;
                break;
            case 'R':
                dir = Direction::Right;
                break;
            case 'U':
                dir = Direction::Up;
                break;
            case 'D':
                dir = Direction::Down;
                break;
            default:
                throw std::invalid_argument("Unknown direction!");
        }

        token[0] = '0';

        vec.emplace_back(std::pair<Direction, uint16_t>(dir, (int16_t)std::stol(token)));
    }

    return vec;
}

int main()
{
    std::ifstream input("/home/michelle/AoC_2019/Day_3/input.txt");
    std::string line;
    std::list<Tuple> coords[2]; // list of coordinates for our wires
    std::list<Tuple> intersects; // list of intersections
    Wire wire[2]; // our two wires

    // easy: this loop parses our input for each wire
    for (auto &i : wire)
    {
        std::getline(input, line);

        i.instrs = parse_line(line);
    }

    // easy: add all coordinates to the lists
    for (size_t i = 0; i < 2; i++)
    {
        coords[i].emplace_back(Tuple(0, 0, 0));

        for (auto &inst : wire[i].instrs)
        {
            for (int16_t j = 0; j < inst.second; j++)
            {
                // let the branch predictor do the work, I guess
                switch (inst.first)
                {
                    case Direction::Left:
                        --wire[i].x;
                        break;
                    case Direction::Right:
                        ++wire[i].x;
                        break;
                    case Direction::Up:
                        ++wire[i].y;
                        break;
                    case Direction::Down:
                        --wire[i].y;
                        break;
                }

                ++wire[i].steps;

                coords[i].emplace_back(Tuple(wire[i].x, wire[i].y, wire[i].steps));
            }
        }
    }

    // naively traverse the lists and find intersections by comparing coordinate pairs
    // this is SLOW! (for obvious reasons)
    for (auto &coord : coords[0])
    {
        for (auto &coord_2 : coords[1])
        {
            if (coord.x == coord_2.x && coord.y == coord_2.y)
            {
                intersects.emplace_back(Tuple(coord.x, coord.y, coord.steps + coord_2.steps));
            }
        }
    }

    uint16_t distance = UINT16_MAX;
    uint64_t steps = UINT64_MAX;

    // traverse the list of intersections, find the minimum distance and amount of steps
    for (auto &i : intersects)
    {
        auto temp = (uint16_t)(abs(i.x) + abs(i.y));

        if (temp != 0 && temp < distance)
        {
            distance = temp;
        }

        if (i.steps != 0 && i.steps < steps)
        {
            steps = i.steps;
        }
    }

    // ... and print out the results!
    printf("Distance: %u\n", distance);
    printf("Steps: %lu\n", steps);
}
