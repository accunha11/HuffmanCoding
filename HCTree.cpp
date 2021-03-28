#include <queue>
#include <vector>
#include <fstream>
#include <stack>
#include "Helper.hpp"
#include "HCTree.hpp"
using namespace std;

/*Huffman Tree Builder*/
void HCTree::build(const vector<int>& freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    int count = 0; 
    for(unsigned int i = 0; i < freqs.size(); i++) {
        if(freqs[i] > 0) {
            unsigned char c = (char)i;
            HCNode* temp = new HCNode(freqs[i], c);
            leaves[i] = temp;
            pq.push(temp);
        }

        if(freqs[i] == 0){ 
            count++;
        }
    }

    while(pq.size() > 1) {
        HCNode* parent = new HCNode(0, NULL); 
        int sum;

        parent->c0 = pq.top();
        pq.pop();

        parent->c1 = pq.top();
        pq.pop();

        sum = parent->c1->count + parent->c0->count;
        parent->count = sum;

        parent->c1->p = parent;
        parent->c0->p = parent;
        pq.push(parent);
    }

    if(count != 256) {
        root = pq.top();
        pq.pop();
    }

}

/*Helper Method of HCTree Destructor*/
void DeleteTree(HCNode* node) {
    
    if(node != nullptr){
        if(node->c0 != nullptr){
            DeleteTree(node->c0);
        }
        if(node->c1 != nullptr){
            DeleteTree(node->c1);
        }
        delete node;
    }
}

/* Huffman Tree destructor */
HCTree::~HCTree() {
    
    if(root != nullptr){
        DeleteTree(root);
    }
}

/* Huffman Tree Encoding method */
void HCTree::encode(unsigned char symbol, FancyOutputStream& out) const {
    HCNode* node = leaves[(int) symbol]; 
    stack<int> st; 

    while(node != nullptr && node->p != nullptr) {
        if(node->p->c0 == node){
            st.push(0);
        }
        else if(node->p->c1 == node){
            st.push(1);
        }
        node = node->p;
    }

    while(!st.empty()) {
        out.write_bit(st.top());
        st.pop();
    }
}

/* Huffman Decoding Method */
int HCTree::decode(FancyInputStream& in) const {
    HCNode* node = root;
    int n = 0;

    while(node->c0 != nullptr || node->c1 != nullptr) {
        n = in.read_bit();
        if(n){
            node = node->c1;
        }  
        else{
            node = node->c0;
        }      
    }

    return node->symbol;
    
}

