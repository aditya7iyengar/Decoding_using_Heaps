//
//  2005-MessageDecoding
//
// Aditya Iyengar
//
// For this project, we first inserted the elements of a header in an IMAGINARY tree which was actually an array.
// We used the reverse algorithm of the level order, making sure that the header was not inserted on the right
// most elements as we cannot have a key with all 1's. So, one 0 would mean tranverse left on the tree and one
// would make it traverse right.
//
// We made our own Node class with variables for left, right and parent. We also made the binary to decimal and
// decimal to binary converters.
//
// The algorithm has a void numbers method which selects the indices of the arrays which would represent the
// right most elements of the tree and hence cannot have a char from the header. That algorithm is O(log N) where
// N is the number of characters in the header.
// This method is then used to initialize the tree. The cost of making the tree is hence O(NlogN). Once the tree is
// initiallized, we set up the children for each node as the index of the children for a node will be:
// left child = 2 * index of the node + 1 ; right child = 2 * index of the node + 2. Which is again O(N).
//
// Once the tree is ready, all that's left is to read the code and and for every 0s in the key, go to the left child
// and for every 1s in the tree go to the right child. Hence, reading the key and getting an output is at the max,
// O(log N).
//
// Therefore, total runtime = O( log N) + O( Nlog N) + O(N) + O(log N). = O (NlogN). 
//


#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <ostream>

using namespace std;

bool is_binary (const string& str) {
    for (char c : str)
        if (c != '0' && c != '1') return false;
    return true;
}

int binaryToDecimal(string input){
    string copy(input);
    reverse(copy.begin(), copy.end());
    input = copy;
    int power =0;
    vector<double> numbers;
    
    for(string::iterator it=input.begin();it!=input.end();it++){
        if(*it=='1')
            numbers.push_back(pow(2, power));
        power++;
    }
    
    int sum = 0;
    for ( auto &d : numbers){
        sum += d;
    }
    return sum;
}
string decimalToBinary(int number) {
	int remainder;
    
	if(number <= 1) {
        stringstream ss1;
        ss1 << number;
		return ss1.str();
	}
    
	remainder = number%2;
    stringstream ss;
    ss << remainder;
	return decimalToBinary(number >> 1) + ss.str();
}

class Node{
public:
    char data;
    string key;
    Node * left;
    Node * right;
    Node * parent;
    
    Node(){
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
    
    Node (char data){
        Node();
        this->data = data;
    }
};

int indexOf ( Node*  arr[], Node * pos){
    Node* temp = arr[0];
    int n =0;
    while ( temp != nullptr){
        if(temp == pos){
            return n;
        }
        n++;
        temp = arr[n];
    }
    return -1;
}

bool voidNumbers ( int check, int power){
    int num = 0;
    for ( int i = 1; i < power ; i++){
        if ( check == num)
            return true;
        num += pow(2,i);
    }
    
    return false;
}


Node *  treeFormer ( string header){
    Node * nArray [(header.size() + (int)log2((header.size()))+ 3)];
    
    int m = 0;
    
    for ( int n = 0; n < (header.size() + (int)log2((header.size()))+ 3); n ++){
        if ( voidNumbers(n, 2* (int) log2(header.size()) + 2)){
            nArray[n] = new Node ();
        }
        else{
            nArray[n] = new Node (header[m]);
            m++;
        }
    }
    
    Node * temp1 = nArray[0];
    int n2 =0;
    while (temp1->data != header[header.size()-1] ){
        int i = indexOf(nArray, temp1);
        if ((2*i + 1) <= (header.size() + (int)log2((header.size()))+ 2)){
            temp1->left = nArray[2*i + 1];
        }
        if ((2*i + 2) <= (header.size() + (int)log2((header.size()))+ 3)){
            temp1->right = nArray[2*i + 2];
        }
        n2++;
        temp1 = nArray[n2];
    }
    
    
    return nArray[0];
}

char keyDecoder (string key,  Node * start){
    Node * temp = start;
    
    for ( int n = 0; n < key.size(); n++){
        if ( key[n] == '0'){
            temp = temp->left;
        }
        
        else if ( key [n] == '1'){
            temp = temp->right;
        }
        
    }
    
    return temp->data;
    
}

bool allOnes ( string code, int length) {
    for ( int n = 0; n < length; n++ ){
        if ( code[n] != '1')
            return false;
    }
    return true;
}


int main(int argc, const char * argv[]) {
    string pre_header, header;
    fstream input(argv[1]);
    
    /* read the header text */
    while (getline(input, header, '\n')) {
        header = pre_header + header;
        string secret, chunk;
        /* read the binary encoded message, line-by-line */
        input >> chunk;
        while (!input.eof() && is_binary (chunk)) {
            secret += chunk; /* concatenate into a longer binary string */
            input >> chunk;
        }
        secret += chunk;
        pre_header = chunk;
        
        Node* start = treeFormer(header);
        
        while (binaryToDecimal(secret.substr(0,3)) != 0){
            int length_key = binaryToDecimal(secret.substr(0,3));
        
            secret = secret.substr(3,secret.length()-3);

            while (! allOnes(secret, length_key) ){
                string key = secret.substr(0,length_key);
                secret = secret.substr(length_key,secret.length() -length_key);
                cerr << keyDecoder(key, start);
            }
            secret = secret.substr(length_key,secret.length()-length_key);
            
        }
        cerr<< "" << endl;
        
    }
    return 0;
}


