#ifndef BPlusTree_H
#define BPlusTree_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cassert>
#include "../estruturas/estruturaBlocoRegistro.hpp"

//Registro que será usado na árvore
struct RegistroBPT{ //VERIFICAR DEPOIS SE DÁ PRA FAZER SETANDO AO INVÉS DOS DOIS PARA DIFERENCIAR MAIS AINDA
    int chave, valor;
    RegistroBPT(int chave, int valor) : chave(chave), valor(valor) {}
    RegistroBPT() : chave(0), valor(0) {} 
};

template <typename T>
struct No {
    bool folha;
    std::size_t grau; 
    std::size_t ocupacao;  
    RegistroBPT* registros;
    No<RegistroBPT>** filhos;
    No<RegistroBPT>* ancestral;

public:
    No(std::size_t _grau) {
        this->folha = false;
        this->grau = _grau;
        this->ocupacao = 0;

        RegistroBPT* registro_aux = new RegistroBPT[grau-1];
        for(int i=0; i<grau-1; i++){
            registro_aux[i] = RegistroBPT(0,0);
        }
        this->registros = registro_aux;

        No<RegistroBPT>** filhos_aux = new No<RegistroBPT>*[grau];
        for(int i=0; i<grau; i++){
            filhos_aux[i] = nullptr;
        }
        this->filhos = filhos_aux;
        this->ancestral = nullptr;
    }
};

template <typename T>
class BPlusTree {
public:
    No<RegistroBPT>* raiz;
    std::size_t grau;

public:
    BPlusTree(std::size_t _grau) {
        this->raiz = nullptr;
        this->grau = _grau;
    }
    ~BPlusTree() {
        deletar(this->raiz);
    }

    No<RegistroBPT>* get_raiz(){
        return this->raiz;
    }

    No<RegistroBPT>* busca_BPlusTree(No<RegistroBPT>* node, int chave_busca){ //Raiz deve ser passada como parâmetro para node
        if(node == nullptr) { // Caso raiz nula
            return nullptr;
        }
        else{
            No<RegistroBPT>* cursor = node; 

            while(cursor->folha == false){ 
                for(int i=0; i<cursor->ocupacao; i++){ 
                    if(chave_busca < cursor->registros[i].chave){ 
                        cursor = cursor->filhos[i];
                        break;
                    }
                    if(i == (cursor->ocupacao -1)){
                        cursor = cursor->filhos[i+1];
                        break;
                    }
                }
            }

            //busca se a chave existe no nó folha
            for(int i=0; i<cursor->ocupacao; i++){
                if(cursor->registros[i].chave == chave_busca){
                    return cursor;
                }
            }
            std::cout<<"No nao encontrado\n";
            return nullptr;
        }
    }
    No<RegistroBPT>* BPlusTreeRangeSearch(No<RegistroBPT>* node, RegistroBPT chave_busca){ //Também usa raiz como parâmetro para node
        if(node == nullptr) { // para raiz nula
            return nullptr;
        }
        else{
            No<RegistroBPT>* cursor = node; 

            while(cursor->folha == false){ 
                for(int i=0; i<cursor->ocupacao; i++){ 
                    if(chave_busca.chave < cursor->registros[i].chave){ 
                        cursor = cursor->filhos[i];
                        break;
                    }
                    if(i == (cursor->ocupacao)-1){
                        cursor = cursor->filhos[i+1];
                        break;
                    }
                }
            }
            return cursor;
        }
    }
    
    bool busca(int chave_busca) {  //checa se registro existe ou não
        return busca_BPlusTree(this->raiz, RegistroBPT(chave_busca,0)) != nullptr;
    }

