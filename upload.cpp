#include <iostream>
#include <fstream>
#include <sstream>
#include "estruturas/estruturaBlocoRegistro.hpp"
#include "hash/hashTable.cpp"
#include "bptree/bptreeSerializada.cpp"

using namespace std;

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


void ler_arquivo_csv(const string& nome_arquivo, hashTable& hash, BPlusTree& arvore1, BPlusTree& arvore2)
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
            remover_aspas(linha_analisada, dado);
            id = stoi(dado);

            getline(linha_analisada, dado, '"');
            remover_aspas(linha_analisada, dado);
            title = dado;

            getline(linha_analisada, dado, '"');
            remover_aspas(linha_analisada, dado);
            year = stoi(dado);

            getline(linha_analisada, dado, '"');
            remover_aspas(linha_analisada, dado);
            authors = dado;

            getline(linha_analisada, dado, '"');
            remover_aspas(linha_analisada, dado);
            citations = stoi(dado);

            getline(linha_analisada, dado, '"');
            remover_aspas(linha_analisada, dado);
            update = dado;

            getline(linha_analisada, dado, '"');
            remover_aspas(linha_analisada, dado);
            snippet = dado;

            title = normalizar_string(title);
            authors = normalizar_string(authors);
            update = normalizar_string(update);
            snippet = normalizar_string(snippet);

            Registro* novo_registro = criar_registro(id, title, year, authors, citations, update, snippet);
            int posicao = hash.inserirRegistroBucket(novo_registro);

            arvore1.inserir_arvore(new RegistroBPT(id, posicao));
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
    const string nome_arquivo = argv[1];
    const int altura_arvore = 3;
    const string nome_arquivo_hash_table= "hashtable_dados.bin";
    const string nome_arquivo_arvore_primaria= "arvore_primaria_dados.bin";

    hashTable hash_table = hashTable(nome_arquivo_hash_table);
    BPlusTree arvore_primaria(altura_arvore);
    BPlusTree arvore_secundaria(altura_arvore);

    ler_arquivo_csv(nome_arquivo, hash_table, arvore_primaria, arvore_secundaria);

    arvore_primaria.serializeBPlusTree(arvore_primaria, nome_arquivo_arvore_primaria);
    
    BPlusTree novaArvore = novaArvore.deserializeBPlusTree(nome_arquivo_arvore_primaria);
    
    No<RegistroBPT>* reg_aux = novaArvore.busca_BPlusTree(novaArvore.raiz, std::atoi(argv[2]));
    for (int i = 0; i < reg_aux->ocupacao ; i++) {
        if (reg_aux->registros[i].chave == std::atoi(argv[2])) {
            //novaArvore.imprime_registro(reg_aux->registros[i]);
            break;
        }
    }

    imprimir_registro(novaArvore.buscar_registro_bpt(hash_table.arquivoIn, std::atoi(argv[2])));

    return 0;
}