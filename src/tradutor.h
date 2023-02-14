#ifndef TRADUTOR_H_INCLUDED
#define TRADUTOR_H_INCLUDED

#include <bits/stdc++.h>
#include <fstream>
#include <regex>
#include <stdexcept>
#include <cctype>
#include <string>
#include <algorithm>
#include <vector>
#include "utils.h"

using namespace std;

vector<string> dot_bss = cria_bss_section();
vector<string> aux_string_bss;
vector<string> aux_char_bss;

map <string,int> opcodes = {
{"ADD", 1},
{"SUB", 2},
{"MULT", 3},
{"DIV", 4},
{"JMP", 5},
{"JMPN", 6},
{"JMPP", 7},
{"JMPZ", 8},
{"COPY", 9},
{"LOAD", 10},
{"STORE", 11},
{"INPUT", 12},
{"OUTPUT", 13},
{"STOP", 14},
{"INPUT_C", 15},
{"OUTPUT_C", 16},
{"INPUT_S", 17},
{"OUTPUT_S", 18},
};

vector<string> diretivas = {"CONST", "EQU", "IF", "SPACE", "SECTION"};

void preprocess (vector<vector<string>> &programa) {
    vector<int> remove_index;
    map <string,int> equs;
    int value;
    string label;

    for (int i=0; i < programa.size(); i++){
        if (programa[i].size() > 1) {
            if (programa[i][1] == "EQU") {
                label = getLabel(programa[i][0]);
                value = atoi(programa[i][2].c_str());
                equs.insert(pair<string, int>(label, value));
                remove_index.push_back(i);

                continue;
            }
        }

        if (programa[i][0] == "IF") {
            value = getValue(programa[i][1], equs);
            remove_index.push_back(i);
            if (value == 0) {
                remove_index.push_back(i+1);
            }
        } else {
            if (programa[i].size() > 1) {
                if ((programa[i][1] == "SPACE") || (programa[i][1] == "CONST")) {
                    if (programa[i].size() > 2) {
                        if (inTS(programa[i][2], equs)) {
                            value = getValue(programa[i][2], equs);
                            programa[i][2] = to_string(value);
                        }
                    }
                }
            }
        }
    }

    sort(remove_index.begin(), remove_index.end(), greater<int>());

    for (int i=0; i < remove_index.size(); i++) {
        programa.erase(programa.begin() + remove_index[i]);
    }
}

string to_IA32(vector<string> linha){
    // MEXER COM EAX PQ É O ACMULADOR
    extern  vector<string> dot_bss;
    extern vector<string> aux_string_bss;
    extern vector<string> aux_char_bss;

    int op;
    string label;
    string new_line = "";

    if (isLabel(linha[0])){
        new_line += linha[0]+ ' ';
        op = getValue(linha[1], opcodes);

        if (op != 14){ // Se for sTOP nao tem outra label na frente
            label = linha[2];
        }

    } else {
        op = getValue(linha[0], opcodes);

        if (op != 14){ // Se for sTOP nao tem outra label na frente
            label = linha[1];
        }

    }

    switch (op){
        //ADD
        case 1:
        {
            new_line += "add EAX, [" + label + "]\n";
            break;
        }

        //SUB
        case 2:
        {
            new_line += "sub EAX, [" + label + "]\n";
            break;
        }

        //MUL
        case 3:
            new_line += "mov ECX, [" + label + "]\n";
            new_line += "imul ECX\n";
            break;

        //DIV
        case 4:
            new_line += "mov ECX, [" + label + "]\n";
            new_line += "cdq\n";
            new_line += "idiv ECX\n";
            break;

        //JMP
        case 5:
        {
            new_line += "JMP " + label + "\n";
            break;
        }

        //JMPN
        case 6:
        {
            new_line += "CMP EAX, 0\n";
            new_line += "JL " + label + "\n";
            break;
        }

        //JMPP
        case 7:
        {
            new_line += "CMP EAX, 0\n";
            new_line += "JG " + label + "\n";
            break;
        }

        //JMPZ
        case 8:
        {
            new_line += "CMP EAX, 0\n";
            new_line += "JE " + label + "\n";
            break;
        }

        // COPY
        case 9:
        {
            vector<string> args_copy = sepOps(label);
            new_line += "mov EBX, [" + args_copy[0] + "]\n";
            new_line += "mov [" + args_copy[1] + "], EBX\n";
            break;
        }

        // LOAD
        case 10:
        {
            new_line += "mov EAX, [" + label + "]\n";
            break;
        }

        //STORE
        case 11:
        {
            new_line += "mov [" + label + "], EAX\n";
            break;
        }

        //INPUT
        case 12:
        {
            new_line += "push string_int\n";
            new_line += "call input_int\n";
            new_line += "mov EAX, [num_int]\n";
            new_line += "mov [" + label + "], EAX\n";
            break;
        }

        //OUTPUT
        case 13:
        {
            new_line += "push dword [" + label + "]\n";
            new_line += "push string\n";
            new_line += "call output_int\n";
            break;
        }

        //STOP
        case 14:
        {
            new_line += "mov EAX, 1\n";
            new_line += "mov EBX, 0\n";
            new_line += "int 80h\n";
            break;
        }

        //INPUT_C
        case 15:
        {
            aux_char_bss.push_back(label);
            new_line += "push " + label + "\n";
            new_line += "call INPUT_C\n";
            break;
        }

        //OUTPUT_C
        case 16:
        {
            new_line += "push " + label + "\n";
            new_line += "call OUTPUT_C\n";
            break;
        }

        //INPUT_S
        case 17:
        {

            vector<string> args_copy = sepOps(label);
            aux_string_bss.push_back(args_copy[0]);

            new_line += "push " + args_copy[0] + "_size" + "\n";
            new_line += "push " + args_copy[1] + "\n";
            new_line += "push " + args_copy[0] + "\n";
            new_line += "call INPUT_S\n";

            break;
        }

        //OUTPUT_S
        case 18:
        {
            new_line += "push " + label + "_size" + "\n";
            new_line += "push " + label + "\n";
            new_line += "call OUTPUT_S\n";
            break;
        }
    }
    return new_line;
}

