#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <synchapi.h>


using namespace std;
using std::string;
using std::hex;

string DEFcase(const string &input, int i) {
    int sum = (input[i] - '0') + (input[i + 2] - '0');
    switch (sum){
        case 0x0d:   return "D";
        case 0x0e:   return "E";
        case 0x0f:   return "F";
        default:   return "F";
    }
}


string convert_ASCII(string hex){
    string ascii = "";
    for (size_t i = 0; i < hex.length(); i += 2){
        //taking two characters from hex string
        string part = hex.substr(i, 2);
        //changing it into base 16
        char ch = stoul(part, nullptr, 16);
        //putting it into the ASCII string
        ascii += ch;
    }
    return ascii;
}

string setClientOutput() {
    string command = "client.exe DMSG >output.txt";
    char c[command.size() + 1];
    strcpy(c, command.c_str());
    system(c);
    Sleep(10000);

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

string decypherMessage(string& input) {
    string output="";
    for (int i=0; i < input.size(); i++){
        switch (input[i]){
            case 'A':   output+="1";//'1'
                break;
            case 'J':   output+="A";//'A'
                break;
            case 'Q':   output+="B";//'B'
                break;
            case 'K':   output+="C";//'C'
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (i<input.size()-1 ){
                    if (input[i+1]=='-'){
                        output+="0";//'0'
                        i+=2;
                        break;
                    }else if ((input[i+1]=='+' )){
                        output += DEFcase(input, i);
                        i+=2;
                        break;
                    }
                }
                output+=to_string((((int)(input[i])) - ((int)('0'))));
        }

    }

    return convert_ASCII(output);
}

int main() {

//    string input = setClientOutput();
    string input = "4926-67466+966+96Q26-674686526-67267+86262657229-9636A77-77469766527+726-6486523-3697324-468656K6428-86968+622-2423328+6\n"
            "496626-66666+97224-47366+968+56524-472656A7367+866+824-4776526-6736867+8756K6424-46672656528-874686525-56775792K\n"
            "67+865+96524-468+575737426-6666967+76429-96A27-76368+7646526-66A737367+863696A74656424-47769746828-874686525-55246+94242\n"
            "455258+7434A52-2545552454423-365766568+67427+7\n"
            "57686565+922-27468697324-46366+9646526-6697324-4757365642K29-97266+96K6K6967+76726-674686523-36469636524-4736866+9756K64\n"
            "25-5726573756K7422-27769746829-9637562657322-274686A7423-3737566+728-87468+724-47365766568+625+9\n"
            "\n"
            "4767+8626K6965+9";


    cout << decypherMessage(input) << endl;

    return 0;
}


