#include <iostream>
#include <fstream>
#include <sstream>
#include "../estruturas/estruturaBlocoRegistro.hpp"
#include "../hash/hashTable.cpp"
#include "../bptree/bptreeSerializada.cpp"
#include "../bptree/bptreeStringSerializada.cpp"

#ifndef PARSER_HPP
#define PARSER_HPP

string remover_aspas_input(const std::string& input) {
    string result = "";
    
    for (size_t i = 0; i < input.length(); ++i) {
        char ch = input[i];

        if (ch == '"' && i + 1 < input.length() && input[i + 1] == ';') {
            break;
        }
        result += ch;
    }

    return result;
}

void remover_aspas(std::istream& stream, std::string& result)
{
    result.clear();
    char ch;

    while (stream.get(ch))
    {
        if (ch == '"' && stream.peek() == ';')
        {
            stream.get();
            break;
        }
        result += ch;
    }
}

std::string normalizar_string(std::string str)
{
    for (char &c : str)
    {
        if (static_cast<unsigned char>(c) > 127 || c == '\n' || c == '\"')
        {
            c = ' ';
        }
    }

    std::string result;
    bool inSpace = false;
    for (char c : str) {
        if (std::isspace(c))
        {
            if (!inSpace)
            {
                result += ' ';
                inSpace = true;
            }
        } else
        {
            result += c;
            inSpace = false;
        }
    }

    size_t start = result.find_first_not_of(' ');
    if (start == std::string::npos) return ""; // String é apenas espaços
    size_t end = result.find_last_not_of(' ');

    return result.substr(start, end - start + 1);
}

#endif