#include "utils.hpp"

class Executor{
public:
    std::string inputFilePath;
    std::unordered_map<u_int32_t,u_int32_t> addressToTextInstructionMapping;
    std::unordered_map<u_int32_t,u_int32_t> addressToDataMapping;
    std::unordered_map<std::string,u_int32_t> registorToValueMapping;
    std::vector<std::string> outputDataLines;

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

    std::unordered_map<std::string,int> register_to_int_mapping;

    void inputFileParser();
    void execuete();
    void execueteRformatInstruction();
    void execueteIformatInstruction();
    void execueteSformatInstruction();
    void execueteBformatInstruction();
    void execueteUformatInstruction();
    void execueteJformatInstruction();
    Executor(std::string);
};