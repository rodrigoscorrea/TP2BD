#include <iostream>
#include <fstream>
#include <sstream>
#include "../estruturas/estruturaBlocoRegistro.hpp"
#include "../hash/hashTable.cpp"
#include "../bptree/bptreeSerializada.cpp"
#include "../bptree/bptreeStringSerializada.cpp"

#ifndef PARSER_HPP
#define PARSER_HPP

string remover_aspas_input(const std::string& entrada) {
    string resultado = "";
    
    for (size_t i = 0; i < entrada.length(); ++i) {
        char ch = entrada[i];

        if (ch == '"' && i + 1 < entrada.length() && entrada[i + 1] == ';') {
            break;
        }
        resultado += ch;
    }

    return resultado;
}

void remover_aspas(std::istream& stream, std::string& resultado)
{
    resultado.clear();
    char ch;

    while (stream.get(ch))
    {
        if (ch == '"' && stream.peek() == ';')
        {
            stream.get();
            break;
        }
        resultado += ch;
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