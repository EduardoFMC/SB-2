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
//////////////////////// COME�O TRAB 2

vector<string> cria_data_section(){
    vector<string> sec;
    sec.push_back("section .data\n");

    return sec;
}


vector<string> cria_bss_section(){
    vector<string> sec;
    sec.push_back("section .bss\n");

    return sec;
}



#endif // UTILS_H_INCLUDED
