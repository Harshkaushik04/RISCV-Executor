#include "../headers/utils.hpp"
using namespace std;

RegEntry::RegEntry(){}
RegEntry::RegEntry(std::unordered_map<std::string,u_int32_t> registerToValueMapping){
    this->registerToValueMapping=registerToValueMapping;
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