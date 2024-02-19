#include <iostream>
#include <fstream>
#include <sstream>
#include "./estruturaBlocoRegistro.hpp"
#include "../hash/hashtable.cpp"
using namespace std;

vector<Registro*> ler_arquivo_csv(const string& nome_arquivo) {
    vector<Registro*> registros;

    ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        cerr << "Nao foi possivel abrir o arquivo: " << nome_arquivo << "\n";
        return registros;
    }

    string linha;
    while (getline(arquivo, linha)) {
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

        Registro* novo_registro = criar_registro(id, title, year, authors, citations, update, snippet);
        registros.push_back(novo_registro);
    }
    arquivo.close();

    return registros;
}

int main(){ //quase o findrec, no fim isso não vai estar aqui
    const string nome_arquivo = "../teste2.csv";
    vector<Registro*> registros = ler_arquivo_csv(nome_arquivo);
    hashTable Hash = hashTable("arquivoDados.bin");

    for (const auto& registro : registros) {
        Hash.inserirRegistroBucket(registro);
    }

    imprimir_registro(Hash.busca_registro_hashtable(428050));

    for (auto& registro : registros) {
        delete registro;
    } 

    return 0;
}