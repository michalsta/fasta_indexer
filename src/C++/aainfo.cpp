#include <cstdint>
#include <cstring>



uint32_t** setup_formulas()
{
    uint32_t** ret = new uint32_t*[256];
    memset(ret, 0, 256*sizeof(uint32_t));

    ret['A'] = new uint32_t[5];
    ret['C'] = new uint32_t[5];
    ret['E'] = new uint32_t[5];
    ret['D'] = new uint32_t[5];
    ret['G'] = new uint32_t[5];
    ret['F'] = new uint32_t[5];
    ret['I'] = new uint32_t[5];
    ret['J'] = new uint32_t[5];
    ret['H'] = new uint32_t[5];
    ret['K'] = new uint32_t[5];
    ret['M'] = new uint32_t[5];
    ret['L'] = new uint32_t[5];
    ret['N'] = new uint32_t[5];
    ret['O'] = new uint32_t[5];
    ret['Q'] = new uint32_t[5];
    ret['P'] = new uint32_t[5];
    ret['S'] = new uint32_t[5];
    ret['R'] = new uint32_t[5];
    ret['T'] = new uint32_t[5];
    ret['W'] = new uint32_t[5];
    ret['V'] = new uint32_t[5];
    ret['Y'] = new uint32_t[5];

    ret['A'][1] = 5;
    ret['A'][0] = 3;
    ret['A'][3] = 1;
    ret['A'][2] = 1;
    ret['C'][1] = 5;
    ret['C'][0] = 3;
    ret['C'][4] = 1;
    ret['C'][3] = 1;
    ret['C'][2] = 1;
    ret['E'][1] = 7;
    ret['E'][0] = 5;
    ret['E'][3] = 3;
    ret['E'][2] = 1;
    ret['D'][1] = 5;
    ret['D'][0] = 4;
    ret['D'][3] = 3;
    ret['D'][2] = 1;
    ret['G'][1] = 3;
    ret['G'][0] = 2;
    ret['G'][3] = 1;
    ret['G'][2] = 1;
    ret['F'][1] = 9;
    ret['F'][0] = 9;
    ret['F'][3] = 1;
    ret['F'][2] = 1;
    ret['I'][1] = 11;
    ret['I'][0] = 6;
    ret['I'][3] = 1;
    ret['I'][2] = 1;
    ret['J'][1] = 11;
    ret['J'][0] = 6;
    ret['J'][3] = 1;
    ret['J'][2] = 1;
    ret['H'][1] = 7;
    ret['H'][0] = 6;
    ret['H'][2] = 3;
    ret['H'][3] = 1;
    ret['K'][1] = 12;
    ret['K'][0] = 6;
    ret['K'][2] = 2;
    ret['K'][3] = 1;
    ret['M'][1] = 9;
    ret['M'][0] = 5;
    ret['M'][4] = 1;
    ret['M'][3] = 1;
    ret['M'][2] = 1;
    ret['L'][1] = 11;
    ret['L'][0] = 6;
    ret['L'][3] = 1;
    ret['L'][2] = 1;
    ret['N'][1] = 6;
    ret['N'][0] = 4;
    ret['N'][3] = 2;
    ret['N'][2] = 2;
    ret['O'][1] = 19;
    ret['O'][0] = 12;
    ret['O'][3] = 2;
    ret['O'][2] = 3;
    ret['Q'][1] = 8;
    ret['Q'][0] = 5;
    ret['Q'][3] = 2;
    ret['Q'][2] = 2;
    ret['P'][0] = 5;
    ret['P'][3] = 1;
    ret['P'][1] = 7;
    ret['P'][2] = 1;
    ret['S'][1] = 5;
    ret['S'][0] = 3;
    ret['S'][3] = 2;
    ret['S'][2] = 1;
    ret['R'][1] = 12;
    ret['R'][0] = 6;
    ret['R'][2] = 4;
    ret['R'][3] = 1;
    ret['T'][1] = 7;
    ret['T'][0] = 4;
    ret['T'][3] = 2;
    ret['T'][2] = 1;
    ret['W'][1] = 10;
    ret['W'][0] = 11;
    ret['W'][2] = 2;
    ret['W'][3] = 1;
    ret['V'][1] = 9;
    ret['V'][0] = 5;
    ret['V'][3] = 1;
    ret['V'][2] = 1;
    ret['Y'][1] = 9;
    ret['Y'][0] = 9;
    ret['Y'][3] = 2;
    ret['Y'][2] = 1;

    return ret;
}

uint32_t** formulas = setup_formulas();


