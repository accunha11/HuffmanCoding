#include <iostream>
#include <fstream>
#include <sstream>
#include "HCTree.hpp"
#include "Helper.hpp"
#include <stdlib.h>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    ifstream infile(argv[1], ios::binary);
    ofstream outfile(argv[2], ios::binary);

    vector<int> freqs(256, 0);
    HCTree tree;
    int count = 0;
    string str;

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
        for(int i = 0; i < infile.tellg(); i++){
           bitOS.write_byte(bitIS.read_byte()); 
        }
        return 0;
    }
    
    //header
    for(int i = 0; i < freqs.size(); i++) {
        if(!infile.good() || !outfile.good()){
            break;
        }

        int j = bitIS.read_int();
        if(j == 2147483647){
            break;
        }
        freqs[j] = bitIS.read_int();
        cout << (char)j << freqs[j] << endl;
        count += freqs[j];
    }

    tree.build(freqs); 
    
    for(int i = 0; i < count; i++) {
	    if(!infile.good() || !outfile.good()){
            break;
        } 
        bitOS.write_byte(tree.decode(bitIS));
    } 

    outfile.close();
    infile.close();
    
}
