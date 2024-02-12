#ifndef BLOCOREGISTRO_HPP
#define BLOCOREGISTRO_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector> //tentar trabalhar sem e se conseguir apagar 

using namespace std;

struct Registro{
    int id;
    string title;
    int year;
    string authors;
    int citations;
    string update;
    string snippet;
    int tamanho;
};

struct Cabecalho_Bloco{
    int quantidade_registros;
    int tamanho_disponivel;
    int posicoes_registros[X];
    
};

struct Bloco {
    Cabecalho_Bloco* cabecalho;
    unsigned char dados[TAM_BLOCO];
};

//Funções para Registro
Registro* criar_registro(int id, string title, int year, string authors, int citations, string update, string snippet) {
    Registro* registro = new Registro();
    registro->id = id;
    registro->title = title;
    registro->year = year;
    registro->authors = authors;
    registro->citations = citations;
    registro->update = update;
    registro->snippet = snippet;
    registro->tamanho = registro->title.size() + 4 + sizeof(int) + sizeof(int) + registro->authors.size() + sizeof(int) + sizeof(int) + registro->update.size() + registro->snippet.size();
    return registro;
}

void imprimir_registro(Registro registro) {
    std::cout << "------------------------" << "\n";
    std::cout << "Id: " << registro.id << "\n";
    std::cout << "Titulo: " << registro.title << "| Ano: " << registro.year <<"\n";
    std::cout << "Autores: " << registro.authors << "\n";
    std::cout << "Citacoes: " << registro.citations << "\n";
    std::cout << "Atualizacao: " << registro.update << "\n";
    std::cout << "Snippet: " << registro.snippet << "\n";
    std::cout << "Tamanho do registro em bytes: " << registro.tamanho << "\n";
    std::cout << "------------------------" << "\n";
}

//Funções para Bloco
Cabecalho_Bloco* criar_cabecalho_bloco() {
    Cabecalho_Bloco* novo_cabecalho = new Cabecalho_Bloco();
    novo_cabecalho->quantidade_registros = 0;
    for (int i = 0; i < X; i++) {
        novo_cabecalho->posicoes_registros[i] = 0;
    }
    novo_cabecalho->tamanho_disponivel = TAM_BLOCO - sizeof(int) * 18 - sizeof(int) * 2;

    return novo_cabecalho;
}

Bloco* criar_bloco() {
    Bloco* novo_bloco = new Bloco();
    novo_bloco->cabecalho = criar_cabecalho_bloco();
    for(int i = 0; i < TAM_BLOCO; i++) {
        novo_bloco->dados[i] = 0;
    }
    return novo_bloco;
}

void deletar_bloco(Bloco* bloco) {
    delete bloco->cabecalho;
    delete bloco;
}

#endif