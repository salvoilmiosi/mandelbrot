#include "globalvars.h"

#include <fstream>

#undef VAR
#define VAR(type, name, default) type name = default;
GLOBAL_VARS

void readOptions(const std::string &filename) {
    std::ifstream ifs(filename);
    std::string str;

    std::getline(ifs, str);
    if (str != FIRST_LINE) return;
    std::getline(ifs, str);

    while(ifs.good() && ! ifs.eof()) {
        ifs >> str;
#undef VAR
#define VAR(type,name,default) if (str == #name) {ifs >> name; continue;}
        GLOBAL_VARS
    }
}

void saveOptions(const std::string &filename) {
    std::ofstream ofs(filename, std::ofstream::trunc|std::ofstream::out);

    ofs << FIRST_LINE << "\n\n";
#undef VAR
#define VAR(type,name,default) ofs << #name << ' ' << name << '\n';
    GLOBAL_VARS
}
