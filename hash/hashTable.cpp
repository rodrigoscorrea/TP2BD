#include <iostream>
#include <vector>
#include <memory>
#include "string"
#include "../estruturas/estruturaBlocoRegistro.hpp"
using namespace std;
#define BUCKETS 20023
#define BLOCOS 23

struct Bucket{
    Bloco* blocos[BLOCOS];
    int qtd_blocos;
};

void deletarBucket(Bucket* bucket){
    for (int i = 0; i < bucket->qtd_blocos; i++)
    {
        deletar_bloco(bucket->blocos[i]);
    }
    delete bucket;
}

void inicializarBucket(ofstream& arquivoBin)
{
    Bucket* bucket = new Bucket();
    bucket->qtd_blocos = 0;
    for (int i = 0; i < BLOCOS ; i++)
    {
        bucket->blocos[i] = criar_bloco();
        bucket->qtd_blocos++;
        arquivoBin.write(reinterpret_cast<char*>(bucket->blocos[i]->cabecalho), sizeof(Cabecalho_Bloco));
        arquivoBin.write(reinterpret_cast<char*>(bucket->blocos[i]->dados), TAM_BLOCO);
    }
    delete bucket;
}

class hashTable
{
    private:
        streampos offsetsBucket[BUCKETS]; 
        ofstream arquivoBin;
    public:
        hashTable(string nomeArquivo, bool sobrescrever);
        int funcaoHash(unsigned int id);
        void inserirRegistroBloco(Bloco* bloco, Registro* reg);
        int inserirRegistroBucket(Registro* reg);
        Registro* busca_registro_hashtable(int id);
        ifstream arquivoIn;
};

hashTable::hashTable(string nomeArquivo, bool sobrescrever = true)
{   
    if (sobrescrever) 
    {
        ofstream _arquivoBin(nomeArquivo, ios::binary | ios::out);
        if (!arquivoBin) cout << "Erro ao criar arquivo de dados" << endl;
        
        this->arquivoBin = move(_arquivoBin);

        for (int i = 0; i < BUCKETS; i++)
        {
            this->offsetsBucket[i] = this->arquivoBin.tellp();
            inicializarBucket(this->arquivoBin);
        }
    }
    else
    {
        ifstream _arquivoIn(nomeArquivo, ios::in);
        if (!_arquivoIn) cout << "Erro ao abrir arquivo de entrada" << endl;

        this->arquivoIn = move(_arquivoIn);
        for (int i = 0; i < BUCKETS; i++) this->offsetsBucket[i] = this->arquivoIn.tellg();

    }

}

int hashTable::funcaoHash(unsigned int id) { 
    unsigned int primoAleatorio = 20021;
    return (id * primoAleatorio) % BUCKETS;
}

int hashTable::inserirRegistroBucket(Registro* registro) 
{
    int chave = funcaoHash(registro->id);
    bool regInserido = false;
    streampos posicao = this->offsetsBucket[chave];

    this->arquivoIn.seekg(posicao);

    for (int i = 0; i < BLOCOS && !regInserido; i++) 
    {
        Bloco* bloco = criar_bloco();

        this->arquivoIn.read(reinterpret_cast<char*>(bloco->cabecalho), sizeof(Cabecalho_Bloco));
        this->arquivoIn.read(reinterpret_cast<char*>(bloco->dados), TAM_BLOCO);

        if (bloco->cabecalho->tamanho_disponivel >= registro->ocupacao) 
        {
            int endereco = (int) posicao + (int) sizeof(Cabecalho_Bloco) + bloco->cabecalho->posicoes_registros[bloco->cabecalho->quantidade_registros];
            
            // Inserir registro no bloco
            bloco = inserir_registro_bloco(bloco, registro);
            
            regInserido = true;

            // Preparar buffer para escrever no arquivo
            char buffer[sizeof(Cabecalho_Bloco) + TAM_BLOCO];
            memcpy(buffer, bloco->cabecalho, sizeof(Cabecalho_Bloco));
            memcpy(buffer + sizeof(Cabecalho_Bloco), bloco->dados, TAM_BLOCO);

            // Escrever bloco de volta no arquivo

            this->arquivoBin.seekp(posicao);
            this->arquivoBin.write(reinterpret_cast<char*>(buffer), TAM_BLOCO + sizeof(Cabecalho_Bloco));

            return endereco;
        }

        deletar_bloco(bloco);

        posicao += sizeof(Cabecalho_Bloco) + TAM_BLOCO;
        this->arquivoIn.seekg(posicao);
    }

    if (!regInserido) 
    {
        cout << "Overflow" << endl;
    }

    return -1;
}

Registro* hashTable::busca_registro_hashtable(int id) 
{
    int posicao_bucket = funcaoHash(id);
    streampos posicao = posicao_bucket * (sizeof(Cabecalho_Bloco) + TAM_BLOCO) * BLOCOS;

    this->arquivoIn.seekg(posicao);

    for (int i = 0; i < BLOCOS; i++) 
    {
        Bloco* bloco = criar_bloco();

        this->arquivoIn.read(reinterpret_cast<char*>(bloco->cabecalho), sizeof(Cabecalho_Bloco));
        this->arquivoIn.read(reinterpret_cast<char*>(bloco->dados), TAM_BLOCO);

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

                    cout << "Quantidade de Blocos Lidos: " << (i + 1) << endl;
                    cout << "Quantidade de Blocos Totais: " << BLOCOS * BUCKETS << endl;
                    return registro_aux;
                }

                delete registro_aux;
            }
        }

        deletar_bloco(bloco);
        posicao += sizeof(Cabecalho_Bloco) + TAM_BLOCO;
        this->arquivoIn.seekg(posicao);
    }

    cout << "Registro nao encontrado no arquivo" << endl;
    return nullptr;
}