#include "../headers/executor.hpp"
using namespace std;

int main(){
    Executor executor=Executor("../testInputs/test3.mc");
    executor.inputFileParser();
    cout<<"executor.addressToDataMapping"<<endl;
    for(pair p:executor.addressToDataMapping){
        cout<<"0x"<<hex<<p.first<<":"<<"0x"<<hex<<p.second<<endl;
    }
    cout<<"executor.addressToTextInstructionMapping"<<endl;
    for(pair p:executor.addressToTextInstructionMapping){
        cout<<"0x"<<hex<<p.first<<":"<<"0x"<<hex<<p.second<<endl;
    }
    return 0;
}