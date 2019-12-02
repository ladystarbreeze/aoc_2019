/*
 * Note to self: this is **very** ugly!!!!
 */

#include <cinttypes>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// this function splits a string by ',' and stuffs the individual tokens into a vector of uint64_ts
std::vector<uint64_t> split(const std::string &string)
{
    std::vector<uint64_t> vec;
    std::istringstream s_stream(string);
    std::string token;

    while (std::getline(s_stream, token, ',') && !token.empty())
    {
        vec.push_back(std::stoul(token));
    }

    return vec;
}

/* that's the heart of our solution: the interpreter!
 * right now, it supports two "ALU instructions":
 * ADD c, a, b (in memory: op byte, source a, source b, dest c), mem[c] = mem[a] + mem[b]
 * and
 * MUL c, a, b (in memory: op byte, source a, source b, dest c), mem[c] = mem[a] * mem[b]
 */
inline void interpret(std::vector<uint64_t> &program, size_t &pc)
{
    if (program[pc] == 1)
    {
        uint64_t a_add = program[program[pc + 1u]];
        uint64_t b_add = program[program[pc + 2u]];

        program[program[pc + 3u]] = a_add + b_add;
    }
    else if (program[pc] == 2)
    {
        uint64_t a_mul = program[program[pc + 1u]];
        uint64_t b_mul = program[program[pc + 2u]];

        program[program[pc + 3u]] = a_mul * b_mul;
    }

    pc += 4u; // increment by four since every instruction has a length of four uint64_ts
}

int main()
{
    std::ifstream input("/home/michelle/AoC_2019/Day_2/input.txt");
    std::string line;

    std::getline(input, line);

    std::vector<uint64_t> program = split(line);
    std::vector<uint64_t> copy = program;
    size_t pc = 0;

    uint64_t c_size = copy.size();
    uint64_t value_p1 = 0;
    uint64_t noun = 0;
    uint64_t verb = 0;

    // naively test all combinations of nouns and verbs until mem[0] = 19690720
    while (copy[0] != 19690720)
    {
        copy = program;
        copy[1] = noun;
        copy[2] = verb;
        pc = 0;

        // run interpreter until it hits a STOP opcode
        while (copy[pc] != 99)
        {
            interpret(copy, pc);
        }

        // SOLUTION TO PART 1!!!!!
        if (noun == 12 && verb == 2)
        {
            value_p1 = copy[0];
        }

        ++verb;

        if (verb == c_size)
        {
            ++noun;
            verb = 0;
        }
    }

    // yay, print out the results!
    printf("Value at position 0: %lu\n", value_p1);
    printf("100 * noun + verb = %lu\n", 100 * noun + verb - 1u);
}
