#include <iostream>
#include <fstream>
#include <sstream>
#include "estruturas/definicoes.hpp"
#include "estruturas/parser.cpp"
#include "bptree/bptreeSerializada.cpp"
#include "bptree/bptreeStringSerializada.cpp"

using namespace std;

void ler_arquivo_csv(const string& nome_arquivo, hashTable& hash, BPlusTree& arvore1, BPlusTreeString& arvore2)
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
            size_t posicao = hash.inserirRegistroBucket(novo_registro);

            RegistroBPT* reg1 = new RegistroBPT(id, posicao);
            RegistroString* reg2 = new RegistroString(title, posicao);

            arvore1.inserir_arvore(reg1);
            arvore2.inserir_arvore_s(reg2);
            
            delete novo_registro;
        } 
        catch (const std::exception& e)
        {
            //std::cerr << "Caught an exception: " << e.what() << " " << id << std::endl;
        }
    }
    arquivo.close();

    return;
}

int main(int argc, char const *argv[])
{   
    const string nome_arquivo = "artigo.csv";//argv[1];

    hashTable hash_table = hashTable(ARQUIVO_HASHTABLE);
    BPlusTree arvore_primaria(ALTURA_ARVORE_PRIMARIA);
    BPlusTreeString arvore_secundaria(ALTURA_ARVORE_SECUNDARIA);

    ler_arquivo_csv(nome_arquivo, hash_table, arvore_primaria, arvore_secundaria);

    arvore_primaria.serializar_arvore(arvore_primaria, ARQUIVO_ARVORE_PRIMARIA);
    arvore_secundaria.serializar_arvore_s(arvore_secundaria, ARQUIVO_ARVORE_SECUNDARIA);
    
    hash_table.mediaRegistros();
    return 0;
}