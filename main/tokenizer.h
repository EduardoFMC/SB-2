#ifndef TOKENIZER_H_INCLUDED
#define TOKENIZER_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "tradutor.h"

bool loneLabel(vector<string> linha){
    if ((linha.size() == 1) && (isLabel(linha[0]))){
        return true;
    } else{
        return false;
    }
}

vector<string> trataPlus(string tokenPlus){
    vector<string> vec;
    istringstream iss(tokenPlus);
    string s;
    while (getline(iss, s, '+')) {
        vec.push_back(s);
    }
    return vec;
}

// Separa os tokens em linhas, assim saberemos exatamente qual linha de erro ir� acontecer e qual
vector<vector<string>> tokenParser(string arquivo){
    ifstream file(arquivo);
    string str;
    string labelLinha;
    bool flag = false;
    vector<vector<string>> programa;
    vector<string> linha;
    vector<string> vec; // para X+int
    string hex_prefix = "0X"; // em maisculo por causa da tranforma��o em maisculo abaixo

    while (getline(file, str)) {
        transform(str.begin(), str.end(), str.begin(), ::toupper); // tranforma em maiusculo
        istringstream iss(str.substr(0, str.find(';'))); // remove comentarios. Possivel erro: � possivel que haja espa�o a mais no final da linha
        string token;

        if (str.length() != 0){ // Ignora linhas vazias

            while (getline(iss, token, ' ')){
                token = token.c_str();

                if (token.find_first_not_of(' ') != string::npos){ // remove espa�os desnecessarios

                    if (token.find(hex_prefix, 0) == 0){
                        token = token.substr(hex_prefix.length());
                        token = to_string(stoul(token, nullptr, 16));
                    }
                    if (flag){
                        flag = false;
                        linha.push_back(labelLinha);
                    }

                    linha.push_back(token);

                }
            }
            if (loneLabel(linha)){
                flag = true;
                labelLinha = linha[0];
                linha.clear();
                //continue;
            }
            if (linha.size() != 0 && !flag){ // Como eliminamos na linha 18, precisamos checar se a linha nao � vazia
                programa.push_back(linha);
                linha.clear();
            }

        }
    }
    return programa;

}

// criar os aqruivos .PRE .MCR
void createFile(vector<vector<string>> programa,  std::string file_name, std::string extension){

    std::ofstream outfile (file_name + extension); // criar arquivo vazio

    cout << file_name + extension;

    for (int i=0; i < programa.size(); i++){
        for (int j=0; j < programa[i].size(); j++) {
            outfile << programa[i][j];
            outfile << " ";
        }
        outfile << "\n";
    }

    outfile.close();
}

void createFileObj(string objeto, std::string file_name, std::string extension){

    std::ofstream outfile (file_name + extension); // criar arquivo vazio
    outfile << objeto;
    outfile.close();
}

#endif // TOKENIZER_H_INCLUDED
