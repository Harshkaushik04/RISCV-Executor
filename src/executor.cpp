#include "../headers/executor.hpp"
using namespace std;

string int_to_hex_string(int num){
    stringstream ss;
    ss<<setfill('0')<<setw(8)<<hex<<num;
    return ss.str();
}

string int_to_2digitHex_string(int num){
    stringstream ss;
    ss<<setfill('0')<<setw(2)<<hex<<num;
    return ss.str();
}


string int_to_binary_string(int value,int numBits) {
    return bitset<32>(value).to_string().substr(32 - numBits);  
}

vector<string> string_to_hex_bytes(string input) {
    vector<string> hex_bytes;
    for (unsigned char c : input) {
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << static_cast<int>(c);
        hex_bytes.push_back(ss.str());
    }
    return hex_bytes;
}

Executor::Executor(string inputFilePath){
    this->inputFilePath=inputFilePath;
    endInstruction=0; //initialise

    //architecture
    twoMultiplexer m1=twoMultiplexer();
    u_int32_t pc=0x0;
    Adder adder=Adder();
    IMEM imem=IMEM();
    RegEntry reg_entry=RegEntry();
    ImmGenerator imm_gen=ImmGenerator();
    BranchComparator branch_comp=BranchComparator();
    twoMultiplexer m2=twoMultiplexer();
    twoMultiplexer m3=twoMultiplexer();
    ALU alu=ALU();
    DMEM dmem=DMEM();
    threeMultiplexer m4=threeMultiplexer();
    ControlLogic control_logic=ControlLogic();
}

void Executor::inputFileParser(){
    //INPUT-> inputFilePath
    //OUTPUT->addressToTextInstructionMapping,addressToDataMapping
    ifstream file(inputFilePath);  
    string line,current;
    vector<string> tokens;
    //text instructions
    while (getline(file, line)) {
        tokens=vector<string>();
        if(line=="END"){
            for(char ch:line){
                if(ch==' ' && !current.empty()){
                    endInstruction=stoul(current,nullptr,16);
                    current.clear();
                    break;
                }
                current+=ch;
            }
            break;
        }
        for(char ch:line){
            if(ch==' ' && !current.empty()){
                tokens.emplace_back(current);
                current.clear();
                continue;
            }
            else if(ch==','){
                break;
            }
            current+=ch;
        }
        if(!current.empty()){
            tokens.emplace_back(current);
            current.clear();
        }
        imem.addressToTextInstructionMapping[stoul(tokens[0],nullptr,16)]=stoul(tokens[1],nullptr,16);
    }
    //data
    while(getline(file,line)){
        tokens=vector<string>();
        for(char ch:line){
            if(ch==' ' && !current.empty()){
                tokens.emplace_back(current);
                current.clear();
                continue;
            }
            current+=ch;
        }
        if(!current.empty()){
            tokens.emplace_back(current);
            current.clear();
        }
        dmem.addressToDataMapping[stoul(tokens[0],nullptr,16)]=stoul(tokens[1],nullptr,16);
    }
}

