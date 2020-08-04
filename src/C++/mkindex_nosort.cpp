#include <vector>
#include <algorithm>
#include <cassert>
#include <fstream>

#include "fasta_stream.cpp"




std::ofstream open_outf(const std::string& filename, std::ios_base::openmode add_mode = std::ios_base::openmode())
{
    std::ofstream res;
    res.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    res.open(filename, std::ofstream::out | std::ofstream::trunc | add_mode);
    return res;
}

int real_main(int argc, char** argv)
{
    std::vector<std::pair<double, uint64_t> > locations;

    assert(argc==2);

    std::string infile(argv[1]);
    std::string sorted_filename = infile + ".sorted";
    std::string index_filename = sorted_filename + ".idx";

    std::ios_base::sync_with_stdio(false);

    FASTA_Stream fasta(infile);

    while(fasta.next_valid())
        locations.push_back(std::make_pair<double, uint64_t>(fasta.get().mass(), static_cast<uint64_t>(fasta.offset())));

    std::sort(locations.begin(),
              locations.end(),
              [](auto& a, auto& b) { return a.first < b.first; }
             );

    std::ofstream index = open_outf(index_filename, std::ofstream::binary);

    for(auto [mass, offset] : locations)
    {
        static_assert(std::is_same<decltype(offset), uint64_t>::value);
        index.write(reinterpret_cast<char*>(&mass), sizeof(double));
        index.write(reinterpret_cast<char*>(&offset), sizeof(uint64_t));
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
