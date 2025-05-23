#include "utils.hpp"

class Executor{
public:
    std::string inputFilePath;
    std::vector<std::string> outputDataLines;
    u_int32_t endInstruction;

    //architecture
    twoMultiplexer m1;
    u_int32_t pc;
    Adder adder;
    IMEM imem;
    RegEntry reg_entry;
    ImmGenerator imm_gen;
    BranchComparator branch_comp;
    twoMultiplexer m2;
    twoMultiplexer m3;
    ALU alu;
    DMEM dmem;
    threeMultiplexer m4;
    ControlLogic control_logic;

    void inputFileParser();
    void setupControlLogic(Instruction);
    void applyControlLogic();
    void execueteInstruction();
    void execueteALL();
    Executor(std::string);
};