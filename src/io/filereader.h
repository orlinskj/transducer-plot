#ifndef _FILEREADER_H
#define _FILEREADER_H

//#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <memory>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>

#include "../model/transducer.h"

namespace ac {

    using namespace std;
    using namespace boost::filesystem;
    using namespace boost::algorithm;

    // Reads data from file into Transducer instance.
    class FileReader{

    public:
        virtual ~FileReader();

        virtual std::shared_ptr<Transducer> read(const char* filepath, int* status);
    };

}

#endif // _FILEREADER_H

