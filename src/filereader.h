#ifndef _FILEREADER_H
#define _FILEREADER_H

//#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <locale>
#include <algorithm>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>

#include "dataset.h"

namespace ac {

    using namespace std;
    using namespace boost::filesystem;
    using namespace boost::algorithm;

    // Reads data from file into DataSet instance.
    class FileReader{

    public:
        virtual ~FileReader();

        virtual unique_ptr<DataSet> read(const char* filepath, int* status);
    };

}

#endif // _FILEREADER_H

