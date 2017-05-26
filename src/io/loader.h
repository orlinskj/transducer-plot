#ifndef _LOADER_H
#define _LOADER_H

//#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <memory>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>

#include "../model/transducer.h"

using namespace boost::filesystem;
using namespace boost::algorithm;

// Reads data from file into Transducer instance.
class Loader{
public:
    // load a transducer - returns new instance on heap
    virtual Transducer* load(const std::string& path);
};


#endif // _LOADER_H

