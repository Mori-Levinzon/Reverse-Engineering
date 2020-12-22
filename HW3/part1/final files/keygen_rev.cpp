#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;
using std::string;

string setkeygenOutput(const string &input) {
    string command = "keygen.exe " + input + "  >output.txt";
    char c[command.size() + 1];
    strcpy(c, command.c_str());
    system(c);

    fstream outputFile;
    string output;
    outputFile.open("output.txt", ios_base::in);
    if (outputFile.is_open())
    {
        while ( getline (outputFile,output) )
        {
            cout << output << '\n';
        }
        outputFile.close();
    }
    else cout << "Unable to open file";
    return output;
}


int main(int argc, char * argv[]) {
    string reverseDict="";
    for (int i=0;i<256;i++){
        reverseDict += char(' ');
    }
//    string input;
//    for (int i=int(0x21);i<int(0x7F);i++){
//        input+=char(i);
//    }

    string input = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";


    string output = "5";
    output.push_back('`');
    output += ">AdIG-*8Ee}@BDRFkzw]ZYgrsm1q\\2|3Jj(KuX !':_v6aWf{y\"C;+L<S4HO[0?$bh";
    output.push_back('%');
    output += "/MtoT9,N)&l=xUpV^7~n#ciQP";

    for (int i=0;i<int(0x7F)- int(0x21);i++){
        reverseDict[output[i]] = (char)(((char)input[i]));
    }

    string result="";
    int i=0;
    while(argv[1][i]!= (char)('\0')){
        result+=reverseDict[(argv[1][i])];
        i++;
    }
    std::cout << result;

    return 0;
}

