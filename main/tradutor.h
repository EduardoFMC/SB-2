#ifndef TRADUTOR_H_INCLUDED
#define TRADUTOR_H_INCLUDED

#include <bits/stdc++.h>
#include <fstream>
#include <regex>
#include <stdexcept>
#include <cctype>
#include <string>
#include "utils.h"

using namespace std;

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
    // KKKKKKK
    // DEMOREI 3 HORAS SÓ PRA SACAR IISSO DSAKDAKSP
    int op;
    string label;
    string new_line = "";


    if (isLabel(linha[0])){
        new_line += linha[0]+ ' ';
        op = getValue(linha[1], opcodes);
        label = linha[2];
    } else {
        op = getValue(linha[0], opcodes);

        if (op != 14){ // Se for sTOP nao tem outra label na frente sacas?
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
            new_line += "PERGUNTA PRO POSTO IPIRANGA\n";
            break;
        }

        //OUTPUT
        case 13:
        {
            new_line += "SEILA kkk\n";
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
            new_line += "EU GOSTO\n";
            break;
        }

        //OUTPUT_C
        case 16:
        {
            new_line += " DO CHEIRO\n";
            break;
        }

        //INPUT_S
        case 17:
        {
            new_line += "SEBOSO DE UMA\n";
            break;
        }

        //OUTPUT_S
        case 18:
        {
            new_line += "PICA\n";
            break;
        }
    }
    return new_line;
}


#endif // TRANSLATOR_H_INCLUDED
