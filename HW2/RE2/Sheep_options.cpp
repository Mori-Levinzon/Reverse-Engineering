//
// Created by Mor Levinzon on 07/12/2020.
//

#include <iostream>
#include <string>

using namespace std;

int main()
{
    string first = "ABCDEFGHIJKLMN"; // 9
    string second = "LRUD"; // 4
    string third = "0123456789"; // 10
    int size = 14 * 4 * 10;

    int count=0;
    char everything[size][3];

    for(int i=0 ;i<first.size();i++){
        for(int j=0 ;j<second.size();j++){
            for(int k=0 ;k<third.size();k++){
//                cout << "k is " << everything[count] << endl;
                everything[count][0] = first[i];
                everything[count][1] = second[j];
                everything[count][2] = third[k];
//                cout << everything[count] << "\n";
//                cout << everything ;
                count++;
            }
        }
    }


    for (int i = 0; i < size; i++)
        std::cout << everything[i][0]<< everything[i][1]<< everything[i][2]<<endl;

    return 0;
}