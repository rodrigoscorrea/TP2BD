#ifndef BPlusTree_H
#define BPlusTree_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cassert>
#include <stack>
#include "../estruturas/estruturaBlocoRegistro.hpp"
#include <set>

// Registro que será usado na árvore
struct RegistroBPT
{ // VERIFICAR DEPOIS SE DÁ PRA FAZER SETANDO AO INVÉS DOS DOIS PARA DIFERENCIAR MAIS AINDA
    int chave;
    size_t valor;
    RegistroBPT(int chave, size_t valor) : chave(chave), valor(valor) {}
    RegistroBPT() : chave(0), valor(0) {} 
};

template <typename T>
struct No
{
    bool folha;
    std::size_t grau; 
    std::size_t ocupacao;  
    RegistroBPT* registros;
    No<RegistroBPT>** filhos;
    No<RegistroBPT>* ancestral;

public:
    No(std::size_t _grau)
    {
        this->folha = false;
        this->grau = _grau;
        this->ocupacao = 0;

        RegistroBPT* registro_aux = new RegistroBPT[grau-1];
        for (int i=0; i<grau-1; i++)
        {
            registro_aux[i] = RegistroBPT(0,0);
        }
        this->registros = registro_aux;

        No<RegistroBPT>** filhos_aux = new No<RegistroBPT>*[grau];
        for (int i=0; i<grau; i++)
        {
            filhos_aux[i] = nullptr;
        }
        this->filhos = filhos_aux;
        this->ancestral = nullptr;
    }
};

class BPlusTree
{
public:
    No<RegistroBPT>* raiz;
    std::size_t grau;

public:
    BPlusTree(std::size_t _grau)
    {
        this->raiz = nullptr;
        this->grau = _grau;
    }

    No<RegistroBPT>* get_raiz(){ return this->raiz; }

    No<RegistroBPT>* busca_arvore(No<RegistroBPT>* node, int chave_busca)
    { // Raiz deve ser passada como parâmetro para node
        if (node == nullptr)
        { // Caso raiz nula
            return nullptr;
        }
        else
        {
            No<RegistroBPT>* cursor = node; 

            while (cursor->folha == false)
            { 
                for (int i=0; i<cursor->ocupacao; i++)
                { 
                    if (chave_busca < cursor->registros[i].chave)
                    { 
                        cursor = cursor->filhos[i];
                        break;
                    }
                    if (i == (cursor->ocupacao -1))
                    {
                        cursor = cursor->filhos[i+1];
                        break;
                    }
                }
            }

            // Busca se a chave existe no nó folha
            for (int i=0; i<cursor->ocupacao; i++)
            {
                if (cursor->registros[i].chave == chave_busca)
                {
                    return cursor;
                }
            }
            std::cout<<"No nao encontrado\n";
            return nullptr;
        }
    }

    No<RegistroBPT>* busca_arvore_alcance(No<RegistroBPT>* node, RegistroBPT chave_busca)
    { // Também usa raiz como parâmetro para node
        if (node == nullptr)
        { // Caso raiz nula
            return nullptr;
        }
        else
        {
            No<RegistroBPT>* cursor = node; 

            while (cursor->folha == false)
            { 
                for (int i=0; i<cursor->ocupacao; i++)
                { 
                    if (chave_busca.chave < cursor->registros[i].chave)
                    { 
                        cursor = cursor->filhos[i];
                        break;
                    }
                    if (i == (cursor->ocupacao)-1)
                    {
                        cursor = cursor->filhos[i+1];
                        break;
                    }
                }
            }
            return cursor;
        }
    }

    int busca_index(RegistroBPT* registros, RegistroBPT registro_buscado, int tamanho)
    {
        int index_registros = 0;
        for (int i=0; i<tamanho; i++)
        {
            if (registro_buscado.chave < registros[i].chave)
            {
                index_registros = i;
                break;
            }
            if (i==tamanho-1)
            {
                index_registros = tamanho;
                break;
            }
        }
        return index_registros;
    }

    RegistroBPT* inserir_registro(RegistroBPT* registros, RegistroBPT registro_insercao, int tamanho)
    {
        int index = 0;
        for (int i=0; i<tamanho; i++)
        {
            if (registro_insercao.chave < registros[i].chave)
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
            registros[i] = registros[i-1];
        }

        registros[index] = registro_insercao;

        return registros;
    }

