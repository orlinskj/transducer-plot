#ifndef _TRANSDUCER_H
#define _TRANSDUCER_H

#include <vector>
#include <string>
#include <map>
#include <utility>

#include "set.h"

namespace ac{

// Stores all measured set of values
class Transducer {

public:

    using Description = std::map<string, string>;

    Transducer(const string& name, const string& source,
            Description&& description, vector<Set>&& sets);

    const string& get_source() const;
    const string& get_name() const;

    const Description& get_description() const;
    const Set& get_set(Unit unit) const;

private:
    string m_name;
    string m_source;
    Description m_description;
    vector<Set> m_sets;
};

}   // namespace ac

#endif // _DATASET_H

