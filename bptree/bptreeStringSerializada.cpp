#ifndef BPlusTreeString_H
#define BPlusTreeString_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cassert>
#include <stack>
#include "../estruturas/estruturaBlocoRegistro.hpp"
#include <set>

// Registro que será usado na árvore
struct RegistroString
{ 
    string chave;
    size_t valor;
    RegistroString(string chave, size_t valor) : chave(chave), valor(valor) {}
    RegistroString() : chave(""), valor(0) {}
};

template <typename T>
//Nó da árvore - S simboliza String
struct NoS
{
    bool folha;
    std::size_t grau; 
    std::size_t ocupacao;  
    RegistroString* registros;
    NoS<RegistroString>** filhos;
    NoS<RegistroString>* ancestral;

public:
    NoS(std::size_t _grau)
    {
        this->folha = false;
        this->grau = _grau;
        this->ocupacao = 0;

        RegistroString* registro_aux = new RegistroString[grau-1];
        for (int i = 0; i < grau - 1; i++)
        {
            registro_aux[i] = RegistroString("", 0);
        }
        this->registros = registro_aux;

        NoS<RegistroString>** filhos_aux = new NoS<RegistroString>*[grau];
        for (int i = 0; i < grau; i++)
        {
            filhos_aux[i] = nullptr;
        }
        this->filhos = filhos_aux;
        this->ancestral = nullptr;
    }
};

class BPlusTreeString
{
public:
    NoS<RegistroString>* raiz;
    std::size_t grau;

public:
    BPlusTreeString(std::size_t _grau)
    {
        this->raiz = nullptr;
        this->grau = _grau;
    }

    NoS<RegistroString>* get_raiz(){ return this->raiz; } //retorna a raiz da árvore

    NoS<RegistroString>* busca_arvore_s(NoS<RegistroString>* no, string chave_busca)
    { // Raiz deve ser passada como parâmetro para no a fim de permitir recursão
        if (no == nullptr)
        { // Caso raiz nula
            return nullptr;
        }
        else
        {
            NoS<RegistroString>* cursor = no; 

            while (cursor->folha == false)
            { 
                for (int i = 0; i < cursor->ocupacao; i++)
                { 
                    if (chave_busca.compare(cursor->registros[i].chave) < 0)
                    { 
                        cursor = cursor->filhos[i];
                        break;
                    }
                    if (i == (cursor->ocupacao - 1))
                    {
                        cursor = cursor->filhos[i + 1];
                        break;
                    }
                }
            }

            // Busca se a chave existe nos nós folhas
            for (int i = 0; i<cursor->ocupacao; i++)
            {
                if (cursor->registros[i].chave.compare(chave_busca) == 0)
                {
                    return cursor;
                }
            }
            std::cout<<"NoS nao encontrado\n";
            return nullptr;
        }
    }

    NoS<RegistroString>* posicionar_cursor_arvore_s(NoS<RegistroString>* no, RegistroString chave_busca)
    { // Também usa raiz como parâmetro para no
        if (no == nullptr)
        { // Caso raiz nula
            return nullptr;
        }
        else
        {
            NoS<RegistroString>* cursor = no; 

            while (cursor->folha == false)
            { 
                for (int i = 0; i < cursor->ocupacao; i++)
                { 
                    if (chave_busca.chave.compare(cursor->registros[i].chave) < 0)
                    { 
                        cursor = cursor->filhos[i];
                        break;
                    }
                    if (i == (cursor->ocupacao) - 1)
                    {
                        cursor = cursor->filhos[i + 1];
                        break;
                    }
                }
            }
            return cursor;
        }
    }

    int busca_index_s(RegistroString* registros, RegistroString registro_buscado, int tamanho)
    {
        int index_registros = 0;
        for (int i = 0; i < tamanho; i++)
        {
            if (registro_buscado.chave.compare(registros[i].chave) < 0)
            {
                index_registros = i;
                break;
            }
            if (i == tamanho - 1)
            {
                index_registros = tamanho;
                break;
            }
        }
        return index_registros;
    }

