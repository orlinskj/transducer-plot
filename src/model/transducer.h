#ifndef _TRANSDUCER_H
#define _TRANSDUCER_H

#include <vector>
#include <string>
#include <map>
#include <memory>

#include <QMetaType>

#include "set.h"

// namespace ac{

/**
 * @brief The Transducer class describes the measured transducer.
 * It contains all measured and calculated values for transducer.
 */
class Transducer {
public:

    using Description = std::map<std::string, std::string>;

    Transducer() = default;
    Transducer(const std::string& name, const std::string& source,
            Description&& description, std::vector<Set>&& sets);

    const std::string& get_source() const;
    const std::string& get_name() const;

    const Description& get_description() const;
    const Set* get_set(const Unit &unit) const;
    std::vector<Set>& get_sets();

    operator const std::string&() const;

private:
    std::string name_;
    std::string source_;
    Description description_;
    std::vector<Set> sets_;
};

using Transducer_ptr = std::shared_ptr<Transducer>;

// }   // namespace ac

Q_DECLARE_METATYPE(Transducer_ptr)

#endif // _DATASET_H

