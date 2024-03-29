#include <iostream>
#include <vector>
#include <memory>
#include "string"
#include "../estruturas/estruturaBlocoRegistro.hpp"
#include "../estruturas/definicoes.hpp"
using namespace std;

struct Bucket{
    Bloco* blocos[BLOCOS];
    int qtd_blocos;
};

void deletar_bucket(Bucket* bucket){
    for (int i = 0; i < bucket->qtd_blocos; i++)
    {
        deletar_bloco(bucket->blocos[i]);
    }
    delete bucket;
}

void inicializar_bucket(ofstream& arquivoBin)
{
    Bucket* bucket = new Bucket();
    bucket->qtd_blocos = 0;
    for (int i = 0; i < BLOCOS ; i++)
    {
        bucket->blocos[i] = criar_bloco();
        bucket->qtd_blocos++;
        arquivoBin.write(reinterpret_cast<char*>(bucket->blocos[i]->cabecalho), sizeof(Cabecalho_Bloco));
        arquivoBin.write(reinterpret_cast<char*>(bucket->blocos[i]->dados), TAM_BLOCO - sizeof(Cabecalho_Bloco));
    }
    deletar_bucket(bucket);
}

class HashTable
{
    private:
        ofstream arquivoBin;
    public:
        HashTable(string nomeArquivo, bool sobrescrever);
        size_t funcao_hash(size_t id);
        size_t inserir_registro_bucket(Registro* reg);
        Registro* busca_registro_hashtable(int id);
        void media_registros();
        ifstream arquivoIn;
        int qtd_registros = 0; 
        float resultado = 0.0;
        float soma_ocupacao = 0.0;
};

void HashTable::media_registros(){
    cout<< "A media em bytes de um registro eh de: " << this->soma_ocupacao / this->qtd_registros << endl; 
}

HashTable::HashTable(string nomeArquivo, bool sobrescrever = true)
{   
    if (sobrescrever) 
    {
        ofstream _arquivoBin(nomeArquivo, ios::binary | ios::out);
        if (!_arquivoBin) cout << "Erro ao criar arquivo de dados" << endl;
        
        this->arquivoBin = std::move(_arquivoBin);

        for (int i = 0; i < BUCKETS; i++)
        {
            inicializar_bucket(this->arquivoBin);
        }

        ifstream _arquivoIn(nomeArquivo, ios::in);
        if (!_arquivoIn) cout << "Erro ao abrir arquivo de entrada" << endl;

        this->arquivoIn = std::move(_arquivoIn);
    }
    else
    {
        ifstream _arquivoIn(nomeArquivo, ios::in);
        if (!_arquivoIn) cout << "Erro ao abrir arquivo de entrada" << endl;

        this->arquivoIn = std::move(_arquivoIn);
    }

}

size_t HashTable::funcao_hash(size_t id) { 
    size_t primoAleatorio = 37;
    return (id * primoAleatorio) % BUCKETS;
}

size_t HashTable::inserir_registro_bucket(Registro* registro) 
{
    bool regInserido = false;

    size_t chave = funcao_hash(registro->id);
    size_t posicao = chave * TAM_BLOCO * BLOCOS;

    this->arquivoIn.seekg(posicao);

    for (size_t bloco_atual = 0; bloco_atual < BLOCOS; bloco_atual++) 
    {   
        Bloco* bloco = criar_bloco();

        this->arquivoIn.read(reinterpret_cast<char*>(bloco->cabecalho), sizeof(Cabecalho_Bloco));
        this->arquivoIn.read(reinterpret_cast<char*>(bloco->dados), TAM_BLOCO - sizeof(Cabecalho_Bloco));

        if (bloco->cabecalho->tamanho_disponivel >= registro->ocupacao && bloco->cabecalho->quantidade_registros < BLOCO_REGISTROS) 
        {   
            size_t endereco = (size_t) posicao + 
                              (size_t) (bloco_atual * TAM_BLOCO) + 
                              (size_t) sizeof(Cabecalho_Bloco) + 
                              (size_t) bloco->cabecalho->posicoes_registros[bloco->cabecalho->quantidade_registros];

            // Inserir registro no bloco
            bloco = inserir_registro_bloco(bloco, registro);
            this->qtd_registros++;
            soma_ocupacao+= registro->ocupacao;
            regInserido = true;
            // Preparar buffer para escrever no arquivo
            char buffer[TAM_BLOCO];
            memcpy(buffer, bloco->cabecalho, sizeof(Cabecalho_Bloco));
            memcpy(buffer + sizeof(Cabecalho_Bloco), bloco->dados, TAM_BLOCO - sizeof(Cabecalho_Bloco));

            // Escrever bloco de volta no arquivo
            this->arquivoBin.seekp(posicao + (bloco_atual * TAM_BLOCO));
            this->arquivoBin.write(reinterpret_cast<char*>(buffer), TAM_BLOCO);

            deletar_bloco(bloco);
            return endereco;
        }

        deletar_bloco(bloco);
    }

    if (!regInserido) cout << "Overflow: Sem espaço suficiente para escrever no bucket" << endl;

    return -1;
}

Registro* HashTable::busca_registro_hashtable(int id) 
{
    size_t chave = funcao_hash(id);
    size_t posicao = chave * TAM_BLOCO * BLOCOS;
    this->arquivoIn.seekg(posicao);

    for (int bloco_atual = 0; bloco_atual < BLOCOS; bloco_atual++) 
    {
        Bloco* bloco = criar_bloco();

        this->arquivoIn.read(reinterpret_cast<char*>(bloco->cabecalho), sizeof(Cabecalho_Bloco));
        this->arquivoIn.read(reinterpret_cast<char*>(bloco->dados), TAM_BLOCO - sizeof(Cabecalho_Bloco));

        if (bloco->cabecalho->quantidade_registros > 0)
        {
            for (int j = 0; j < bloco->cabecalho->quantidade_registros; j++)
            {
                int cursor = bloco->cabecalho->posicoes_registros[j];
                Registro* registro_aux = new Registro();
                
                memcpy(&registro_aux->id, &bloco->dados[cursor], sizeof(int));

                if (registro_aux->id == id)
                {
                    cursor += sizeof(int);
                    preencher_registro(registro_aux, bloco, cursor);  
                    deletar_bloco(bloco);

                    cout << "Quantidade de Blocos Lidos: " << (bloco_atual + 1) << endl;
                    cout << "Quantidade de Blocos Totais: " << BLOCOS * BUCKETS << endl;
                    return registro_aux;
                }

                delete registro_aux;
            }
        }

        deletar_bloco(bloco);
        this->arquivoIn.seekg(posicao + (bloco_atual * TAM_BLOCO));
    }

    cout << "Registro nao encontrado no arquivo" << endl;
    return nullptr;
}