    RegistroString* inserir_registro_s(RegistroString* registros, RegistroString registro_insercao, int tamanho)
    {
        int index = 0;
        for (int i = 0; i < tamanho; i++)
        {
            if (registro_insercao.chave.compare(registros[i].chave) < 0)
            {
                index = i;
                break;
            }
            if (i==tamanho-1)
            {
                index = tamanho;
                break;
            }
        }

        for (int i = tamanho; i > index; i--)
        {
            registros[i] = registros[i - 1];
        }

        registros[index] = registro_insercao;

        return registros;
    }

    NoS<RegistroString>** inserir_filho_s(NoS<RegistroString>** filhos, NoS<RegistroString>*filho,int tamanho,int index)
    {
        for (int i = tamanho; i > index; i--)
        {
            filhos[i] = filhos[i - 1];
        }
        filhos[index] = filho;
        return filhos;
    }

    NoS<RegistroString>* inserir_registro_filho_s(NoS<RegistroString>* no_insercao, RegistroString registro_insercao, NoS<RegistroString>* filho)
    {
        int registro_index=0;
        int filho_index=0;
        for (int i=0; i< no_insercao->ocupacao; i++)
        {
            if (registro_insercao.chave.compare(no_insercao->registros[i].chave) < 0)
            {
                registro_index = i;
                filho_index = i+1;
                break;
            }
            if (i == no_insercao->ocupacao-1)
            {
                registro_index = no_insercao->ocupacao;
                filho_index = no_insercao->ocupacao + 1;
                break;
            }
        }
        for (int i = no_insercao->ocupacao;i > registro_index; i--)
        {
            no_insercao->registros[i] = no_insercao->registros[i - 1];
        }
        for (int i = no_insercao->ocupacao + 1; i > filho_index;i--)
        {
            no_insercao->filhos[i] = no_insercao->filhos[i - 1];
        }

        no_insercao->registros[registro_index] = registro_insercao;
        no_insercao->filhos[filho_index] = filho;

        return no_insercao;
    }

    void inserir_ancestral_s(NoS<RegistroString>* ancestral,NoS<RegistroString>* filho, RegistroString registro_insercao)
    {
        NoS<RegistroString>* cursor = ancestral;
        if (cursor->ocupacao < this->grau-1)
        { // Checagem de overflow, caso negativo
            cursor = inserir_registro_filho_s(cursor, registro_insercao, filho);
            cursor->ocupacao++;
        }
        else 
        { // Overflow positivo
            auto* novo_no = new NoS<RegistroString>(this->grau);
            novo_no->ancestral = cursor->ancestral;

            RegistroString* registros_aux = new RegistroString[cursor->ocupacao + 1];
            for (int i = 0; i < cursor->ocupacao; i++)
            {
                registros_aux[i] = cursor->registros[i];
            }
            registros_aux = inserir_registro_s(registros_aux, registro_insercao, cursor->ocupacao);

            auto** filhos_aux = new NoS<RegistroString>*[cursor->ocupacao + 2];
            for (int i = 0; i < cursor->ocupacao+1; i++)
            {
                filhos_aux[i] = cursor->filhos[i];
            }
            filhos_aux[cursor->ocupacao + 1] = nullptr;
            filhos_aux = inserir_filho_s(filhos_aux, filho, cursor->ocupacao + 1 ,busca_index_s(registros_aux, registro_insercao, cursor->ocupacao + 1));

            // Divisao dos nós
            cursor->ocupacao = (this->grau) / 2;
            if ((this->grau) % 2 == 0)
            {
                novo_no->ocupacao = (this->grau) / 2 - 1;
            }
            else
            {
                novo_no->ocupacao = (this->grau) / 2;
            }

            for (int i = 0; i < cursor->ocupacao; i++)
            {
                cursor->registros[i] = registros_aux[i];
                cursor->filhos[i] = filhos_aux[i];
            }
            cursor->filhos[cursor->ocupacao] = filhos_aux[cursor->ocupacao];

            for (int i = 0; i < novo_no->ocupacao; i++)
            {
                novo_no->registros[i] = registros_aux[cursor->ocupacao + i + 1];
                novo_no->filhos[i] = filhos_aux[cursor->ocupacao + i + 1];
                novo_no->filhos[i]->ancestral = novo_no;
            }
            novo_no->filhos[novo_no->ocupacao] = filhos_aux[cursor->ocupacao+novo_no->ocupacao + 1];
            novo_no->filhos[novo_no->ocupacao]->ancestral = novo_no;

            RegistroString registros_ancestral = registros_aux[this->grau / 2];

            delete[] registros_aux;
            delete[] filhos_aux;

            // Balanceamento - ancestral
            if (cursor->ancestral == nullptr)
            { // Caso seja raiz, não há
                auto* novo_ancestral = new NoS<RegistroString>(this->grau);
                cursor->ancestral = novo_ancestral;
                novo_no->ancestral = novo_ancestral;

                novo_ancestral->registros[0] = registros_ancestral;
                novo_ancestral->ocupacao++;

                novo_ancestral->filhos[0] = cursor;
                novo_ancestral->filhos[1] = novo_no;

                this->raiz = novo_ancestral;
            }
            else
            {
                inserir_ancestral_s(cursor->ancestral, novo_no, registros_ancestral);
            }
        }
    }

