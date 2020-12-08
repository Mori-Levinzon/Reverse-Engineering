//
// Created by Mor Levinzon on 07/12/2020.
//

//
// Created by Mor Levinzon on 07/12/2020.
//
#include <iostream>
#include <string>

using namespace std;
#include <bits/stdc++.h>
#include <cstdio>
#include <synchapi.h>
#include <processthreadsapi.h>
#include <rpc.h>
long i=0;

void runExe();

void writeToAnswer(const string &possibleInput);

using namespace std;

void runExe() {



//    system("type currentInput.txt >> possibleAnswer.txt");
    system("sheep.exe < currentInput.txt >> possibleAnswer.txt");
    ifstream MyReadFile("possibleAnswer.txt");
    string win;
    while (getline (MyReadFile, win)) {
        // Output the text from the file
        if (win == "You won the game!"){
            system("sheep.exe < currentInput.txt > possibleAnswer.txt");
            system("type currentInput.txt >> possibleAnswer.txt");
            exit(1);
        }
    }
}


void permute(vector<string> allTriples, int l, int r)
{
    // Base case
    if (l == r){
        string possibleInput="";
        for (int i = 0; i < allTriples.size(); i++) {
            possibleInput=possibleInput + allTriples[i];
        }
        writeToAnswer(possibleInput);
        runExe();
    }
    else
    {
        // Permutations made
        for (int i = l; i <= r; i++)
        {

            // Swapping done
            swap(allTriples[l], allTriples[i]);

            // Recursion called
            permute(allTriples, l+1, r);

            //backtrack
            swap(allTriples[l], allTriples[i]);
        }
    }
}

void writeToAnswer(const string &possibleInput) {
    fstream newfile;
    newfile.open("currentInput.txt", ios_base::out);
    if(newfile.is_open()) //checking whether the file is open
    {
        newfile<<possibleInput;
        newfile.close(); //close the file object.
    }

}

int main()
{
    string line;
    int line_count=0;
    int answer_count=0;
    vector<string> rightAnswers=vector<string>();
    vector<string> allTriples= vector<string>();

    fstream newfile;
    newfile.open("in.txt",ios::in);
    if (newfile.is_open()){   //checking whether the file is open
        string tp;
        while(getline(newfile, line)){
            allTriples.push_back(line);
        }
        newfile.close(); //close the file object.
    }
    vector<bool> B = vector<bool>(allTriples.size());
    permute(allTriples, 0, 6);
//    runExe();

}

