#include "../headers/utils.hpp"
using namespace std;

RegEntry::RegEntry(){}
RegEntry::RegEntry(std::unordered_map<std::string,u_int32_t> registerToValueMapping){
    this->registerToValueMapping=registerToValueMapping;
    this->register_to_int_mapping={};
    for(int i=0;i<32;i++){
        register_to_int_mapping["x"+to_string(i)]=i;
    }
    for(int i=0;i<3;i++){
        register_to_int_mapping["t"+to_string(i)]=i+5;
    }
    for(int i=0;i<2;i++){
        register_to_int_mapping["s"+to_string(i)]=i+8;
    }
    for(int i=0;i<8;i++){
        register_to_int_mapping["a"+to_string(i)]=i+10;
    }
    for(int i=2;i<12;i++){
        register_to_int_mapping["s"+to_string(i)]=i+16;
    }
    for(int i=3;i<7;i++){
        register_to_int_mapping["t"+to_string(i)]=i+25;
    }
    register_to_int_mapping["zero"]=0;
    register_to_int_mapping["ra"]=1;
    register_to_int_mapping["sp"]=2;
    register_to_int_mapping["gp"]=3;
    register_to_int_mapping["tp"]=4;
    register_to_int_mapping["fp"]=8;
    this->int_to_register_mapping={};
    for(int i=0;i<32;i++){
        int_to_register_mapping[i]="x"+to_string(i);
    }
}
DMEM::DMEM(){}
DMEM::DMEM(std::unordered_map<u_int32_t,u_int32_t>addressToDataMapping){
    this->addressToDataMapping=addressToDataMapping;
}
Adder::Adder(){}
IMEM::IMEM(unordered_map<u_int32_t,u_int32_t>addressToTextInstructionMapping){
    this->addressToTextInstructionMapping=addressToTextInstructionMapping;
}
IMEM::IMEM(){}
twoMultiplexer::twoMultiplexer(){}
threeMultiplexer::threeMultiplexer(){}
BranchComparator::BranchComparator(){}
ALU::ALU(){}
ImmGenerator::ImmGenerator(){}
ControlLogic::ControlLogic(){}

void Adder::add(){
    output=inputs[0]+inputs[1];
}

void IMEM::fetch_instruction(){
    output=addressToTextInstructionMapping[inputs[0]];
}

void twoMultiplexer::decide(){
    output=inputs[controlInputSignal];
}

void threeMultiplexer::decide(){
    output=inputs[controlInputSignal];
}

void RegEntry::readRegistors(){
    outputs[0]=registerToValueMapping[int_to_register_mapping[inputs[0]]];
    outputs[1]=registerToValueMapping[int_to_register_mapping[inputs[1]]];
}

void RegEntry::writeRegistor(){
    registerToValueMapping[int_to_register_mapping[inputs[2]]]=inputs[3];
}

void BranchComparator::compare(){
    if((int32_t) inputs[0]>(int32_t) inputs[1]){
        controlOutputSignals[0]=0;
        controlOutputSignals[1]=0;
    }
    else if(inputs[0]==inputs[1]){
        controlOutputSignals[0]=1;
        controlOutputSignals[1]=0;
    }
    else if((int32_t) inputs[0]<(int32_t) inputs[1]){
        controlOutputSignals[0]=0;
        controlOutputSignals[1]=1;
    }
}

void ALU::compute(){
    if(controlInputSignal=="add"){
        output=inputs[0]+inputs[1];
    }
    else if(controlInputSignal=="sub"){
        output=inputs[0]-inputs[1];
    }
    else if(controlInputSignal=="A"){
        output=inputs[0];
    }
    else if(controlInputSignal=="B"){
        output=inputs[1];
    }
    else if(controlInputSignal=="mul"){
        output=inputs[0]*inputs[1];
    }
    else if(controlInputSignal=="div"){
        output=inputs[0]/inputs[1];
    }
    else if(controlInputSignal=="rem"){
        output=inputs[0]%inputs[1];
    }
    else if(controlInputSignal=="sll"){
        output=inputs[0]<<inputs[1];
    }
    else if(controlInputSignal=="slt"){
        if((int32_t)inputs[0]<(int32_t)inputs[1]){
            output=1;
        }
        else{
            output=0;
        }
    }
    else if(controlInputSignal=="sltu"){
        if(inputs[0]<inputs[1]){
            output=1;
        }
        else{
            output=0;
        }
    }
    else if(controlInputSignal=="xor"){
        output=inputs[0]^inputs[1];
    }
    else if(controlInputSignal=="srl"){
        output=inputs[0]>>inputs[1];
    }
    else if(controlInputSignal=="sra"){
        output=int32_t(inputs[0])>>inputs[1];
    }
    else if(controlInputSignal=="or"){
        output=inputs[0]|inputs[1];
    }
    else if(controlInputSignal=="and"){
        output=inputs[0]&inputs[1];
    }
}

void DMEM::readMemory(){
    if(controlInputSignal=="read"){
        output=addressToDataMapping[inputs[0]];
    }
}