    void inserir_arvore_s(RegistroString* registro_insercao)
    {
        RegistroString registro_aux(registro_insercao->chave, registro_insercao->valor);

        if (this->raiz == nullptr)
        { // Caso de raiz nula
            this->raiz = new NoS<RegistroString>(this->grau);
            this->raiz->folha = true;
            this->raiz->registros[0] = *registro_insercao;
            this->raiz->ocupacao = 1; 
        } else 
        { 
            NoS<RegistroString>* cursor = this->raiz;

            // Ir até as folhas
            cursor = posicionar_cursor_arvore_s(cursor, registro_aux);

            if (cursor->ocupacao < (this->grau-1))
            { // Caso não haja overflow
                cursor->registros = inserir_registro_s(cursor->registros, *registro_insercao, cursor->ocupacao);
                cursor->ocupacao++;
                cursor->filhos[cursor->ocupacao] = cursor->filhos[cursor->ocupacao - 1];
                cursor->filhos[cursor->ocupacao - 1] = nullptr;
            }
            else
            {   // Overflow detectado
                auto* novo_no = new NoS<RegistroString>(this->grau);
                novo_no->folha = true;
                novo_no->ancestral = cursor->ancestral;

                RegistroString* registros_aux = new RegistroString[cursor->ocupacao + 1];
                
                for (int i = 0; i < cursor->ocupacao; i++)
                {
                    registros_aux[i] = cursor->registros[i];
                }

                registros_aux = inserir_registro_s(registros_aux, *registro_insercao, cursor->ocupacao);

                // Dividir nós
                cursor->ocupacao = (this->grau) / 2;
                
                if ((this->grau) % 2 == 0)
                {
                    novo_no->ocupacao = (this->grau) / 2;
                }
                else
                {
                    novo_no->ocupacao = (this->grau) / 2 + 1;
                }

                for (int i = 0; i < cursor->ocupacao; i++)
                {
                    cursor->registros[i] = registros_aux[i];
                }
                for (int i=0; i < novo_no->ocupacao; i++)
                {
                    novo_no->registros[i] = registros_aux[cursor->ocupacao + i];
                }

                cursor->filhos[cursor->ocupacao] = novo_no;
                novo_no->filhos[novo_no->ocupacao] = cursor->filhos[this->grau-1];
                cursor->filhos[this->grau-1] = nullptr;
                delete[] registros_aux;

                // Checagem de ancestral
                RegistroString registros_ancestral = novo_no->registros[0];

                if (cursor->ancestral == nullptr)
                { //caso não haja nó ancestral -> cursor = raiz
                    auto* novo_ancestral = new NoS<RegistroString>(this->grau);
                    cursor->ancestral = novo_ancestral;
                    novo_no->ancestral = novo_ancestral;

                    novo_ancestral->registros[0] = registros_ancestral;
                    novo_ancestral->ocupacao++;

                    novo_ancestral->filhos[0] = cursor;
                    novo_ancestral->filhos[1] = novo_no;

                    this->raiz = novo_ancestral;
                }
                else
                {
                    inserir_ancestral_s(cursor->ancestral, novo_no, registros_ancestral);
                }
            }
        }
    }

