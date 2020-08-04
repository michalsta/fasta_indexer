#include <cassert>
#include <exception>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cerrno>
#include "aainfo.cpp"





int real_main(int argc, char** argv)
{
    std::vector<std::pair<double, std::string>> results;

    assert(argc==2);

    std::string infile(argv[1]);
    std::string idxfile = infile + ".sorted";

    std::ios_base::sync_with_stdio(false);

    std::ifstream in;
    in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    in.open(infile);

    char c;

    uint64_t file_pos = 0;
    uint64_t seq_start = -1;
    bool descr = false;
    bool correct = true;
    
    uint32_t formula[5];

    std::string fasta;
    std::string tmp;

    assert(in.peek() == '>');

    bool first = false;

    while(std::getline(in, tmp))
    {
        if(tmp[0] == '>')
        {
            if(!first && correct)
                results.push_back(make_pair(mass(formula), fasta));

            memset(formula, 0, sizeof(uint32_t)*5);
            fasta = tmp;
            fasta += '\n';
            correct = true;
            first = false;
        }
        else
        {
            fasta += tmp;
            fasta += '\n';
            for(size_t ii = 0; ii<tmp.size(); ii++)
            {
                uint32_t* cformula = formulas[tmp[ii]];
                if(cformula == nullptr)
                    correct = false;
                else
                    for(int ii = 0; ii<5; ii++)
                        formula[ii] += cformula[ii];
            }
        }
    }
    /*
    while(not in.eof())
    {
        memset(formula, 0, sizeof(uint32_t)*5);
        correct = true;
        fasta = "";
        std::getline(in, fasta);
        fasta += '\n';
        while(not in.eof() and in.peek() != '>')
        {
            tmp = "";
            std::getline(in, tmp);
            fasta += tmp;
            fasta += '\n';
            for(size_t ii = 0; ii<tmp.size(); ii++)
            {
                uint32_t* cformula = formulas[tmp[ii]];
                if(cformula == nullptr)
                    correct = false;
                else
                    for(int ii = 0; ii<5; ii++)
                        formula[ii] += cformula[ii];
            }
        }

        if(correct)
            results.push_back(make_pair(mass(formula), fasta));
    }*/
     /*   
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
*/
    sort(results.begin(), results.end());


    std::ofstream index;
    index.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    index.open(idxfile, std::ios::out | std::ios::trunc);

    for(int ii=0; ii<results.size(); ii++)
    {
        index << results[ii].second;
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
    catch (const std::system_error& e) {
        perror(nullptr);
        throw;
    }
}