void Executor::setupControlLogic(Instruction instruction){
    control_logic.instruction=instruction;
    //pcsel is function of brun,breq,brlt , so would be set later in logic
    //R format
    if(instruction==ADD||instruction==SUB||instruction==SLL||instruction==SLT||instruction==SLTU||
        instruction==XOR||instruction==SRL||instruction==SRA||instruction==OR||instruction==AND||
        instruction==MUL||instruction==DIV||instruction==REM){
            // ADD,SUB,SLL,SLT,SLTU,XOR,SRL,SRA,OR,AND,MUL,DIV,REM,
            control_logic.pcSel=0;
            control_logic.immSel="I"; //dont care(so just initialising)
            control_logic.regWEn=1;
            control_logic.brUn=0; //dont care
            control_logic.brEq=0; //dont care
            control_logic.brLt=0; //dont care
            control_logic.Bsel=0;
            control_logic.Asel=0;
            control_logic.MemRW="read";
            control_logic.WBsel=1;
            if(instruction==ADD){
                control_logic.ALUsel="add";
            }
            else if(instruction==SUB){
                control_logic.ALUsel="sub";
            }
            else if(instruction==SLL){
                control_logic.ALUsel="sll";
            }if(instruction==ADD){
                control_logic.ALUsel="add";
            }
            else if(instruction==SUB){
                control_logic.ALUsel="sub";
            }
            else if(instruction==SLL){
                control_logic.ALUsel="sll";
            }
            else if(instruction==SLT){
                control_logic.ALUsel="slt";
            }
            else if(instruction==SLTU){
                control_logic.ALUsel="sltu";
            }
            else if(instruction==XOR){
                control_logic.ALUsel="xor";
            }
            else if(instruction==SRL){
                control_logic.ALUsel="srl";
            }
            else if(instruction==SRA){
                control_logic.ALUsel="sra";
            }
            else if(instruction==OR){
                control_logic.ALUsel="or";
            }
            else if(instruction==AND){
                control_logic.ALUsel="and";
            }
            else if(instruction==MUL){
                control_logic.ALUsel="mul";
            }
            else if(instruction==DIV){
                control_logic.ALUsel="div";
            }
            else if(instruction==REM){
                control_logic.ALUsel="rem";
            }
            else if(instruction==SLT){
                control_logic.ALUsel="slt";
            }
            else if(instruction==SLTU){
                control_logic.ALUsel="sltu";
            }
            else if(instruction==XOR){
                control_logic.ALUsel="xor";
            }
            else if(instruction==SRL){
                control_logic.ALUsel="srl";
            }
            else if(instruction==SRA){
                control_logic.ALUsel="sra";
            }
            else if(instruction==OR){
                control_logic.ALUsel="or";
            }
            else if(instruction==AND){
                control_logic.ALUsel="and";
            }
            else if(instruction==MUL){
                control_logic.ALUsel="mul";
            }
            else if(instruction==DIV){
                control_logic.ALUsel="div";
            }
            else if(instruction==REM){
                control_logic.ALUsel="rem";
            }
        }
        else if(instruction==ADDI||instruction==SLTI||instruction==SLTIU||instruction==XORI||
            instruction==ORI||instruction==ANDI||instruction==SLLI||instruction==SRLI||instruction==SRAI||
            instruction==LD||instruction==LB||instruction==LH||instruction==LW||instruction==LBU||
            instruction==LHU){
                //     ADDI,SLTI,SLTIU,XORI,ORI,ANDI,SLLI,SRLI,SRAI,LD,LB,LH,LW,LBU,LHU,
                control_logic.pcSel=0;
                control_logic.immSel="I"; 
                control_logic.regWEn=1;
                control_logic.brUn=0; //dont care
                control_logic.brEq=0; //dont care
                control_logic.brLt=0; //dont care
                control_logic.Bsel=0;
                control_logic.Asel=0;
                control_logic.MemRW="read";
                control_logic.WBsel=1;
                if(instruction==ADDI){
                    control_logic.ALUsel="add";
                }
                else if(instruction==SLTI){
                    control_logic.ALUsel="slt";
                }
                else if(instruction==SLTIU){
                    control_logic.ALUsel="sltu";
                }
                else if(instruction==XORI){
                    control_logic.ALUsel="xor";
                }
                else if(instruction==ORI){
                    control_logic.ALUsel="ow";
                }
                else if(instruction==ANDI){
                    control_logic.ALUsel="and";
                }
                else if(instruction==SLLI){
                    control_logic.ALUsel="sll";
                }
                else if(instruction==SRLI){
                    control_logic.ALUsel="srl";
                }
                else if(instruction==SRAI){
                    control_logic.ALUsel="sra";
                }
                else if(instruction==LD||instruction==LB||instruction==LH||instruction==LW||
                    instruction==LBU||instruction==LHU){
                    control_logic.ALUsel="add";
                }
            }
            else if(instruction==SB||instruction==SH||instruction==SW||instruction==SD){
                //     SB,SH,SW,SD,
                control_logic.pcSel=0;
                control_logic.immSel="S"; 
                control_logic.regWEn=0;
                control_logic.brUn=0; //dont care
                control_logic.brEq=0; //dont care
                control_logic.brLt=0; //dont care
                control_logic.Bsel=1;
                control_logic.Asel=0;
                control_logic.ALUsel="add";
                control_logic.MemRW="write";
                control_logic.WBsel=1; //dont care
            }
            else if(instruction==BEQ||instruction==BNE||instruction==BLT||instruction==BLTU||
                instruction==BGEU){
                // BEQ,BNE,BGE,BLT,BLTU,BGEU,
                // pcsel would be set later
                //breq,brlt also to be set later
                control_logic.pcSel=0; //initialise
                control_logic.immSel="B"; 
                control_logic.regWEn=0;
                control_logic.brEq=0; //initialise
                control_logic.brLt=0; //initialise
                control_logic.Bsel=1;
                control_logic.Asel=1;
                control_logic.ALUsel="add";
                control_logic.MemRW="read";
                control_logic.WBsel=1; //dont care
                if(instruction==BEQ){
                    control_logic.brUn=0;
                }
                else if(instruction==BNE){
                    control_logic.brUn=0;
                }
                else if(instruction==BLT){
                    control_logic.brUn=0;                    
                }
                else if(instruction==BLTU){
                    control_logic.brUn=1;                    
                }
                else if(instruction==BGEU){
                    control_logic.brUn=1;                    
                }
            }
            else if(instruction==LUI||instruction==AUIPC){
                //     LUI,AUIPC,
                control_logic.pcSel=0;
                control_logic.immSel="U"; 
                control_logic.regWEn=1;
                control_logic.brUn=0; //dont care
                control_logic.brEq=0; //dont care
                control_logic.brLt=0; //dont care
                control_logic.Bsel=1;
                control_logic.Asel=0; //dont care
                control_logic.MemRW="read";
                control_logic.WBsel=1;
                if(instruction==LUI){
                    control_logic.ALUsel="B";
                }
                else if(instruction==AUIPC){
                    control_logic.ALUsel="add";
                }
            }
            else if(instruction==JALR||instruction==JAL){
                //     JAL,JALR
                control_logic.pcSel=1;
                control_logic.regWEn=1;
                control_logic.brUn=0; //dont care
                control_logic.brEq=0; //dont care
                control_logic.brLt=0; //dont care
                control_logic.Bsel=1;
                control_logic.Asel=0; 
                control_logic.MemRW="read";
                control_logic.WBsel=2;
                control_logic.ALUsel="add";
                if(instruction==JALR){
                    control_logic.immSel="I";
                }
                else if(instruction==JAL){
                    control_logic.immSel="J";
                }
            }
}

void Executor::applyControlLogic(){
    m1.controlInputSignal=control_logic.pcSel;
    imm_gen.controlInputSignal=control_logic.immSel;
    reg_entry.controlInputSignal=control_logic.regWEn;
    branch_comp.controlInputSignal=control_logic.brUn;
    m2.controlInputSignal=control_logic.Asel;
    m3.controlInputSignal=control_logic.Bsel;
    alu.controlInputSignal=control_logic.ALUsel;
    dmem.controlInputSignal=control_logic.MemRW;
    m4.controlInputSignal=control_logic.WBsel;
}

void Executor::execueteALL(){
    //OUTPUT->changes in dmem,imem and reg_entry||outputDataLines
    while(pc!=endInstruction){
        execueteInstruction();
    }
}

void Executor::execueteInstruction(){

}