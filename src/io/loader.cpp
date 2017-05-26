#include "loader.h"

#include <locale>
#include <regex>
#include <QtCore>
#include <cmath>
#include "../error.h"
#include <boost/algorithm/string.hpp>

void complete_sets(std::vector<Set>& sets)
{
    auto find_set = [&sets](const Unit& u){
        return std::find_if(sets.cbegin(),sets.cend(), [u](const Set& s){
            return s.unit() == u;
        });
    };

    auto valid = [&sets](const std::vector<Set>::const_iterator& it){
        return it != sets.cend();
    };

    auto voltage_it = find_set(Unit::Voltage);
    auto current_it = find_set(Unit::Current);
    auto impedance_it = find_set(Unit::Impedance);
    auto imp_real_it = find_set(Unit::ImpedanceReal);
    auto imp_imag_it = find_set(Unit::ImpedanceImag);
    auto admitance_it = find_set(Unit::Admitance);
    auto phase_angle_it = find_set(Unit::PhaseAngle);

    // impedance
    if (!valid(impedance_it)){
        qDebug() << QObject::tr("Impedance set") << " " << QObject::tr("not found") << ". ";
        if (valid(voltage_it) && valid(current_it)){
            auto U = voltage_it->values().cbegin();
            auto I = current_it->values().cbegin();

            sets.push_back(Set(Unit::Impedance));
            auto& set = *(sets.rbegin());
            impedance_it = sets.end() - 1;

            while (U != voltage_it->values().cend() && I != current_it->values().cend()){
                auto val = (*U) / (*I);
                set.push(val);
                ++U;
                ++I;
            }

            qInfo() << "  " << QObject::tr("calculated successfully from")
                     << " " << QObject::tr("voltage") << "/" << QObject::tr("current");
        }
        else{
            qInfo() << "  " << QObject::tr("couldn't calculate it - voltage or current set not present");
        }
    }

    // impedance real part and imag part
    if (!valid(imp_real_it)){
        qDebug() << QObject::tr("Impedance (real part) set") << " " << QObject::tr("not found") << ". ";
        if (valid(phase_angle_it) && valid(impedance_it)){
            auto Z = impedance_it->values().cbegin();
            auto Th = phase_angle_it->values().cbegin();

            sets.push_back(Set(Unit::ImpedanceReal));
            auto& set = *(sets.rbegin());
            impedance_it = sets.end() - 1;

            while (Z != impedance_it->values().cend() && Th != phase_angle_it->values().cend()){
                auto val = (*Z) * std::cos(*Th);
                set.push(val);
                ++Z;
                ++Th;
            }

            qInfo() << "  " << QObject::tr("calculated successfully from")
                     << " cosinus(" << QObject::tr("impedance") << ") * " << QObject::tr("phase angle");
        }
        else{
            qInfo() << "  " << QObject::tr("couldn't calculate it - voltage or current set not present");
        }
    }
    if (!valid(imp_imag_it)){
        qDebug() << QObject::tr("Impedance (imaginary part) set") << " " << QObject::tr("not found") << ". ";
        if (valid(phase_angle_it) && valid(impedance_it)){
            auto Z = impedance_it->values().cbegin();
            auto Th = phase_angle_it->values().cbegin();

            sets.push_back(Set(Unit::ImpedanceImag));
            auto& set = *(sets.rbegin());
            impedance_it = sets.end() - 1;

            while (Z != impedance_it->values().cend() && Th != phase_angle_it->values().cend()){
                auto val = (*Z) * std::sin(*Th);
                set.push(val);
                ++Z;
                ++Th;
            }

            qInfo() << "  " << QObject::tr("calculated successfully from")
                     << " sinus(" << QObject::tr("impedance") << ") * " << QObject::tr("phase angle");
        }
        else{
            qInfo() << "  " << QObject::tr("couldn't calculate it - voltage or current set not present");
        }
    }

    // admitance
    if (!valid(admitance_it)){
        qInfo() << QObject::tr("Admitance set") << " " << QObject::tr("not found") << ". ";
        if (valid(impedance_it)){
            auto Z = impedance_it->values().cbegin();

            sets.push_back(Set(Unit::Admitance));
            auto& set = *(sets.rbegin());
            admitance_it = sets.end() - 1;

            while(Z != impedance_it->values().cend()){
                auto val = 1.0 / (*Z);
                set.push(val);
                ++Z;
            }

            qInfo() << "  " << QObject::tr("calculated successfully from") << " " << QObject::tr("impedance");
        }
        else{
            qInfo() << "  " << QObject::tr("couldn't calculate it - impedance set not present");
        }
    }
}

Transducer* Loader::load(const std::string& file_path)
{
    std::ifstream stream(file_path.c_str());

    std::locale l("");
    char decimal_point = std::use_facet<std::numpunct<char> >(l).decimal_point();

    if (!stream)
        return nullptr;

    std::string source = file_path;
    std::string name = path(file_path).stem().generic_string();

    std::vector<Unit> units;
    std::vector<std::vector<Set::value_type>> values;
    Transducer::Description description;

    std::string line_buffer = "";
    size_t pos;
    bool is_first_data_line_readed = false;

    while (getline(stream,line_buffer))
    {
        // If char ':' is inside the line - it's property for sure.
        pos = line_buffer.find(':');

        if (pos != std::string::npos)
        {
            description.emplace(line_buffer.substr(0,pos),
                               line_buffer.substr(pos+1));
        }
        else
        {
            char delimiters[] = " \t";

            size_t token_pos = 0;
            size_t delim_pos = 0;
            std::string token;
            size_t token_index = 0;

            while (token_pos<line_buffer.length())
            {
                delim_pos = line_buffer.find_first_of(delimiters, token_pos);

                if (delim_pos == std::string::npos)
                {
                    token = line_buffer.substr(token_pos);
                    token_pos = line_buffer.length();

                    // Trim whitespaces from both sides.
                    // It helps to avoid \r char at the end of string
                    // left by getline() on windows (CRLF!)
                    trim(token);

                    if(token.empty())
                    {
                        break;
                    }
                }
                else
                {
                    token = line_buffer.substr(token_pos, delim_pos-token_pos);
                    token_pos = delim_pos + 1;
                }

                if( !is_first_data_line_readed)
                {
                    Unit u = Unit::from_symbol(std::string(token));

                    units.push_back(u);
                    values.push_back( std::vector<Set::value_type>(0) );

                    if (token_pos >= line_buffer.length())
                    {
                        is_first_data_line_readed = true;
                    }
                }
                else
                {
                    if (decimal_point == '.')
                        replace( token.begin(), token.end(), ',', '.');
                    else
                        replace( token.begin(), token.end(), '.', ',');

                    if (token_index < values.size())
                    {
                        double value = stod(token);
                        values[token_index].push_back( value );
                    }
                }

                token_index++;
            }
        }
    }

    std::vector<Set> sets;

    auto vit = values.begin();
    for (auto uit=units.begin(); uit!=units.end() && vit!=values.end(); uit++, vit++)
    {
        sets.push_back(Set(std::move(*vit), *uit));
    }

    complete_sets(sets);

    return new Transducer(name,source,std::move(description),std::move(sets));
}
