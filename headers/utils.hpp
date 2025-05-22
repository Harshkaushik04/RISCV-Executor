#include<unordered_map>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<bitset>
#include<algorithm>

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
    std::unordered_map<u_int32_t,u_int32_t> addressToTextInstructionMapping;
    void fetch_instruction();
    IMEM(std::unordered_map<u_int32_t,u_int32_t>);
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
    std::unordered_map<std::string,u_int32_t> registerToValueMapping;
    void readRegistors();
    void writeRegistor();
    RegEntry();
    RegEntry(std::unordered_map<std::string,u_int32_t>);
};

class BranchComparator{
public:
    u_int32_t inputs[2]; //dataA,dataB
    int controlInputSignal;
    int* controlOutputSignals;
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
    u_int32_t inputs[2];
    std::string controlInputSignal;
    u_int32_t output;
    std::unordered_map<u_int32_t,u_int32_t> addressToDataMapping;
    void readMemory();
    void writeMemory();
    DMEM();
    DMEM(std::unordered_map<u_int32_t,u_int32_t>);
};

class ImmGenerator{
public:
    u_int32_t inputs[1];
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
    ControlLogic();
};