    void deletar_s(NoS<RegistroString>* cursor) //deleta a árvore
    {
        if (cursor != nullptr)
        {
            if (!cursor->folha)
            {
                for (int i = 0; i <= cursor->ocupacao; i++)
                {
                    deletar_s(cursor->filhos[i]);
                }
            }
            delete[] cursor->registros;
            delete[] cursor->filhos;
            delete cursor;
            this->raiz = NULL;
        } else
        {
            std::cout << "Arvore deletada" << endl;
        }
    }

    void imprimir_arvore_s() {
        imprimir_no_s(this->raiz, 0, "");
    }

    void imprime_registro_arvore_s(const RegistroString& registro, const std::string& prefixo) {
        std::cout << prefixo << "Chave: " << registro.chave << " Valor: " << registro.valor << std::endl;
    }

    void imprimir_no_s(NoS<RegistroString>* cursor, int nivel, const std::string& prefixo) {
        if (cursor == NULL) {
            std::cout << prefixo << "- <no vazio>" << std::endl;
            return;
        }

        std::string novo_prefixo = prefixo + (nivel > 0 ? "|  " : "");
        std::string filho_prefixo = novo_prefixo + "|--";

        for (int i = 0; i < cursor->ocupacao; ++i) {
            imprime_registro_arvore_s(cursor->registros[i], novo_prefixo);
            if (!cursor->folha) {
                imprimir_no_s(cursor->filhos[i], nivel + 1, filho_prefixo);
            }
        }

        if (!cursor->folha) {
            imprimir_no_s(cursor->filhos[cursor->ocupacao], nivel + 1, filho_prefixo);
        }
    }

    NoS<RegistroString>* desserializar_no_s(ifstream& arquivo, NoS<RegistroString>* ancestral, size_t grau)
    {
        bool folha;
        size_t tamanho;

        // Ler informações do nó
        if (!arquivo.read(reinterpret_cast<char*>(&folha), sizeof(folha)) || !arquivo.read(reinterpret_cast<char*>(&tamanho), sizeof(tamanho)))
        {
            cerr << "Erro ao ler informaçoes do no do arquivo." << endl;
            return nullptr;
        }

        // Criar novo nó
        auto* node = new NoS<RegistroString>(grau);
        node->folha = folha;
        node->ocupacao = tamanho;
        node->ancestral = ancestral;

        // Alocar espaço para os registros
        node->registros = new RegistroString[grau - 1];

        // Ler registros do nó
        for (size_t i = 0; i < tamanho; ++i)
        {
            size_t chave_size;
            arquivo.read(reinterpret_cast<char*>(&chave_size), sizeof(chave_size));
            string chave(chave_size, '\0');
            arquivo.read(&chave[0], chave_size);
            size_t valor;
            arquivo.read(reinterpret_cast<char*>(&valor), sizeof(valor));
            node->registros[i] = RegistroString(chave, valor);
        }

        // Desserializar nós filhos
        if (!folha)
        {
            node->filhos = new NoS<RegistroString>*[grau];
            for (size_t i = 0; i <= tamanho; ++i)
            {
                node->filhos[i] = desserializar_no_s(arquivo, node, grau);
            }
        }

        return node;
    }

    void destruir_no_s(NoS<RegistroString>* no) 
    {
        if (no)
        {
            if (!no->folha)
            {
                for (size_t i = 0; i <= no->ocupacao; ++i)
                {
                    destruir_no_s(no->filhos[i]);
                }

                delete[] no->filhos;
            }

            delete[] no->registros;
            delete no;
        }
    }

    BPlusTreeString desserializar_arvore_s(const string& nome_arquivo)
    {
        ifstream arquivo(nome_arquivo, ios::binary | ios::in);
        if (!arquivo)
        {
            cerr << "Erro ao abrir aquivo para desserializacao: " << nome_arquivo << endl;
            return BPlusTreeString(0);  // Retornar uma árvore B+ vazia
        }

        // Ler o grau da árvore do arquivo
        size_t grau;
        if (!arquivo.read(reinterpret_cast<char*>(&grau), sizeof(grau)))
        {
            cerr << "Erro ao ler o grau do arquivo: " << nome_arquivo << endl;
            arquivo.close();
            return BPlusTreeString(0);  // Retornar uma árvore B+ vazia
        }

        // Criar uma nova árvore B+ com o grau fornecido
        BPlusTreeString arvore(grau);

        // Desserializar a árvore recursivamente, começando pelo nó raiz
        arvore.raiz = desserializar_no_s(arquivo, nullptr, grau);

        if (!arvore.raiz)
        {
            cerr << "Erro ao desserializar a raiz do arquivo: " << nome_arquivo << endl;
            arquivo.close();
            return BPlusTreeString(0);  // Retornar uma árvore B+ vazia
        }

        arquivo.close();

        return arvore;
    }

