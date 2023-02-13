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

    vector<vector<string>> programa = tokenParser("equtest.asm");

    preprocess(programa);

    montador(programa);

    return 0;
}
