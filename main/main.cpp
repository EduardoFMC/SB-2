#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include "tradutor.h"
#include "tokenizer.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[])
{
    //string arquivo = argv[1];
    string linha;
    vector<vector<string>> programa = tokenParser("equtest.asm");

    preprocess(programa);
    for ( int i =0 ; i < programa.size();i++){
        for ( int j =0 ; j < programa[i].size();j++){
            cout << programa[i][j];
            cout << " ";
        }
        cout << "\n";
    }
    cout << "\n";
    cout << "\n";
    for ( int i =0 ; i < programa.size();i++){
        linha = to_IA32(programa[i]);
        cout << linha;
     }


    return 0;
}