    int contar_nos_s(NoS<RegistroString>* NoS) {
        if (NoS == nullptr) {
            return 0;
        }

        int c = 1;

        if (!NoS->folha) {
            for (size_t i = 0; i <= NoS->ocupacao; i++) {
                c += contar_nos_s(NoS->filhos[i]); // conta os nós filhos recursivamente
            }
        }

        return c;
    }

    void serializar_arvore_s(const BPlusTreeString& arvore, const string& nome_arquivo) {
        ofstream arquivo(nome_arquivo, ios::binary | ios::out);
        if (!arquivo) {
            cerr << "Erro ao abrir o arquivo para serializaçao " << nome_arquivo << endl;
            return;
        }

        // Escrever o grau da árvore NoS arquivo
        size_t grau = arvore.grau;
        arquivo.write(reinterpret_cast<char*>(&grau), sizeof(grau));

        // Serializar a árvore recursivamente, começando pelo nó raiz
        serializar_no_s(arquivo, arvore.raiz);
        //deletar_s(tree.raiz);
        arquivo.close();
    }

    void serializar_no_s(ofstream& arquivo, const NoS<RegistroString>* no) {
        if (no == nullptr) {
            return;
        }

        // Escrever as informações do nó no arquivo
        bool folha = no->folha;
        size_t tamanho = no->ocupacao;
        arquivo.write(reinterpret_cast<const char*>(&folha), sizeof(folha));
        arquivo.write(reinterpret_cast<const char*>(&tamanho), sizeof(tamanho));

        // Escrever os registros do nó no arquivo
        for (size_t i = 0; i < tamanho; ++i) {
            size_t tam_chave = no->registros[i].chave.size();
            arquivo.write(reinterpret_cast<const char*>(&tam_chave), sizeof(tam_chave));
            arquivo.write(no->registros[i].chave.c_str(), tam_chave);
            arquivo.write(reinterpret_cast<const char*>(&no->registros[i].valor), sizeof(no->registros[i].valor));
        }

        // Serializar nós filhos, se não for folha
        if (!folha) {
            for (size_t i = 0; i <= tamanho; ++i) {
                serializar_no_s(arquivo, no->filhos[i]);
            }
        }
    }

    Registro* buscar_registro_s(ifstream& arquivo_index, string id_busca) 
    {
        NoS<RegistroString>* node = this->busca_arvore_s(this->raiz, id_busca);
        Registro* registro = nullptr;

        if (node != nullptr) {
            RegistroString* reg = nullptr;

            for (int i = 0; i < node->ocupacao; i++) {

                if (node->registros[i].chave.compare(id_busca) == 0) {
                    reg = &node->registros[i];
                    break;
                }
            }

            if (reg == nullptr) {
                this->deletar_s(this->get_raiz());
                return nullptr;
            }

            registro = new Registro();
            arquivo_index.seekg(reg->valor);
            arquivo_index.read(reinterpret_cast<char*>(&registro->id), sizeof(int));

            getline(arquivo_index, registro->title, '\0');
            arquivo_index.read(reinterpret_cast<char*>(&registro->year), sizeof(int));
            getline(arquivo_index, registro->authors, '\0');
            arquivo_index.read(reinterpret_cast<char*>(&registro->citations), sizeof(int));
            getline(arquivo_index, registro->update, '\0');
            getline(arquivo_index, registro->snippet, '\0');

            registro->ocupacao = sizeof(int) + registro->title.size() + 1 +
                                sizeof(int) + registro->authors.size() + 1 +
                                sizeof(int) + registro->update.size() + 1 +
                                registro->snippet.size() + 1;

            int quantidade_nos = contar_nos_s(this->get_raiz());
            cout << "Quantidade total de blocos do arquivo de indice secundario: " << quantidade_nos << endl;

            this->deletar_s(this->get_raiz());
        }

        return registro;
    }
};
#endif