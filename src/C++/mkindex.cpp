#include <vector>
#include <algorithm>
#include <cassert>
#include <fstream>

#include "fasta_stream.hpp"




std::ofstream open_outf(const std::string& filename, std::ios_base::openmode add_mode = std::ios_base::openmode())
{
    std::ofstream res;
    res.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    res.open(filename, std::ofstream::out | std::ofstream::trunc | add_mode);
    return res;
}

int mkindex_real_main(const char* path)
{
    std::vector<Protein> proteins;

    std::string infile(path);
    std::string sorted_filename = infile + ".sorted";
    std::string index_filename = sorted_filename + ".idx";

    std::ios_base::sync_with_stdio(false);

    FASTA_Stream fasta(infile);

    while(fasta.next_valid())
        proteins.emplace_back(std::move(fasta.get()));

    std::sort(proteins.begin(),
              proteins.end(),
              [](Protein& a, Protein& b) { return a.mass() < b.mass(); }
             );

    std::ofstream sorted_fasta = open_outf(sorted_filename);
    std::ofstream index = open_outf(index_filename, std::ofstream::binary);

    uint64_t offset = 0;

    for(Protein& protein : proteins)
    {
        std::string fasta = protein.fasta();
        sorted_fasta << fasta;
        double mass = protein.mass();
        index.write(reinterpret_cast<char*>(&mass), sizeof(double));
        index.write(reinterpret_cast<char*>(&offset), sizeof(uint64_t));
        offset += fasta.size();
    }

    sorted_fasta.close();
    index.close();

    return 0;
}


int mkindex_main(const char* path)
{
    try
    {
        return mkindex_real_main(path);
    }
    catch (const std::system_error& e)
    {
        perror(nullptr);
        throw;
    }
}


#ifndef FASTA_INDEXER_OMIT_MAIN
int main(int argc, char** argv)
{
    assert(argc == 2);
    return mkindex_main(argv[1]);
}
#endif
