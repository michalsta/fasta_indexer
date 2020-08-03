#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cerrno>
#include <exception>
#include "aainfo.cpp"




double mass(uint32_t formula[])
{
    return 

    12.010824250306753 * formula[0] +
    1.007941468386931  * formula[1] + 
    14.00670519076357  * formula[2] + 
    15.999408717132628 * formula[3] +
    32.064887818622296 * formula[4];
}

void handle_sys_error(bool errored)
{
    if(errored)
        throw std::system_error(errno, std::generic_category());
}


int real_main(int argc, char** argv)
{
    std::vector<std::pair<double, uint64_t>> results;

    assert(argc==2);

    std::string infile(argv[1]);
    std::string idxfile = infile + ".idx";

    std::ios_base::sync_with_stdio(false);

    std::ifstream fasta(infile);
    fasta.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    char c;

    uint64_t file_pos = 0;
    uint64_t seq_start = -1;
    bool descr = false;
    bool correct = true;
    
    uint32_t formula[5];
    memset(formula, 0, sizeof(uint32_t)*5);

    while(true)
    {
        fasta >> c;
        
        if(c == '\n')
            descr = false;
        else if(c == '>' or c == EOF)
        {
            descr = true;
            if(correct and seq_start >= 0 and mass(formula) > 0.0)
            {
                //printf("C%d H%d N%d O%d S%d %f\n", formula[0], formula[1], formula[2], formula[3], formula[4], mass(formula));
                results.push_back(std::make_pair(mass(formula), seq_start));
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

    fasta.close();

    sort(results.begin(), results.end());

    std::ofstream index;
    index.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    index.open(idxfile, std::ios::in | std::ios::binary | std::ios::trunc);

    for(int ii=0; ii<results.size(); ii++)
    {
        index.write(reinterpret_cast<char*>(&results[ii].first),  sizeof(double));
        index.write(reinterpret_cast<char*>(&results[ii].second), sizeof(uint64_t));
    }

    index.close();

    return 0;
}


int main(int argc, char** argv)
{
    try
    {
        return real_main(argc, argv);
    }
    catch(const std::system_error& e)
    {
        perror(nullptr);
        throw;
    }
}
