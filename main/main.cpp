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
    std::string arquivo = argv[1];
    std::string file_name = arquivo.substr(0, arquivo.find_last_of('.'));

    vector<vector<string>> programa = tokenParser(arquivo);

    preprocess(programa);

    montador(programa, file_name);

    return 0;
}