    int busca_index(RegistroBPT* registros, RegistroBPT registro_buscado, int tamanho){
        int index_registros = 0;
        for(int i=0; i<tamanho; i++){
            if(registro_buscado.chave < registros[i].chave){
                index_registros = i;
                break;
            }
            if(i==tamanho-1){
                index_registros = tamanho;
                break;
            }
        }
        return index_registros;
    }
    RegistroBPT* inserir_registro(RegistroBPT* registros, RegistroBPT registro_insercao, int tamanho){
        int index = 0;
        for(int i=0; i<tamanho; i++){
            if(registro_insercao.chave < registros[i].chave){
                index = i;
                break;
            }
            if(i==tamanho-1){
                index = tamanho;
                break;
            }
        }

        for(int i = tamanho; i > index; i--){
            registros[i] = registros[i-1];
        }

        registros[index] = registro_insercao;

        return registros;
    }
    No<RegistroBPT>** inserir_filho(No<RegistroBPT>** filhos, No<RegistroBPT>*filho,int tamanho,int index){
        for(int i= tamanho; i > index; i--){
            filhos[i] = filhos[i-1];
        }
        filhos[index] = filho;
        return filhos;
    }
    No<RegistroBPT>* inserir_registro_filho(No<RegistroBPT>* no_insercao, RegistroBPT registro_insercao, No<RegistroBPT>* filho){
        int registro_index=0;
        int filho_index=0;
        for(int i=0; i< no_insercao->ocupacao; i++){
            if(registro_insercao.chave < no_insercao->registros[i].chave){
                registro_index = i;
                filho_index = i+1;
                break;
            }
            if(i==no_insercao->ocupacao-1){
                registro_index = no_insercao->ocupacao;
                filho_index = no_insercao->ocupacao+1;
                break;
            }
        }
        for(int i = no_insercao->ocupacao;i > registro_index; i--){
            no_insercao->registros[i] = no_insercao->registros[i-1];
        }
        for(int i=no_insercao->ocupacao+1;i>filho_index;i--){
            no_insercao->filhos[i] = no_insercao->filhos[i-1];
        }

        no_insercao->registros[registro_index] = registro_insercao;
        no_insercao->filhos[filho_index] = filho;

        return no_insercao;
    }
    void inserir_ancestral(No<RegistroBPT>* ancestral,No<RegistroBPT>* filho, RegistroBPT registro_insercao){
        
        No<RegistroBPT>* cursor = ancestral;
        if(cursor->ocupacao < this->grau-1){//checagem de overflow, caso negativo
            cursor = inserir_registro_filho(cursor,registro_insercao,filho);
            cursor->ocupacao++;
        }
        else{//overflow positivo
            auto* novo_no = new No<RegistroBPT>(this->grau);
            novo_no->ancestral = cursor->ancestral;

            RegistroBPT* registros_aux = new RegistroBPT[cursor->ocupacao+1];
            for(int i=0; i<cursor->ocupacao; i++){
                registros_aux[i] = cursor->registros[i];
            }
            registros_aux = inserir_registro(registros_aux,registro_insercao,cursor->ocupacao);

            auto** filhos_aux = new No<RegistroBPT>*[cursor->ocupacao+2];
            for(int i=0; i<cursor->ocupacao+1;i++){
                filhos_aux[i] = cursor->filhos[i];
            }
            filhos_aux[cursor->ocupacao+1] = nullptr;
            filhos_aux = inserir_filho(filhos_aux,filho,cursor->ocupacao+1,busca_index(registros_aux,registro_insercao,cursor->ocupacao+1));

            //divisao dos nós
            cursor->ocupacao = (this->grau)/2;
            if((this->grau) % 2 == 0){
                novo_no->ocupacao = (this->grau) / 2 -1;
            }
            else{
                novo_no->ocupacao = (this->grau) / 2;
            }

            for(int i=0; i<cursor->ocupacao;i++){
                cursor->registros[i] = registros_aux[i];
                cursor->filhos[i] = filhos_aux[i];
            }
            cursor->filhos[cursor->ocupacao] = filhos_aux[cursor->ocupacao];
            //todo 안지워짐. 뒤에것.

            for(int i=0; i < novo_no->ocupacao; i++){
                novo_no->registros[i] = registros_aux[cursor->ocupacao + i +1];
                novo_no->filhos[i] = filhos_aux[cursor->ocupacao+i+1];
                novo_no->filhos[i]->ancestral=novo_no;
            }
            novo_no->filhos[novo_no->ocupacao] = filhos_aux[cursor->ocupacao+novo_no->ocupacao+1];
            novo_no->filhos[novo_no->ocupacao]->ancestral=novo_no;

            RegistroBPT registros_ancestral = registros_aux[this->grau/2];

            delete[] registros_aux;
            delete[] filhos_aux;

            //Balanceamento - ancestral
            if(cursor->ancestral == nullptr){//caso seja raiz, não há n
                auto* novo_ancestral = new No<RegistroBPT>(this->grau);
                cursor->ancestral = novo_ancestral;
                novo_no->ancestral = novo_ancestral;

                novo_ancestral->registros[0] = registros_ancestral;
                novo_ancestral->ocupacao++;

                novo_ancestral->filhos[0] = cursor;
                novo_ancestral->filhos[1] = novo_no;

                this->raiz = novo_ancestral;

                //delete novo_ancestral;  NAO MEXA PORRRA ATE O ULTIMO DIA, ENTÃO DELETE-A
            }
            else{
                inserir_ancestral(cursor->ancestral, novo_no, registros_ancestral);
            }
        }
    }
    void inserir_arvore(RegistroBPT* registro_insercao) {

        RegistroBPT registro_aux(registro_insercao->chave, registro_insercao->valor);

        if(this->raiz == nullptr){ //caso de raiz
            this->raiz = new No<RegistroBPT>(this->grau);
            this->raiz->folha = true;
            this->raiz->registros[0] = *registro_insercao;
            this->raiz->ocupacao = 1; 
        } else { 
            No<RegistroBPT>* cursor = this->raiz;

            //ir até as folhas
            cursor = BPlusTreeRangeSearch(cursor, registro_aux);

            if(cursor->ocupacao < (this->grau-1)){ //caso não haja overflow
                
                cursor->registros = inserir_registro(cursor->registros,*registro_insercao,cursor->ocupacao);
                cursor->ocupacao++;
               
                cursor->filhos[cursor->ocupacao] = cursor->filhos[cursor->ocupacao-1];
                cursor->filhos[cursor->ocupacao-1] = nullptr;
            }
            else{//overflow detectado
                
                auto* novo_no = new No<RegistroBPT>(this->grau);
                novo_no->folha = true;
                novo_no->ancestral = cursor->ancestral;

                RegistroBPT* registros_aux = new RegistroBPT[cursor->ocupacao+1];
                for(int i=0; i<cursor->ocupacao; i++){
                    registros_aux[i] = cursor->registros[i];
                }
                registros_aux = inserir_registro(registros_aux,*registro_insercao,cursor->ocupacao);

                //dividir nós
                cursor->ocupacao = (this->grau)/2;
                if((this->grau) % 2 == 0){
                    novo_no->ocupacao = (this->grau) / 2;
                }
                else{
                    novo_no->ocupacao = (this->grau) / 2 + 1;
                }

                for(int i=0; i<cursor->ocupacao;i++){
                    cursor->registros[i] = registros_aux[i];
                }
                for(int i=0; i < novo_no->ocupacao; i++){
                    novo_no->registros[i] = registros_aux[cursor->ocupacao + i];
                }

                cursor->filhos[cursor->ocupacao] = novo_no;
                novo_no->filhos[novo_no->ocupacao] = cursor->filhos[this->grau-1];
                cursor->filhos[this->grau-1] = nullptr;

                delete[] registros_aux;

                //checagem de ancestral
                RegistroBPT registros_ancestral = novo_no->registros[0];

                if(cursor->ancestral == nullptr){ //caso não haja nó ancestral -> cursor = raiz
                    auto* novo_ancestral = new No<RegistroBPT>(this->grau);
                    cursor->ancestral = novo_ancestral;
                    novo_no->ancestral = novo_ancestral;

                    novo_ancestral->registros[0] = registros_ancestral;
                    novo_ancestral->ocupacao++;

                    novo_ancestral->filhos[0] = cursor;
                    novo_ancestral->filhos[1] = novo_no;

                    this->raiz = novo_ancestral;
                }
                else{
                    inserir_ancestral(cursor->ancestral, novo_no, registros_ancestral);
                }
            }
        }
    }

