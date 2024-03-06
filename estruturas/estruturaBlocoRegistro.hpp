#ifndef BLOCOREGISTRO_HPP
#define BLOCOREGISTRO_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include "definicoes.hpp"

using namespace std;

struct Registro
{
    int id;
    string title;
    int year;
    string authors;
    int citations;
    string update;
    string snippet;
    int ocupacao;
};

struct Cabecalho_Bloco
{
    int quantidade_registros;
    int tamanho_disponivel;
    int posicoes_registros[BLOCO_REGISTROS];
    
};

struct Bloco
{
    Cabecalho_Bloco* cabecalho;
    char dados[TAM_BLOCO - sizeof(Cabecalho_Bloco)];
};

//Funções para Registro
Registro* criar_registro(int id, string title, int year, string authors, int citations, string update, string snippet)
{
    Registro* registro = new Registro();
    registro->id = id;
    registro->title = title;
    registro->year = year;
    registro->authors = authors;
    registro->citations = citations;
    registro->update = update;
    registro->snippet = snippet;
    registro->ocupacao = sizeof(int) // id
                       + title.size() + 1 // Tamanho da string title + caractere nulo
                       + sizeof(int) // year
                       + authors.size() + 1 // Tamanho da string authors + caractere nulo
                       + sizeof(int) // citations
                       + update.size() + 1 // Tamanho da string update + caractere nulo
                       + snippet.size() + 1; // Tamanho da string snippet + caractere nulo
    return registro;
}

void imprimir_registro(Registro* registro)
{
    if (registro == NULL)
    {
        std::cout << "Registro NULL"<< "\n";
        return;
    }
    std::cout << "Id: " << registro->id << "\n";
    std::cout << "Titulo: " << registro->title << "| Ano: " << registro->year <<"\n";
    std::cout << "Autores: " << registro->authors << "\n";
    std::cout << "Citacoes: " << registro->citations << "\n";
    std::cout << "Atualizacao: " << registro->update << "\n";
    std::cout << "Snippet: " << registro->snippet << "\n";

}

Registro* preencher_registro(Registro* registro, Bloco* bloco, int cursor)
{
    registro->title = string((char *)&bloco->dados[cursor]);
    cursor += registro->title.size() + 1;

    memcpy(&registro->year, &bloco->dados[cursor], 2);
    cursor += sizeof(int);

    registro->authors = string((char *)&bloco->dados[cursor]);
    cursor += registro->authors.size() + 1;

    memcpy(&registro->citations, &bloco->dados[cursor], 1);
    cursor += sizeof(int);

    registro->update = string((char *)&bloco->dados[cursor]);
    cursor += registro->update.size() + 1;

    registro->snippet = string((char *)&bloco->dados[cursor]);
    cursor += registro->snippet.size() + 1;

    return registro;
}

//Funções para Bloco
Cabecalho_Bloco* criar_cabecalho_bloco()
{
    Cabecalho_Bloco* novo_cabecalho = new Cabecalho_Bloco();
    novo_cabecalho->quantidade_registros = 0;
    for (int i = 0; i < BLOCO_REGISTROS; i++)
    {
        novo_cabecalho->posicoes_registros[i] = 0;
    }
    novo_cabecalho->tamanho_disponivel = TAM_BLOCO - sizeof(Cabecalho_Bloco);

    return novo_cabecalho;
}

Bloco* criar_bloco()
{
    Bloco* novo_bloco = new Bloco();
    novo_bloco->cabecalho = criar_cabecalho_bloco();
    for (int i = 0; i < TAM_BLOCO - sizeof(Cabecalho_Bloco); i++)
    {
        novo_bloco->dados[i] = 0;
    }
    return novo_bloco;
}

void deletar_bloco(Bloco* bloco)
{
    delete bloco->cabecalho;
    delete bloco;
}

Bloco* inserir_registro_bloco(Bloco* bloco, Registro* registro)
{
    size_t cursor = bloco->cabecalho->posicoes_registros[bloco->cabecalho->quantidade_registros];
    memcpy(&bloco->dados[cursor], &registro->id, sizeof(int));
    cursor += sizeof(int);
    memcpy(&bloco->dados[cursor], registro->title.c_str(), registro->title.size() + 1);
    cursor += registro->title.size() + 1;
    memcpy(&bloco->dados[cursor], &registro->year, sizeof(int));
    cursor += sizeof(int);
    memcpy(&bloco->dados[cursor], registro->authors.c_str(), registro->authors.size() + 1);
    cursor += registro->authors.size() + 1;
    memcpy(&bloco->dados[cursor], &registro->citations, sizeof(int));
    cursor += sizeof(int);
    memcpy(&bloco->dados[cursor], registro->update.c_str(), registro->update.size() + 1);
    cursor += registro->update.size() + 1;
    memcpy(&bloco->dados[cursor], registro->snippet.c_str(), registro->snippet.size() + 1);
    cursor += registro->snippet.size() + 1;

    bloco->cabecalho->quantidade_registros++;
    bloco->cabecalho->tamanho_disponivel -= registro->ocupacao;
    bloco->cabecalho->posicoes_registros[bloco->cabecalho->quantidade_registros] = cursor;
    return bloco;
}

#endif