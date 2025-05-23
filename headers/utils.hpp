#include<map>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<bitset>
#include<algorithm>


enum FormatType{
    Rformat,Iformat,Sformat,
    Bformat,Uformat,Jformat
};

enum Instruction{
    ADD,SUB,SLL,SLT,SLTU,XOR,SRL,SRA,OR,AND,MUL,DIV,REM,
    ADDI,SLTI,SLTIU,XORI,ORI,ANDI,SLLI,SRLI,SRAI,LD,LB,LH,LW,LBU,LHU,
    SB,SH,SW,SD,
    BEQ,BNE,BGE,BLT,BLTU,BGEU,
    LUI,AUIPC,
    JAL,JALR
};

class Adder{
public:
    u_int32_t inputs[2]; //4,pc
    u_int32_t output; //pc+4
    void add();
    Adder();
};

class IMEM{
public:
    u_int32_t inputs[1]; //pc
    u_int32_t output; //instruction
    std::map<u_int32_t,u_int32_t> addressToTextInstructionMapping;
    void fetch_instruction();
    IMEM(std::map<u_int32_t,u_int32_t>);
    IMEM();
};

class twoMultiplexer{
public:
    u_int32_t inputs[2];
    int controlInputSignal;
    u_int32_t output;
    void decide();
    twoMultiplexer();
};

class threeMultiplexer{
    public:
    u_int32_t inputs[3];
    int controlInputSignal;
    u_int32_t output;
    void decide();
    threeMultiplexer();
};

class RegEntry{
public:
    u_int32_t inputs[4]; // addrA,addrB,addrD,dataD
    u_int32_t outputs[2]; //dataA,dataB
    int controlInputSignal;
    std::map<std::string,u_int32_t> registerToValueMapping;
    void readRegistors();
    void writeRegistor();
    std::map<std::string,int> register_to_int_mapping;
    std::map<int,std::string> int_to_register_mapping;
    RegEntry();
    RegEntry(std::map<std::string,u_int32_t>);
};

class BranchComparator{
public:
    u_int32_t inputs[2]; //dataA,dataB
    int controlInputSignal; //Brun
    int controlOutputSignals[2]; //Breq,Brlt
    void compare();
    BranchComparator();
};

class ALU{
public:
    u_int32_t inputs[2];
    std::string controlInputSignal;
    u_int32_t output;
    void compute();
    ALU();
};

class DMEM{
public:
    u_int32_t inputs[2]; //addr,data
    std::string controlInputSignal;
    u_int32_t output;
    std::map<u_int32_t,u_int32_t> addressToDataMapping;
    void readMemory();
    void writeMemory();
    DMEM();
    DMEM(std::map<u_int32_t,u_int32_t>);
};

class ImmGenerator{
public:
    u_int32_t inputs[1]; //instruction
    std::string controlInputSignal;
    u_int32_t output;
    void generate();
    ImmGenerator();
};

class ControlLogic{
public:
    int pcSel;
    u_int32_t instruction;
    std::string immSel;
    int regWEn;
    int brUn;
    int brEq;
    int brLt;
    int Bsel;
    int Asel;
    std::string ALUsel;
    std::string MemRW;
    int WBsel;
    Instruction findInstruction(u_int32_t);
    void PCselModify(Instruction,twoMultiplexer);
    ControlLogic();
};