#include <iostream>
#include <fstream>
#include <string>
#include "HCTree.hpp"
#include "Helper.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    
    ifstream infile(argv[1], ios::binary);
    ofstream outfile(argv[2], ios::binary);

    string str;
    vector<int> freqs(256, 0);
    HCTree tree;
    int count = 0;

    const char* arg1 = argv[1];
    FancyInputStream bitIS = FancyInputStream(arg1);
    const char* arg2 = argv[2];
    FancyOutputStream bitOS(arg2);

    if(!infile || !outfile) {
        cerr << "Error: not enough arguments.\n";
        return -1;
    }

    if(!infile.good() || !outfile.good()){
        cerr << "Error: invalid files.\n";
        return -1;
    }

    if(argv[1] == argv[2]){
        cerr << "Error: input and output file must be different.\n";
        return -1;
    }

    infile.seekg(0, ios::end);
    str.reserve(infile.tellg());
    infile.seekg(0, ios::beg);

    str.assign((istreambuf_iterator<char>(infile)), 
                istreambuf_iterator<char>());
    
    if(str.size() < 256){
        for(int i = 0; i < str.size(); i++){
            bitOS.write_int(str[i]);
        }
        return 0;
    }
    
    for(int i = 0; i < str.size(); i++) {
        unsigned char sym = str[i];
        freqs[sym]++;
    }
    
    //header
    for(int i = 0; i < freqs.size(); i++) {
        if(freqs[i] != 0){
            bitOS.write_int(i);
            bitOS.write_int(freqs[i]);
            
            cout << (char)i << ": " << freqs[i] << endl;
        }
        
        if(freqs[i] > 0) 
            count++;
    }
    
    bitOS.write_int(2147483647);
    tree.build(freqs); 

    for(int i = 0; i < str.size(); i++) {
        //Exit loop if invalid files
        if(!infile.good() || !outfile.good()) 
            break;
        
        tree.encode(str[i], bitOS);
        
    }

    outfile.seekp(0, ios_base::end);

    bitOS.flush();
    
    outfile.close();
    infile.close();
}
