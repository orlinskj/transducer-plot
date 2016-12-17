#ifndef _DATASET_H
#define _DATASET_H

#include <vector>
#include <string>
#include <map>
#include <utility>

#include "series.h"

namespace ac{

using namespace std;

// Stores all measured series of values
class DataSet {

public:

    using Description = map<string, string>;

    DataSet(const string& name, const string& source,
            Description&& description, vector<Series>&& series);

    const string& getSource() const;
    const string& getName() const;

    const Description& getDescription() const;
    const vector<Series>& getSeries() const;

private:
    string name;
    string source;
    Description description;
    vector<Series> series;
};

}   // namespace ac

#endif // _DATASET_H

