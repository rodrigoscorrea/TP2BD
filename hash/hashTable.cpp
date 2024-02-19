#include <iostream>
#include <vector>
#include <memory>
#include "string"
#include "../estruturas/estruturaBlocoRegistro.hpp"
using namespace std;
#define BUCKETS 1000
#define BLOCOS 12

struct Bucket{
    Bloco* blocos[BLOCOS]; //12 blocos por bucket
    int qtd_blocos;
};

void deletarBucket(Bucket* bucket){
    for (int i = 0; i < bucket->qtd_blocos; i++){
        deletar_bloco(bucket->blocos[i]);
    }
    delete bucket;
}

void inicializarBucket(ofstream& arquivoBin){
    Bucket* bucket = new Bucket();
    bucket->qtd_blocos = 0;
    for(int i=0; i < BLOCOS ; i++){
        bucket->blocos[i] = criar_bloco();
        bucket->qtd_blocos++;

        arquivoBin.write(reinterpret_cast<char*>(bucket->blocos[i]->cabecalho), sizeof(Cabecalho_Bloco));
        arquivoBin.write(reinterpret_cast<char*>(bucket->blocos[i]->dados), sizeof(Bloco)-sizeof(Cabecalho_Bloco));
        
    }
    delete bucket; //retira o bucket e os blocos da memória
}

class hashTable{
private:
    streampos offsetsBucket[BUCKETS]; 
    ofstream arquivoBin;
    ifstream arquivoIn;
public:
    hashTable(string nomeArquivo);
    int funcaoHash(int id);
    void inserirRegistroBloco(Bloco* bloco, Registro* reg);
    void inserirRegistroBucket(Registro* reg);
    Registro* busca_registro_hashtable(int id);
};

hashTable::hashTable(string nomeArquivo){
    ofstream _arquivoBin(nomeArquivo, ios::binary | ios::out);
    if(!arquivoBin){
        cout << "Erro ao criar arquivo de dados" << endl;
    }
    this->arquivoBin = move(_arquivoBin);
    ifstream _arquivoIn(nomeArquivo, ios::in);
    if (!_arquivoIn) {
        cout << "Erro ao abrir arquivo de entrada" << endl;
    }
    this->arquivoIn = move(_arquivoIn);
    for (int i = 0; i < BUCKETS; i++){
        this->offsetsBucket[i] = this->arquivoBin.tellp();
        inicializarBucket(this->arquivoBin);
    }
}

int hashTable::funcaoHash(int id){ return id%BUCKETS;}

void hashTable::inserirRegistroBucket(Registro* registro){
    //fazer o hash para pegar o bucket correspondente
    int chave = funcaoHash(registro->id);
    streampos posicao = this->offsetsBucket[chave];
    //abre o arquivo na posição do bucket
    this->arquivoIn.seekg(posicao);
    //traz o bucket pra memória
    
    Bloco* bloco = criar_bloco();
    this->arquivoIn.read(reinterpret_cast<char*>(bloco->cabecalho), sizeof(Cabecalho_Bloco));
    this->arquivoIn.read(reinterpret_cast<char*>(bloco->dados), sizeof(Bloco) - sizeof(Cabecalho_Bloco));

    bool regInserido = false;
    for (int i = 0; i < BLOCOS; i++){
        if(bloco->cabecalho->tamanho_disponivel >= sizeof(Registro)){
            bloco = inserir_registro_bloco(bloco,registro);
            regInserido = true;
            
            char buffer[TAM_BLOCO];
            memcpy(buffer, bloco->cabecalho, sizeof(Cabecalho_Bloco));
            memcpy(buffer + sizeof(Cabecalho_Bloco), bloco->dados, TAM_BLOCO - sizeof(Cabecalho_Bloco));

            this->arquivoBin.seekp(posicao);
            this->arquivoBin.write(reinterpret_cast<char*>(buffer),TAM_BLOCO);
            break;
        }
    }
    if(!regInserido){
        cout<<"Overflow"<<endl;
        return;
    }
    deletar_bloco(bloco);
}

Registro* hashTable::busca_registro_hashtable(int id){
    for(int i = 0; i < BLOCOS; i++){
        Bloco* aux = criar_bloco();
        int posicao_bucket = funcaoHash(id);
        streampos posicao = this->offsetsBucket[posicao_bucket];

        this->arquivoIn.seekg(posicao);
        this->arquivoIn.read(reinterpret_cast<char*>(aux->cabecalho), sizeof(Cabecalho_Bloco));
        this->arquivoIn.read(reinterpret_cast<char*>(aux->dados), sizeof(Bloco) - sizeof(Cabecalho_Bloco));

        if(aux->cabecalho->quantidade_registros == 0){
            cout<<"Nao ha registros no bloco"<<endl;
            return nullptr;
        } 

        Registro* registro_aux = new Registro();
        for(int i = 0; i < aux->cabecalho->quantidade_registros; i++){
            int cursor = aux->cabecalho->posicoes_registros[i];
            memcpy(&registro_aux->id, &aux->dados[cursor],sizeof(int));
            
            if(id == registro_aux->id){
                cursor += sizeof(int);
                preencher_registro(registro_aux, aux,cursor);   
                deletar_bloco(aux);
                return registro_aux;
            }
        }
        deletar_bloco(aux);
    }
    cout<<"Registro nao encontrado no arquivo\n";
    return nullptr;
}