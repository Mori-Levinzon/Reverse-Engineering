#include <iostream>
#include <cstring>

using namespace std;


int ord(char c){
    return int(int (c)-int('A'));
}

char toChar(int dis){
    return char (dis+int('A'));
}

bool check(string result){
    string shiftinsert="AAAAAAAAAAAAAAAAAAAA";
    for (int i=0;i<20;i++){
        shiftinsert[i]=result[(i+1)%20];
    }
    string check="AAAAAAAAAAAAAAAAAAAA";
    for (int i=0;i<19;i++){
        check[i] = toChar( ord(result[i]) ^ ord(shiftinsert[i]));
    }
    string expectedResult="HNLANJPLKLLNCFCBIIDM";
    std::cout <<" result is :"<<result << std::endl;
    std::cout <<" check is :"<<check << std::endl;
    std::cout <<" real  is :"<<expectedResult << std::endl<< std::endl<< std::endl;
    return check == expectedResult ;
}
int main() {
    string insert="AAAAAAAAAAAAAAAAAAAA";;

    string result="AAAAAAAAAAAAAAAAAAAAH";;

    string expectedResult="HNLANJPLKLLNCFCBIIDM";



        for (int i=19;i>=0;i--){
            result[i] = toChar( ord(result[(i+1)]) ^ ord(expectedResult[i]));
            std::cout <<" result i+1 :"<<ord(result[(i+1)]) <<" expectedResult i :"<<ord(expectedResult[(i)]) << std::endl;
            std::cout <<" xor is :"<<ord(result[i]) << std::endl;
            std::cout <<" xor is :"<<(result[i]) << std::endl;
        }



        if (check(result)){
            std::cout <<" result is :"<<result << std::endl;
        }
    return 0;
}

