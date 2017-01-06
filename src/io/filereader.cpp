#include "filereader.h"

#include <locale>

using namespace ac;

FileReader::~FileReader()
{

}

std::shared_ptr<Transducer> FileReader::read(const char* file_path, int *status)
{
    ifstream file_stream(file_path);

    locale l("");
    char decimal_point = use_facet< numpunct<char> >(l).decimal_point();

    if (!file_stream)
    {
        *status = -1;
        return nullptr;
    }

    string source = file_path;
    string name = path(file_path).stem().generic_string();

    std::vector<Unit> units;
    vector<vector<Set::value_type>> values;
    Transducer::Description description;

    string line_buffer = "";
    size_t pos;
    bool is_first_data_line_readed = false;

    while (getline(file_stream,line_buffer))
    {
        // If char ':' is inside the line - it's property for sure.
        pos = line_buffer.find(':');

        if (pos!=string::npos)
        {
            description.emplace(line_buffer.substr(0,pos),
                               line_buffer.substr(pos+1));
        }
        else
        {
            char delimiters[] = " \t";

            size_t token_pos = 0;
            size_t delim_pos = 0;
            string token;
            size_t token_index = 0;

            while (token_pos<line_buffer.length())
            {
                delim_pos = line_buffer.find_first_of(delimiters, token_pos);

                if (delim_pos==string::npos)
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
                    Unit u = Unit::get_unit_by_symbol(std::string(token));
                    if (u == Unit::None)
                        u = Unit("",std::string(token),"");

                    units.push_back(u);
                    values.push_back( vector<Set::value_type>(0) );

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

    vector<Set> sets;

    auto vit = values.begin();
    for (auto uit=units.begin(); uit!=units.end() && vit!=values.end(); uit++, vit++)
    {
        sets.push_back(Set(std::move(*vit), *uit));
    }

    return std::make_shared<Transducer>(
                Transducer(name,source,std::move(description),std::move(sets)));
}
