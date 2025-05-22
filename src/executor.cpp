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
    std::unordered_map<u_int32_t,u_int32_t> addressToTextInstructionMapping={};
    std::unordered_map<u_int32_t,u_int32_t> addressToDataMapping={};
    std::unordered_map<std::string,u_int32_t> registorToValueMapping={};
    std::vector<std::string> outputDataLines={};
    //initialise
    for(int i=0;i<32;i++){
        registorToValueMapping["x"+to_string(i)]=0;
    }
    for(int i=0;i<3;i++){
        registorToValueMapping["t"+to_string(i)]=0;
    }
    for(int i=0;i<2;i++){
        registorToValueMapping["s"+to_string(i)]=0;
    }
    for(int i=0;i<8;i++){
        registorToValueMapping["a"+to_string(i)]=0;
    }
    for(int i=2;i<12;i++){
        registorToValueMapping["s"+to_string(i)]=0;
    }
    for(int i=3;i<7;i++){
        registorToValueMapping["t"+to_string(i)]=0;
    }
    registorToValueMapping["zero"]=0;
    registorToValueMapping["ra"]=0;
    registorToValueMapping["sp"]=0;
    registorToValueMapping["gp"]=0;
    registorToValueMapping["tp"]=0;
    registorToValueMapping["fp"]=0;

    //architecture
    twoMultiplexer m1=twoMultiplexer();
    u_int32_t pc=0x0;
    Adder adder=Adder();
    IMEM imem=IMEM(addressToTextInstructionMapping);
    RegEntry reg_entry=RegEntry(registorToValueMapping);
    ImmGenerator imm_gen=ImmGenerator();
    BranchComparator branch_comp=BranchComparator();
    twoMultiplexer m2=twoMultiplexer();
    twoMultiplexer m3=twoMultiplexer();
    ALU alu=ALU();
    DMEM dmem=DMEM(addressToDataMapping);
    threeMultiplexer m4=threeMultiplexer();
    ControlLogic control_logic=ControlLogic();

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
        addressToTextInstructionMapping[stoul(tokens[0],nullptr,16)]=stoul(tokens[1],nullptr,16);
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
        addressToDataMapping[stoul(tokens[0],nullptr,16)]=stoul(tokens[1],nullptr,16);
    }
}

void Executor::execuete(){
    //INPUT->addressToTextInstructionMapping,addressToDataMapping,registorToValueMapping
    //OUTPUT->registorToValueMapping,outputDataLines
    imem.addressToTextInstructionMapping=addressToDataMapping;
    dmem.addressToDataMapping=addressToDataMapping;
    
}