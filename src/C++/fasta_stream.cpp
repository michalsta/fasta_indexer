#include <fstream>
#include <exception>

#include "aainfo.cpp"

class Protein
{
    std::string name;
    std::string contents;
    double _mass;

 public:
    Protein() {};
    void set(std::string&& _name, std::string&& _contents) { name = std::move(_name); contents = std::move(_contents); _mass = -1.0; };
    bool is_valid() const;
    std::string fasta() const;
    double mass();
};

class FASTA_Stream
{
    std::ifstream file;
    std::streampos _offset;
    Protein p;

 public:
    FASTA_Stream(const std::string& filename);
    bool next();
    bool next_valid();
    Protein& get() { return p; };
    std::streampos offset() const { return _offset; };
};

FASTA_Stream::FASTA_Stream(const std::string& filename)
{
    file.exceptions(std::ifstream::badbit);
    file.open(filename);
    assert(file.peek() == '>');
}

bool FASTA_Stream::next()
{
    file.peek();

    if(file.eof())
        return false;

    _offset = file.tellg();

    std::string name, contents;

    assert(file.peek() == '>');
    std::getline(file, name);

    contents.clear();

    std::string tmp;

    while(file.peek() != '>' && !file.eof())
    {
        std::getline(file, tmp);
        contents += tmp;
        contents += '\n';
    }

    p.set(std::move(name), std::move(contents));

    return true;
}

bool Protein::is_valid() const
{
    for(const unsigned char& c : contents)
        if(c != '\n' and formulas[c] == nullptr)
            return false;
    return true;
}

bool FASTA_Stream::next_valid()
{
    if(!next())
        return false;
    while(!p.is_valid())
        if(!next())
            return false;
    return true;
}

double Protein::mass()
{
    if(_mass >= 0.0)
        return _mass;

    _mass = 0.0;

    for(const unsigned char& c : contents)
        _mass += masses[c];

    return _mass;
}

std::string Protein::fasta() const
{
    return name + '\n' + contents;
}
