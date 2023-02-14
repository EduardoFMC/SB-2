#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

vector<string> sepOps(string ops) {
    string tmp;
    stringstream ss(ops);
    vector<string> words;

    while(getline(ss, tmp, ',')) {
        words.push_back(tmp);
    }

    return words;
}

bool isLabel(string token) {
    if (token[token.length()-1] == ':') {
        return true;
    }
    return false;
}

string getLabel(string token) {
    return token.substr(0, token.size()-1);
}

bool inTS(string token, map <string,int> ts) {
    if (ts.find(token) == ts.end()) {
        return false;
    }
    return true;
}

bool isSymbol(string &token) {
    string::iterator it;

    for (it = token.begin(); it != token.end(); it++){
        if ((it == token.begin()) && (*it == '-')) {
            continue;
        }

        if (isdigit(*it) == 0) {
            return true;
        }
    }
    return false;
}

bool isValidLabel(string token) {
    if (token.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != string::npos) {
        return false;
    }
    return true;
}

int getValue (string str, map <string,int> mp) {
    return mp.find(str)->second;
}

string getLine(vector<string> strs) {
    string result;
    ostringstream imploded;
    copy(strs.begin(), strs.end(),
               ostream_iterator<string>(imploded, " "));
    result = imploded.str();
    result.pop_back();
    return result;
}

bool isArg(string value, map<string,string> inds) {
    if (inds.find(value) == inds.end()) {
      return false;
    }
    return true;
}

string joinOps(vector<string> strs) {
    string result;
    ostringstream imploded;
    copy(strs.begin(), strs.end(),
               ostream_iterator<string>(imploded, ","));
    result = imploded.str();
    result.pop_back();

    return result;
}

bool inMACROS(string token, map <string,vector<vector<string>>> macros) {
    if (macros.find(token) == macros.end()) {
        return false;
    }
    return true;
}

int getValueMACRO(string token) {
    token.erase(0,1);
    return atoi(token.c_str());
}

//////////////////
/////////////////////////////
//////////////////////// COMEÇO TRAB 2

vector<string> cria_data_section(){
    vector<string> sec;
    sec.push_back("section .data\n");

    sec.push_back("msg_read_bytes       db     'Foram Lidos '");
    sec.push_back("size_read_bytes      EQU    $-msg_read_bytes \n");
    sec.push_back("msg_print_bytes      db     'Foram Escritos '");
    sec.push_back("size_print_bytes     EQU    $-msg_print_bytes\n");
    sec.push_back("msg_bytes            db     ' Bytes'");
    sec.push_back("size_bytes_msg       EQU    $-msg_bytes\n");
    sec.push_back("newl                 dd     0Ah");
    sec.push_back("n_1                  dd     '1'\n");

    return sec;
}


vector<string> cria_bss_section(){
    vector<string> sec;
    sec.push_back("section .bss\n");
    sec.push_back("size_of_s_input_output_int     resd 10 ; var auxiliar, para sabermos quantos bytes imprimir. INT");
    sec.push_back("size_of_s_input_output_str     resb 10; tamanho em bytes, só que em str, string vazia pra receber o valor. STR\n");
    sec.push_back("string_int 	resb 	max_int ; aux para com 'x'");
    sec.push_back("num_int 	resd 1 ; aux com x\n");


    return sec;
}



#endif // UTILS_H_INCLUDED