void montador(vector<vector<string>> &programa, std::string file_name){
    string linha;
    vector<string> vec_linhas;
    string ch;

    vector<string> dot_data = cria_data_section();
    int flag; // flag de quando tiver um STOP

    // Forma de saber se o pre processador está correto, printa o programa pre proc
    /* for ( int i =0 ; i < programa.size();i++){
        for ( int j =0 ; j < programa[i].size();j++){
            cout << programa[i][j];
            cout << " ";
        }
        cout << "\n";
    }
    cout << "\n";
    cout << "\n"; */

    for ( int i = 1 ; i < programa.size();i++){

        linha = to_IA32(programa[i]);
        vec_linhas.push_back(linha);

        if ((programa[i][0] == "STOP") || (programa[i][1] == "STOP")){
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
                    if ((std::find(aux_string_bss.begin(), aux_string_bss.end(), getLabel(programa[j][0]) ) != aux_string_bss.end())){

                        if (programa[j].size() > 2){ // checa se X: SPACE 3
                            dot_bss.push_back(getLabel(programa[j][0]) + "\t resb " + programa[j][2]);
                        } else {
                            dot_bss.push_back(getLabel(programa[j][0]) + "\t resd 1");
                        }

                        dot_bss.push_back(getLabel(programa[j][0])+ "_size" + "\t resd 10");

                    } else if ((std::find(aux_char_bss.begin(), aux_char_bss.end(), getLabel(programa[j][0]) ) != aux_char_bss.end())){

                        dot_bss.push_back(getLabel(programa[j][0]) + "\t resb 1");

                    } else {

                        if (programa[j].size() > 2){ // checa se X: SPACE 3
                            dot_bss.push_back(getLabel(programa[j][0]) + "\t resd " + programa[j][2]);
                        } else {
                            dot_bss.push_back(getLabel(programa[j][0]) + "\t resd 1");
                        }
                    }
                }
            }
    }

    // criação do arquivo final
    std::ofstream outfile (file_name + ".S"); // criar arquivo vazio

    std::fstream asm_file; // arquivo com funções em
    asm_file.open("asm_file.asm", ios::in);

    while (!asm_file.eof()) { // copiando as funções pra inicio do arquivo

        getline(asm_file, ch);
        outfile << ch << endl;
    }
    outfile << "\n\n";

    cout << file_name + ".S";

    for ( int i =0 ; i < dot_data.size();i++){
        outfile << dot_data[i] + "\n";
    }
    outfile << "\n";
    for ( int i =0 ; i < dot_bss.size();i++){
        outfile << dot_bss[i] + "\n";
    }
    outfile << "\nsection .text\nglobal _start\n_start:\n\n";
    for ( int i =0 ; i < vec_linhas.size();i++){
        outfile << vec_linhas[i];
    }

    outfile.close();
}

#endif // TRANSLATOR_H_INCLUDED