    void deletar(No<RegistroBPT>* cursor){
        if(cursor != nullptr){
            if(!cursor->folha){
                for(int i=0; i <= cursor->ocupacao; i++){
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
    void imprimir_arvore(){
        imprimir_no(this->raiz);
    }

    void imprime_registro(RegistroBPT registro){
        std::cout<<"Chave: "<< registro.chave <<" Valor: "<< registro.valor <<"\n";
    }

    void imprimir_no(No<RegistroBPT>* cursor) {
        if (cursor != NULL) {
            for (int i = 0; i < cursor->ocupacao; ++i) {
                imprime_registro(cursor->registros[i]);
            }
            std::cout <<"\n";

            if (cursor->folha == false) {
                for (int i = 0; i < cursor->ocupacao + 1; ++i) {
                    imprimir_no(cursor->filhos[i]);
                }
            }
        } 
    }
};

#endif

int main(int argc, char const* argv[]){
    const int max = 3;
    //TESTE COM ARVORE EM MEMÓRIA 
    BPlusTree<RegistroBPT*>arvoreTeste(max);
    RegistroBPT* reg1 = new RegistroBPT(5,4);
    RegistroBPT* reg2 = new RegistroBPT(10,2);
    RegistroBPT* reg3 = new RegistroBPT(15,7);
    RegistroBPT* reg4 = new RegistroBPT(25,6);
    RegistroBPT* reg5 = new RegistroBPT(35,9);
    RegistroBPT* reg6 = new RegistroBPT(45,8);
    
    arvoreTeste.inserir_arvore(reg1);
    arvoreTeste.inserir_arvore(reg2);
    arvoreTeste.inserir_arvore(reg3);
    arvoreTeste.inserir_arvore(reg4);
    arvoreTeste.inserir_arvore(reg5);
    arvoreTeste.inserir_arvore(reg6);

    arvoreTeste.imprimir_no(arvoreTeste.busca_BPlusTree(arvoreTeste.get_raiz(), 25)); //versão nova, busca via valor direto
    arvoreTeste.imprimir_arvore();
} 