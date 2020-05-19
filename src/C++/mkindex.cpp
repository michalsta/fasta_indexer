#include <stdio.h>
#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "aainfo.cpp"


using namespace std;


double mass(uint32_t formula[])
{
    return 

    12.010824250306753 * formula[0] +
    1.007941468386931  * formula[1] + 
    14.00670519076357  * formula[2] + 
    15.999408717132628 * formula[3] +
    32.064887818622296 * formula[4];
}

int main(int argc, char** argv)
{
    vector<pair<double, uint64_t>> results;

    assert(argc==2);

    string infile(argv[1]);
    string idxfile = infile + ".idx";

    FILE* fasta = fopen(infile.c_str(), "r");



    char c;

    uint64_t file_pos = 0;
    uint64_t seq_start = -1;
    bool descr = false;
    bool correct = true;
    
    uint32_t formula[5];
    memset(formula, 0, sizeof(uint32_t)*5);

    while(true)
    {
        c = getc_unlocked(fasta);
        
        if(c == '\n')
            descr = false;
        else if(c == '>' or c == EOF)
        {
            descr = true;
            if(correct and seq_start >= 0 and mass(formula) > 0.0)
            {
                //printf("C%d H%d N%d O%d S%d %f\n", formula[0], formula[1], formula[2], formula[3], formula[4], mass(formula));
                results.push_back(make_pair(mass(formula), seq_start));
                //std::cout << mass(formula) << " " << file_pos << '\n';
            }
            correct = true;
            memset(formula, 0, sizeof(uint32_t)*5);
            seq_start = file_pos;
        }
        else if(!descr)
        {
            if(formulas[c] == nullptr)
            {
                correct = false;
            }
            else
            {
                uint32_t* cformula = formulas[c];
                for(int ii = 0; ii<5; ii++)
                    formula[ii] += cformula[ii];
            }
        }

        
        

        if(c == EOF)
            break;
        file_pos++;

//        if(file_pos % 100000000 == 0)
//            cerr << file_pos << endl;
    }
    fclose(fasta);

    sort(results.begin(), results.end());

    FILE* index = fopen(idxfile.c_str(), "w");

    for(int ii=0; ii<results.size(); ii++)
    {
        fwrite_unlocked(&results[ii].first, sizeof(double), 1, index);
        fwrite_unlocked(&results[ii].second, sizeof(uint64_t), 1, index);
    }

    fclose(index);
}