    No<RegistroBPT>** inserir_filho(No<RegistroBPT>** filhos, No<RegistroBPT>*filho,int tamanho,int index)
    {
        for (int i= tamanho; i > index; i--)
        {
            filhos[i] = filhos[i-1];
        }
        filhos[index] = filho;
        return filhos;
    }

    No<RegistroBPT>* inserir_registro_filho(No<RegistroBPT>* no_insercao, RegistroBPT registro_insercao, No<RegistroBPT>* filho)
    {
        int registro_index=0;
        int filho_index=0;
        for (int i=0; i< no_insercao->ocupacao; i++)
        {
            if (registro_insercao.chave < no_insercao->registros[i].chave)
            {
                registro_index = i;
                filho_index = i+1;
                break;
            }
            if (i==no_insercao->ocupacao-1)
            {
                registro_index = no_insercao->ocupacao;
                filho_index = no_insercao->ocupacao+1;
                break;
            }
        }
        for (int i = no_insercao->ocupacao;i > registro_index; i--)
        {
            no_insercao->registros[i] = no_insercao->registros[i-1];
        }
        for (int i=no_insercao->ocupacao+1;i>filho_index;i--)
        {
            no_insercao->filhos[i] = no_insercao->filhos[i-1];
        }

        no_insercao->registros[registro_index] = registro_insercao;
        no_insercao->filhos[filho_index] = filho;

        return no_insercao;
    }

    void inserir_ancestral(No<RegistroBPT>* ancestral,No<RegistroBPT>* filho, RegistroBPT registro_insercao)
    {
        No<RegistroBPT>* cursor = ancestral;
        if (cursor->ocupacao < this->grau-1)
        { // Checagem de overflow, caso negativo
            cursor = inserir_registro_filho(cursor,registro_insercao,filho);
            cursor->ocupacao++;
        }
        else 
        { // Overflow positivo
            auto* novo_no = new No<RegistroBPT>(this->grau);
            novo_no->ancestral = cursor->ancestral;

            RegistroBPT* registros_aux = new RegistroBPT[cursor->ocupacao+1];
            for (int i=0; i<cursor->ocupacao; i++)
            {
                registros_aux[i] = cursor->registros[i];
            }
            registros_aux = inserir_registro(registros_aux,registro_insercao,cursor->ocupacao);

            auto** filhos_aux = new No<RegistroBPT>*[cursor->ocupacao+2];
            for (int i=0; i<cursor->ocupacao+1;i++)
            {
                filhos_aux[i] = cursor->filhos[i];
            }
            filhos_aux[cursor->ocupacao+1] = nullptr;
            filhos_aux = inserir_filho(filhos_aux,filho,cursor->ocupacao+1,busca_index(registros_aux,registro_insercao,cursor->ocupacao+1));

            // Divisao dos nós
            cursor->ocupacao = (this->grau)/2;
            if ((this->grau) % 2 == 0)
            {
                novo_no->ocupacao = (this->grau) / 2 -1;
            }
            else
            {
                novo_no->ocupacao = (this->grau) / 2;
            }

            for (int i=0; i<cursor->ocupacao;i++)
            {
                cursor->registros[i] = registros_aux[i];
                cursor->filhos[i] = filhos_aux[i];
            }
            cursor->filhos[cursor->ocupacao] = filhos_aux[cursor->ocupacao];

            for (int i=0; i < novo_no->ocupacao; i++)
            {
                novo_no->registros[i] = registros_aux[cursor->ocupacao + i +1];
                novo_no->filhos[i] = filhos_aux[cursor->ocupacao+i+1];
                novo_no->filhos[i]->ancestral=novo_no;
            }
            novo_no->filhos[novo_no->ocupacao] = filhos_aux[cursor->ocupacao+novo_no->ocupacao+1];
            novo_no->filhos[novo_no->ocupacao]->ancestral=novo_no;

            RegistroBPT registros_ancestral = registros_aux[this->grau/2];

            delete[] registros_aux;
            delete[] filhos_aux;

            // Balanceamento - ancestral
            if (cursor->ancestral == nullptr)
            { // Caso seja raiz, não há n
                auto* novo_ancestral = new No<RegistroBPT>(this->grau);
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
                inserir_ancestral(cursor->ancestral, novo_no, registros_ancestral);
            }
        }
    }

