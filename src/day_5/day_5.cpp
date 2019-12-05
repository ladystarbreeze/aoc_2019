#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// stuffing two bools into a struct and passing that to functions
// is objectively better than having functions accept two bools
struct Parameter_Modes
{
    Parameter_Modes(bool first_is_imm, bool second_is_imm) :
            first_is_imm(first_is_imm), second_is_imm(second_is_imm)
    {

    }

    bool first_is_imm; // is the first parameter an immediate value??
    bool second_is_imm; // is the second parameter an immediate value??? who knows!
};

inline Parameter_Modes get_mode(const std::string &inst)
{
    switch (inst.length())
    {
        case 1:
            return Parameter_Modes(false, false);
        case 3:
            return Parameter_Modes(inst[0] == '1', false);
        case 4:
            return Parameter_Modes(inst[1] == '1', inst[0] == '1');
        default:
            throw std::runtime_error("Invalid instruction length!");
    }
}

// do you remember our friend Splitty "split()" McSplittington?
std::vector<int32_t> split(const std::string &string)
{
    std::vector<int32_t> vec;
    std::istringstream s_stream(string);
    std::string token;

    while (std::getline(s_stream, token, ','))
    {
        vec.push_back(std::stoi(token));
    }

    return vec;
}

// ALU instructions
/*
 * ADD c, a, b (in memory: op, a, b, c) - adds two numbers and stores them at address c
 * now supports a fancy new parameter mode: the immediate mode!! big OMG moment
 */
void add(std::vector<int32_t> &program, size_t &pc, Parameter_Modes modes)
{
    int32_t a = ((modes.first_is_imm) ? program[pc++] : program[program[pc++]]);
    int32_t b = ((modes.second_is_imm) ? program[pc++] : program[program[pc++]]);
    auto    c = (size_t)program[pc++];

    program[c] = a + b;

    printf("ADD %7lu, %7d%s, %7d%s\n", c, a, ((modes.first_is_imm) ? "(imm)" : "(mem)"),
           b, ((modes.second_is_imm) ? "(imm)" : "(mem)"));
}

/*
 * MUL c, a, b (in memory: op, a, b, c) - multiplies two numbers and stores them at address c
 * supports immediate mode, too! this is so exciting
 */
void mul(std::vector<int32_t> &program, size_t &pc, Parameter_Modes modes)
{
    int32_t a = ((modes.first_is_imm) ? program[pc++] : program[program[pc++]]);
    int32_t b = ((modes.second_is_imm) ? program[pc++] : program[program[pc++]]);
    auto    c = (size_t)program[pc++];

    program[c] = a * b;

    printf("MUL %7lu, %7d%s, %7d%s\n", c, a, ((modes.first_is_imm) ? "(imm)" : "(mem)"),
           b, ((modes.second_is_imm) ? "(imm)" : "(mem)"));
}

/*
 * SLT c, a, b (in memory: op, a, b, c) - stores 1 at address c if a is less than b
 */
void slt(std::vector<int32_t> &program, size_t &pc, Parameter_Modes modes)
{
    int32_t a = ((modes.first_is_imm) ? program[pc++] : program[program[pc++]]);
    int32_t b = ((modes.second_is_imm) ? program[pc++] : program[program[pc++]]);
    auto    c = (size_t)program[pc++];

    program[c] = ((a < b) ? 1 : 0);

    printf("SLT %7lu, %7d%s, %7d%s\n", c, a, ((modes.first_is_imm) ? "(imm)" : "(mem)"),
           b, ((modes.second_is_imm) ? "(imm)" : "(mem)"));
}

/*
 * SEQ c, a, b (in memory: op, a, b, c) - stores 1 at address c if a equals b
 */
void seq(std::vector<int32_t> &program, size_t &pc, Parameter_Modes modes)
{
    int32_t a = ((modes.first_is_imm) ? program[pc++] : program[program[pc++]]);
    int32_t b = ((modes.second_is_imm) ? program[pc++] : program[program[pc++]]);
    auto    c = (size_t)program[pc++];

    program[c] = ((a == b) ? 1 : 0);

    printf("SEQ %7lu, %7d%s, %7d%s\n", c, a, ((modes.first_is_imm) ? "(imm)" : "(mem)"),
           b, ((modes.second_is_imm) ? "(imm)" : "(mem)"));
}

// IO instructions
/*
 * IN a - accepts an input value and stores it at address a
 */
void in(std::vector<int32_t> &program, size_t &pc, const int32_t input)
{
    auto a = (size_t)program[pc++];

    program[a] = input;

    printf("IN  %7lu(1)\n", a);
}

/*
 * OUT a - what could this one do...?
 */
void out(std::vector<int32_t> &program, size_t &pc, Parameter_Modes modes)
{
    int32_t a = ((modes.first_is_imm) ? program[pc++] : program[program[pc++]]);

    // looks like it prints out a value stored in memory
    printf("OUT %7d%s\n", a, ((modes.first_is_imm) ? "(imm)" : "(mem)"));
}

// jump instructions, very cool.
/*
 * JTR a, dest - sets the instruction pointer to dest if a is non-zero
 */
void jtr(std::vector<int32_t> &program, size_t &pc, Parameter_Modes modes)
{
    int32_t a = ((modes.first_is_imm) ? program[pc++] : program[program[pc++]]);
    auto dest = (size_t)((modes.second_is_imm) ? program[pc++] : program[program[pc++]]);

    if (a != 0)
    {
        pc = dest;
    }

    printf("JTR %7d%s, %7lu%s\n", a, ((modes.first_is_imm) ? "(imm)" : "(mem)"),
           dest, ((modes.first_is_imm) ? "(imm)" : "(mem)"));
}

/*
 * JFA a, dest - sets the instruction pointer to dest if a is zero
 */
void jfa(std::vector<int32_t> &program, size_t &pc, Parameter_Modes modes)
{
    int32_t a = ((modes.first_is_imm) ? program[pc++] : program[program[pc++]]);
    auto dest = (size_t)((modes.second_is_imm) ? program[pc++] : program[program[pc++]]);

    if (a == 0)
    {
        pc = dest;
    }

    printf("JFA %7d%s, %7lu%s\n", a, ((modes.first_is_imm) ? "(imm)" : "(mem)"),
           dest, ((modes.first_is_imm) ? "(imm)" : "(mem)"));
}

// our interpreter!
void interpret(std::vector<int32_t> &program, const int32_t input)
{
    size_t pc = 0; // technically not an instruction pointer

    while (program[pc] != 99)
    {
        std::string opcode = std::to_string(program[pc++]);

        switch (opcode[opcode.length() - 1u])
        {
            case '1':
                add(program, pc, get_mode(opcode));
                break;
            case '2':
                mul(program, pc, get_mode(opcode));
                break;
            case '3':
                in(program, pc, input);
                break;
            case '4':
                out(program, pc, get_mode(opcode));
                break;
            case '5':
                jtr(program, pc, get_mode(opcode));
                break;
            case '6':
                jfa(program, pc, get_mode(opcode));
                break;
            case '7':
                slt(program, pc, get_mode(opcode));
                break;
            case '8':
                seq(program, pc, get_mode(opcode));
                break;
            default:
                break; // should never happen
        }
    }
}

int main()
{
    std::ifstream input("/home/michelle/AoC_2019/Day_5/input.txt");
    std::string line;

    std::getline(input, line);

    std::vector<int32_t> program = split(line);
    std::vector<int32_t> copy = program;

    // part 1
    printf("------------------------------------------\n");
    interpret(copy, 1);
    printf("------------------------------------------\n");

    copy = program;
