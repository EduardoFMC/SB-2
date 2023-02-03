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
    vector<string> vec_linhas;
    vector<string> dot_data = cria_data_section();
    vector<string> dot_bss = cria_bss_section();
    int flag; // flag de quando tiver um STOP

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

    for ( int i = 1 ; i < programa.size();i++){

        linha = to_IA32(programa[i]);
        vec_linhas.push_back(linha);

        if (programa[i][0] == "STOP"){
            flag = i+1;
            break;
        }


     }
     if (programa[flag][1] == "DATA"){

            for (int j = flag ; j < programa.size() ; j++){
                if (programa[j][1] == "CONST"){

                    if (programa[j][2].find_first_not_of("0123456789") == string::npos){
                        dot_data.push_back(getLabel(programa[j][0]) + "\t dd " + programa[j][2]);
                    } else {
                        dot_data.push_back(getLabel(programa[j][0]) + "\t db " + programa[j][2]);
                    }
                }

                if (programa[j][1] == "SPACE"){

                    if (programa[j].size() > 2){ // checa se X: SPACE 3
                        dot_bss.push_back(getLabel(programa[j][0]) + "\t resd " + programa[j][2]);
                    } else {
                        dot_bss.push_back(getLabel(programa[j][0]) + "\t resd 1");
                    }
                }
            }
    }

    for ( int i =0 ; i < dot_data.size();i++){
        cout << dot_data[i] + "\n";
    }
    cout << "\n";
    for ( int i =0 ; i < dot_bss.size();i++){
        cout << dot_bss[i] + "\n";
    }
    cout << "\nsection.text\nglobal _start\n\n";
    for ( int i =0 ; i < vec_linhas.size();i++){
        cout << vec_linhas[i];
    }
    return 0;
}
