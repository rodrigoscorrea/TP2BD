#include <iostream>
#include <fstream>
#include <sstream>
#include "./estruturaBlocoRegistro.hpp"
#include "../hash/hashTable.cpp"

using namespace std;

std::string normalizar_string(std::string str) {
    for (char &c : str) {
        if (static_cast<unsigned char>(c) > 127) {
            c = ' ';
        }
    }

    std::string result;
    bool inSpace = false;
    for (char c : str) {
        if (std::isspace(c)) {
            if (!inSpace) {
                result += ' ';
                inSpace = true;
            }
        } else {
            result += c;
            inSpace = false;
        }
    }

    size_t start = result.find_first_not_of(' ');
    if (start == std::string::npos) return ""; // String é apenas espaços
    size_t end = result.find_last_not_of(' ');

    return result.substr(start, end - start + 1);
}


vector<Registro*> ler_arquivo_csv(const string& nome_arquivo)
{
    vector<Registro*> registros;

    ifstream arquivo(nome_arquivo, ios::in);
    if (!arquivo.is_open())
    {
        cerr << "Nao foi possivel abrir o arquivo: " << nome_arquivo << "\n";
        return registros;
    }

    string linha;
    while (getline(arquivo, linha))
    {
        stringstream linha_analisada(linha);
        string dado;

        int id, year, citations;
        string title, authors, update, snippet;

        // Cada getline para cada aspas
        getline(linha_analisada, dado, '"');  
        getline(linha_analisada, dado, '"');
        id = stoi(dado);

        getline(linha_analisada, dado, '"');
        getline(linha_analisada, dado, '"');
        title = dado;

        getline(linha_analisada, dado, '"');
        getline(linha_analisada, dado, '"');
        year = stoi(dado);

        getline(linha_analisada, dado, '"');
        getline(linha_analisada, dado, '"');
        authors = dado;

        getline(linha_analisada, dado, '"');
        getline(linha_analisada, dado, '"');
        citations = stoi(dado);

        getline(linha_analisada, dado, '"');
        getline(linha_analisada, dado, '"');
        update = dado;

        getline(linha_analisada, dado, '"');
        getline(linha_analisada, dado, '"');
        snippet = dado;

        title = normalizar_string(title);
        authors = normalizar_string(authors);
        update = normalizar_string(update);
        snippet = normalizar_string(snippet);

        Registro* novo_registro = criar_registro(id, title, year, authors, citations, update, snippet);
        registros.push_back(novo_registro);
    }
    arquivo.close();

    return registros;
}

int main(int argc, char const *argv[])
{
    // UPLOAD
    const string nome_arquivo = "../teste1.csv";
    vector<Registro*> registros = ler_arquivo_csv(nome_arquivo);
    hashTable Hash = hashTable("arquivoDados.bin");

    for (const auto& registro : registros) {
        Hash.inserirRegistroBucket(registro);
    }
    // FIM UPLOAD

    imprimir_registro(Hash.busca_registro_hashtable(std::atoi(argv[1])));

    for (auto& registro : registros)
    {
        delete registro;
    } 

    return 0;
}