#include <iostream>
#include <fstream>
#include <sstream>
#include "./estruturaBlocoRegistro.hpp"
#include "../hash/hashTable.cpp"

using namespace std;

void getUntilLastQuote(std::istream& stream, std::string& result)
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


void ler_arquivo_csv(const string& nome_arquivo, hashTable& hash)
{
    ifstream arquivo(nome_arquivo, ios::in);
    if (!arquivo.is_open())
    {
        cerr << "Nao foi possivel abrir o arquivo: " << nome_arquivo << "\n";
        return;
    }

    string linha;
    while (getline(arquivo, linha))
    {
        stringstream linha_analisada(linha);
        string dado;

        int id, year, citations;
        string title, authors, update, snippet;

        try
        {
            getline(linha_analisada, dado, '"');
            getUntilLastQuote(linha_analisada, dado);
            id = stoi(dado);

            getline(linha_analisada, dado, '"');
            getUntilLastQuote(linha_analisada, dado);
            title = dado;

            getline(linha_analisada, dado, '"');
            getUntilLastQuote(linha_analisada, dado);
            year = stoi(dado);

            getline(linha_analisada, dado, '"');
            getUntilLastQuote(linha_analisada, dado);
            authors = dado;

            getline(linha_analisada, dado, '"');
            getUntilLastQuote(linha_analisada, dado);
            citations = stoi(dado);

            getline(linha_analisada, dado, '"');
            getUntilLastQuote(linha_analisada, dado);
            update = dado;

            getline(linha_analisada, dado, '"');
            getUntilLastQuote(linha_analisada, dado);
            snippet = dado;

            title = normalizar_string(title);
            authors = normalizar_string(authors);
            update = normalizar_string(update);
            snippet = normalizar_string(snippet);

            Registro* novo_registro = criar_registro(id, title, year, authors, citations, update, snippet);
            hash.inserirRegistroBucket(novo_registro);
        } catch (const std::exception& e)
        {
            std::cerr << "Caught an exception: " << e.what() << " " << id << std::endl;
        }
    }
    arquivo.close();

    return;
}

int main(int argc, char const *argv[])
{
    // UPLOAD
    const string nome_arquivo = argv[2];
    hashTable Hash = hashTable("arquivoDados.bin");

    cout << "Iniciando leitura csv" << endl;

    ler_arquivo_csv(nome_arquivo, Hash);

    cout << "Fim de leitura csv" << endl;

    cout << "Iniciando leitura arquivo binario" << endl;

    cout << "Fim de leitura arquivo binario" << endl;
    // FIM UPLOAD

    imprimir_registro(Hash.busca_registro_hashtable(std::atoi(argv[1])));

    return 0;
}