void DMEM::writeMemory(){
    if(controlInputSignal=="write"){
        addressToDataMapping[inputs[0]]=inputs[1];
    }
}

void ImmGenerator::generate(){
    if(controlInputSignal=="I"){
        u_int32_t imm=inputs[0]>>25;
        //check the 12th bit 
        if(imm&(1<<11)){
            output=imm|0xFFFFF000; //sign extend
        }
        else{
            output=imm;
        }
    }
    else if(controlInputSignal=="S"){
        u_int32_t imm=(inputs[0]&(0b11111<<7))>>7|(inputs[0]&(0b1111111<<25)>>20);
        //check the 12th bit 
        if(imm&(1<<11)){
            output=imm|0xFFFFF000; //sign extend
        }
        else{
            output=imm;
        }
    }
    else if(controlInputSignal=="B"){
        u_int32_t imm=(inputs[0]&(0b1111<<8))>>7|(inputs[0]&(0b111111<<25))>>20|(inputs[0]&(0b1<<7)<<4|(inputs[0]&(0b1<<31)>>19));
        //check the 13th bit
        if(imm&(1<<12)){
            output=imm|0xFFFFE000; //sign extend
        }
        else{
            output=imm;
        }
    }
    else if(controlInputSignal=="J"){
        u_int32_t imm=(inputs[0]&0b1111111111<<21)>>20|(inputs[0]&0b1<<20)>>9|(inputs[0]&0xFF<<12)|(inputs[0]&0b1<<31)>>11;
        //check 21st bit
        if(imm&(1<<20)){
            output=imm|0xFFE00000; //sign extend
        }
        else{
            output=imm;
        }
    }
    else if(controlInputSignal=="U"){
        output=inputs[0]&0xFFFFF000;
    }
}

Instruction ControlLogic::findInstruction(u_int32_t inst){
    u_int32_t opcode=inst&0b1111111;
    if(opcode==0b0110011){
        //R format 
        u_int32_t func3=(inst&0b111<<12)>>12;
        u_int32_t func7=(inst&0b1111111<<25)>>25;
        if(func3==0){
            if(func7==0){
                return ADD;
            }
            else if(func7==0b0100000){
                return SUB;
            }
            else if(func7==1){
                return MUL;
            }
        }
        else if(func3==0b001){
            return SLL;
        }
        else if(func3==0b010){
            return SLT;
        }
        else if(func3==0b011){
            return SLTU;
        }
        else if(func3==0b100){
            if(func7==0){
                return XOR;
            }
            else if(func7==1){
                return DIV;
            }
        }
        else if(func3==0b101){
            if(func7==0){
                return SRL;
            }
            else if(func7==0b0100000){
                return SRA;
            }
        }
        else if(func3==0b110){
            if(func7==0){
                return OR;
            }
            else if(func7==1){
                return REM;
            }
        }
        else if(func3==0b111){
            return AND;
        }
    }
    else if(opcode==0b0010011){
        //I format not including load
        u_int32_t func3=(inst&0b111<<12)>>12;
        u_int32_t func7=(inst&0b1111111<<25)>>25;
        if(func3==0){
            return ADDI;
        }
        else if(func3==0b010){
            return SLTI;
        }
        else if(func3==0b011){
            return SLTIU;
        }
        else if(func3==0b100){
            return XORI;
        }
        else if(func3==0b110){
            return ORI;
        }
        else if(func3==0b111){
            return ANDI;
        }
        else if(func3==0b001){
            return SLLI;
        }
        else if(func3==0b101){
            if(func7==0){
                return SRLI;
            }
            else if(func7==0b0100000){
                return SRAI;
            }
        }
        else if(func3==0b010){
            return SLTI;
        }
    }
    else if(opcode=0b0000011){
        //load
        u_int32_t func3=(inst&0b111<<12)>>12;
        if(func3==0){
            return LB;
        }
        else if(func3==0b001){
            return LH;
        }
        else if(func3==0b010){
            return LW;
        }
        else if(func3==0b100){
            return LBU;
        }
        else if(func3==0b101){
            return LHU;
        }
    }
    else if(opcode==0b0100011){
        //save
        u_int32_t func3=(inst&0b111<<12)>>12;
        if(func3==0){
            return SB;
        }
        else if(func3=0b001){
            return SH;
        }
        else if(func3=0b010){
            return SW;
        }
    }
    else if(opcode==0b1100011){
        //branch
        u_int32_t func3=(inst&0b111<<12)>>12;
        if(func3==0){
            return BEQ;
        }
        else if(func3=0b001){
            return BNE;
        }
        else if(func3=0b100){
            return BLT;
        }
        else if(func3==0b101){
            return BGE;
        }
        else if(func3==0b110){
            return BLTU;
        }
        else if(func3==0b111){
            return BGEU;
        }
    }
    else if(opcode==0b0110111){
        return LUI;
    }
    else if(opcode==0b0010111){
        return AUIPC;
    }
    else if(opcode==0b1100111){
        return JALR;
    }
    else if(opcode==0b1101111){
        return JAL;
    }
}