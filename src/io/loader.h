#ifndef _LOADER_H
#define _LOADER_H

#include <string>
#include <functional> // trim
#include <algorithm> // trim
#include <cctype> // trim
#include "../model/transducer.h"

namespace strtrim{
    // trim from start
    inline std::string& ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    // trim from end
    inline std::string& rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    // trim from both ends
    inline std::string& trim(std::string &s) {
        return ltrim(rtrim(s));
    }
}

/**
 * @brief The Loader class
 * @desc Reads data from file into Transudcer instance.
 */
class Loader{
    struct dot_sep : std::numpunct<char> {
    protected:
        virtual char_type do_decimal_point() const;
    };

public:
    // load a transducer - returns new instance on heap
    // throws Error
    virtual Transducer* load(const std::string& path);

};


#endif // _LOADER_H