    void inserir_arvore(RegistroBPT* registro_insercao)
    {
        RegistroBPT registro_aux(registro_insercao->chave, registro_insercao->valor);

        if (this->raiz == nullptr)
        { // Caso de raiz nula
            this->raiz = new No<RegistroBPT>(this->grau);
            this->raiz->folha = true;
            this->raiz->registros[0] = *registro_insercao;
            this->raiz->ocupacao = 1; 
        } else 
        { 
            No<RegistroBPT>* cursor = this->raiz;

            // Ir até as folhas
            cursor = busca_arvore_alcance(cursor, registro_aux);

            if (cursor->ocupacao < (this->grau-1))
            { // Caso não haja overflow
                cursor->registros = inserir_registro(cursor->registros, *registro_insercao, cursor->ocupacao);
                cursor->ocupacao++;
                cursor->filhos[cursor->ocupacao] = cursor->filhos[cursor->ocupacao - 1];
                cursor->filhos[cursor->ocupacao-1] = nullptr;
            }
            else
            {   // Overflow detectado
                auto* novo_no = new No<RegistroBPT>(this->grau);
                novo_no->folha = true;
                novo_no->ancestral = cursor->ancestral;

                RegistroBPT* registros_aux = new RegistroBPT[cursor->ocupacao + 1];
                
                for (int i = 0; i < cursor->ocupacao; i++)
                {
                    registros_aux[i] = cursor->registros[i];
                }

                registros_aux = inserir_registro(registros_aux, *registro_insercao, cursor->ocupacao);

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

                for (int i=0; i<cursor->ocupacao;i++)
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
                RegistroBPT registros_ancestral = novo_no->registros[0];

                if (cursor->ancestral == nullptr)
                { //caso não haja nó ancestral -> cursor = raiz
                    auto* novo_ancestral = new No<RegistroBPT>(this->grau);
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
                    inserir_ancestral(cursor->ancestral, novo_no, registros_ancestral);
                }
            }
        }
    }

    void deletar(No<RegistroBPT>* cursor)
    {
        if (cursor != nullptr)
        {
            if (!cursor->folha)
            {
                for (int i=0; i <= cursor->ocupacao; i++)
                {
                    deletar(cursor->filhos[i]);
                }
            }
            delete[] cursor->registros;
            delete[] cursor->filhos;
            delete cursor;
            this->raiz = NULL;
        } else {
            std::cout<<"Arvore deletada\n";
        }
    }

    void imprimir_arvore() {
        imprimir_no(this->raiz, 0, "");
    }

    void imprime_registro(const RegistroBPT& registro, const std::string& prefixo) {
        std::cout << prefixo << "Chave: " << registro.chave << " Valor: " << registro.valor << std::endl;
    }

    void imprimir_no(No<RegistroBPT>* cursor, int nivel, const std::string& prefixo) {
        if (cursor == NULL) {
            std::cout << prefixo << "- <nó vazio>" << std::endl;
            return;
        }

        std::string novo_prefixo = prefixo + (nivel > 0 ? "|  " : "");
        std::string child_prefixo = novo_prefixo + "|--";

        for (int i = 0; i < cursor->ocupacao; ++i) {
            imprime_registro(cursor->registros[i], novo_prefixo);
            if (!cursor->folha) {
                imprimir_no(cursor->filhos[i], nivel + 1, child_prefixo);
            }
        }

        if (!cursor->folha) {
            imprimir_no(cursor->filhos[cursor->ocupacao], nivel + 1, child_prefixo);
        }
    }

    No<RegistroBPT>* desserializar_no(ifstream& arquivo, No<RegistroBPT>* ancestral, size_t grau)
    {
        // Ler as informações do nó do arquivo
        bool folha;
        size_t tamanho;
        if (!arquivo.read(reinterpret_cast<char*>(&folha), sizeof(folha)) || !arquivo.read(reinterpret_cast<char*>(&tamanho), sizeof(tamanho)))
        {
            cerr << "Error reading node information from arquivo." << endl;
            return nullptr;
        }

        // Criar um novo nó
        auto* no = new No<RegistroBPT>(grau);
        no->folha = folha;
        no->ocupacao = tamanho;
        no->ancestral = ancestral;

        // Ler os itens do nó do arquivo
        no->registros = new RegistroBPT[grau - 1];
        if (!arquivo.read(reinterpret_cast<char*>(no->registros), sizeof(RegistroBPT) * (grau - 1)))
        {
            cerr << "Erro ao ler no do arquivo." << endl;
            delete no;
            return nullptr;
        }

        if (!folha)
        {
            // Desserializar os nós filhos recursivamente
            no->filhos = new No<RegistroBPT>*[grau];
            for (size_t i = 0; i <= tamanho; ++i) {
                no->filhos[i] = desserializar_no(arquivo, no, grau);
                if (!no->filhos[i])
                {
                    cerr << "Erro ao desserializar filhos do no do arquivo" << endl;
                    destruir_no(no); // Liberar a memória alocada
                    return nullptr;
                }
            }
        }
        return no;
    }

    void destruir_no(No<RegistroBPT>* no)
    {
        if (no)
        {
            if (!no->folha)
            {
                for (size_t i = 0; i <= no->ocupacao; ++i)
                {
                    destruir_no(no->filhos[i]);
                }

                delete[] no->filhos;
            }

            delete[] no->registros;
            delete no;
        }
    }

    BPlusTree desserializar_arvore(const string& nome_arquivo)
    {
        ifstream arquivo(nome_arquivo, ios::binary | ios::in);
        if (!arquivo)
        {
            cerr << "Erro abrindo arquivo para desserilização: " << nome_arquivo << endl;
            return BPlusTree(0);  // Retornar uma árvore B+ vazia
        }

        // Ler o grau da árvore do arquivo
        size_t grau;
        if (!arquivo.read(reinterpret_cast<char*>(&grau), sizeof(grau)))
        {
            cerr << "Erro ao ler grau pelo arquivo: " << nome_arquivo << endl;
            arquivo.close();
            return BPlusTree(0);  // Retornar uma árvore B+ vazia
        }

        // Criar uma nova árvore B+ com o grau fornecido
        BPlusTree arv(grau);

        // Desserializar a árvore recursivamente, começando pelo nó raiz
        arv.raiz = desserializar_no(arquivo, nullptr, grau);

        if (!arv.raiz)
        {
            cerr << "Erro ao desserializar raiz do arquivo: " << nome_arquivo << endl;
            arquivo.close();
            return BPlusTree(0);  // Retornar uma árvore B+ vazia
        }

        arquivo.close();

        return arv;
    }

    int contar_nos(No<RegistroBPT>* no) {
        if (no == nullptr) {
            return 0;
        }

        int c = 1;

        if (!no->folha) {
            for (size_t i = 0; i <= no->ocupacao; i++) {
                c += contar_nos(no->filhos[i]); // conta os nós filhos recursivamente
            }
        }

        return c;
    }

    // Função recursiva para serializar um nó e seus filhos
    void serializar_no(ofstream& arquivo, const No<RegistroBPT>* no) {
        // Escrever as informações do nó no arquivo (is_leaf e size)
        bool folha = no->folha;
        size_t ocupacao = no->ocupacao;
        arquivo.write(reinterpret_cast<const char*>(&folha), sizeof(folha));
        arquivo.write(reinterpret_cast<const char*>(&ocupacao), sizeof(ocupacao));

        // Escrever os itens do nó no arquivo (registros)
        arquivo.write(reinterpret_cast<const char*>(no->registros), sizeof(RegistroBPT) * (no->grau - 1));

        if (!folha) {
            // Serializar os nós filhos recursivamente
            for (size_t i = 0; i <= ocupacao; ++i) {
                serializar_no(arquivo, no->filhos[i]);
            }
        }
    }

    Registro* buscar_registro_bpt(ifstream& arquivo_index, int id_busca) 
    {
        No<RegistroBPT>* node = this->busca_arvore(this->raiz, id_busca);
        Registro* registro = nullptr;

        if (node != nullptr) {
            RegistroBPT* reg = nullptr;

            for (int i = 0; i < node->ocupacao; i++) {

                if (node->registros[i].chave == id_busca) {
                    reg = &node->registros[i];
                    break;
                }
            }

            if (reg == nullptr) {
                this->deletar(this->get_raiz());
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

            int quantidade_nos = contar_nos(this->get_raiz());
            cout << "Quantidade total de blocos do arquivo de índice primário: " << quantidade_nos << endl;

            this->deletar(this->get_raiz());
        }

        return registro;
    }

    void serializar_arvore(const BPlusTree& arvore, const string& nome_arquivo) {
        ofstream arquivo(nome_arquivo, ios::binary | ios::out);
        if (!arquivo) {
            cerr << "Error opening arquivo for serialization: " << nome_arquivo << endl;
            return;
        }

        // Escrever o grau da árvore no arquivo
        size_t grau = arvore.grau;
        arquivo.write(reinterpret_cast<char*>(&grau), sizeof(grau));

        // Serializar a árvore recursivamente, começando pelo nó raiz
        serializar_no(arquivo, arvore.raiz);
        //deletar(arvore.raiz);
        arquivo.close();
    }

};
#endif