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

class BPlusTree {
public:
    No<RegistroBPT>* raiz;
    std::size_t grau;

public:
    BPlusTree(std::size_t _grau) {
        this->raiz = nullptr;
        this->grau = _grau;
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
    
    /* No<RegistroBPT>* busca(int chave_busca) {  //checa se registro existe ou não
        return busca_BPlusTree(this->raiz, RegistroBPT(chave_busca,0));
    } */

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
        } else {
            cout<<"No nulo\n";
        }
    }
    void transferir_arvore_bin(const BPlusTree& tree, const string& nome_arquivo_bin){
        ofstream arquivo_bin(nome_arquivo_bin, ios::binary | ios::out);
        if(arquivo_bin){
            size_t grau = tree.grau;
            bool folha;
            size_t ocupacao;
            arquivo_bin.write(reinterpret_cast<char*>(&grau), sizeof(grau));

            stack<No<RegistroBPT>*> pilha_no;
            pilha_no.push(tree.raiz);
            No<RegistroBPT>* cursor;

            while(!pilha_no.empty()){
                cursor = pilha_no.top();
                pilha_no.pop();

                folha = cursor->folha;
                ocupacao = cursor->ocupacao;

                arquivo_bin.write(reinterpret_cast<const char*>(&folha), sizeof(folha));
                arquivo_bin.write(reinterpret_cast<const char*>(&ocupacao), sizeof(ocupacao));
                arquivo_bin.write(reinterpret_cast<const char*>(cursor->registros), sizeof(RegistroBPT) * (cursor->grau - 1));

                if(!cursor->folha){
                    for(int i = 0; i <= cursor->ocupacao; i++){
                        pilha_no.push(cursor->filhos[i]);
                    }
                }
            }
            arquivo_bin.close(); 
        } else {
            cout<<"Aconteceu algum erro com o arquivo binario\n";
        }
    }

    
    /* void serializeBPlusTree(const BPlusTree& tree, const string& filename) {
        ofstream file(filename, ios::binary | ios::out);
        if (!file) {
            cerr << "Error opening file for serialization: " << filename << endl;
            return;
        }

        // Escrever o grau da árvore no arquivo
        size_t degree = tree.grau;
        file.write(reinterpret_cast<char*>(&degree), sizeof(degree));

        // Serializar a árvore recursivamente, começando pelo nó raiz
        serializeNode(file, tree.raiz);
        deletar(tree.raiz);
        file.close();
    }

    // Função recursiva para serializar um nó e seus filhos
    void serializeNode(ofstream& file, const No<RegistroBPT>* node) {
        // Escrever as informações do nó no arquivo (is_leaf e size)
        bool is_leaf = node->folha;
        size_t size = node->ocupacao;
        file.write(reinterpret_cast<const char*>(&is_leaf), sizeof(is_leaf));
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        // Escrever os itens do nó no arquivo (registros)
        file.write(reinterpret_cast<const char*>(node->registros), sizeof(RegistroBPT) * (node->grau - 1));

        if (!is_leaf) {
            // Serializar os nós filhos recursivamente
            for (size_t i = 0; i <= size; ++i) {
                serializeNode(file, node->filhos[i]);
            }
        }
    } */

    No<RegistroBPT>* deserializeNode(ifstream& file, No<RegistroBPT>* parent, size_t degree) {
        // Ler as informações do nó do arquivo
        bool is_leaf;
        size_t size;
        if (!file.read(reinterpret_cast<char*>(&is_leaf), sizeof(is_leaf)) ||
            !file.read(reinterpret_cast<char*>(&size), sizeof(size))) {
            cerr << "Error reading node information from file." << endl;
            return nullptr;
        }

        // Criar um novo nó
        auto* node = new No<RegistroBPT>(degree);
        node->folha = is_leaf;
        node->ocupacao = size;
        node->ancestral = parent;

        // Ler os itens do nó do arquivo
        node->registros = new RegistroBPT[degree - 1];
        if (!file.read(reinterpret_cast<char*>(node->registros), sizeof(RegistroBPT) * (degree - 1))) {
            cerr << "Error reading node items from file." << endl;
            delete[] node->registros;
            delete node;
            return nullptr;
        }

        if (!is_leaf) {
            // Desserializar os nós filhos recursivamente
            node->filhos = new No<RegistroBPT>*[degree];
            for (size_t i = 0; i <= size; ++i) {
                node->filhos[i] = deserializeNode(file, node, degree);
                if (!node->filhos[i]) {
                    cerr << "Error deserializing child node from file." << endl;
                    destroyNode(node); // Liberar a memória alocada
                    return nullptr;
                }
            }
        }
        return node;
    }
    void destroyNode(No<RegistroBPT>* node) {
        if (node) {
            if (!node->folha) {
                for (size_t i = 0; i <= node->ocupacao; ++i) {
                    destroyNode(node->filhos[i]);
                }
                delete[] node->filhos;
            }
            delete[] node->registros;
            delete node;
        }
    }

    BPlusTree deserializeBPlusTree(const string& filename) {
        ifstream file(filename, ios::binary | ios::in);
        if (!file) {
            cerr << "Error opening file for deserialization: " << filename << endl;
            return BPlusTree(0);  // Retornar uma árvore B+ vazia
        }

        // Ler o grau da árvore do arquivo
        size_t degree;
        if (!file.read(reinterpret_cast<char*>(&degree), sizeof(degree))) {
            cerr << "Error reading degree from file: " << filename << endl;
            file.close();
            return BPlusTree(0);  // Retornar uma árvore B+ vazia
        }

        // Criar uma nova árvore B+ com o grau fornecido
        BPlusTree tree(degree);

        // Desserializar a árvore recursivamente, começando pelo nó raiz
        tree.raiz = deserializeNode(file, nullptr, degree);
        if (!tree.raiz) {
            cerr << "Error deserializing root node from file: " << filename << endl;
            file.close();
            return BPlusTree(0);  // Retornar uma árvore B+ vazia
        }

        file.close();

        return tree;
    }
    
    /* BPlusTree trazer_arvore_bin(const BPlusTree& tree ,const string& nome_arquivo_bin){
        ifstream arquivo_bin(nome_arquivo_bin, ios::binary | ios::in);
        if(arquivo_bin){
            size_t grau;
            if(!arquivo_bin.read(reinterpret_cast<char*>(&grau), sizeof(grau))){
                cout<<"Aconteceu um erro com a leitura do arquivo\n";
                arquivo_bin.close();
                return BPlusTree(0);
            }

            BPlusTree arvore_transferida(grau);

            stack<pair<No<RegistroBPT>*, No<RegistroBPT>*>> pilha_no;
            pilha_no.push({nullptr, arvore_transferida.raiz});
            //auto topo_pilha = pilha_no.top();
            No<RegistroBPT>* cursor;// = topo_pilha.second;
            No<RegistroBPT>* ancestral;// = topo_pilha.first;
            vector<RegistroBPT> vetor_disco;
            set<RegistroBPT> set_insercao;

            while (!pilha_no.empty()) {
                auto topo_pilha = pilha_no.top();
                ancestral = topo_pilha.first;
                cursor = topo_pilha.second;
                pilha_no.pop();
                
                // Ler as informações do nó do arquivo
                bool folha;
                size_t ocupacao;
                if (!arquivo_bin.read(reinterpret_cast<char*>(&folha), sizeof(folha)) ||
                    !arquivo_bin.read(reinterpret_cast<char*>(&ocupacao), sizeof(ocupacao))) {
                    cerr << "Error reading node information from file." << endl;
                    arquivo_bin.close();
                    destroyNode(arvore_transferida.raiz); // Liberar a memória alocada
                    return BPlusTree(0);    // Retornar uma árvore B+ vazia
                }

                // Criar um novo nó
                auto* aux = new No<RegistroBPT>(grau);
                aux->folha = folha;
                aux->ocupacao = ocupacao;
                aux->ancestral = ancestral;

                // Ler os itens do nó do arquivo
                aux->registros = new RegistroBPT[grau - 1];
                if (!arquivo_bin.read(reinterpret_cast<char*>(aux->registros), sizeof(RegistroBPT) * (grau - 1))) {
                    cerr << "Error reading node registross from arquivo_bin." << endl;
                    //arquivo_bin.close();
                    //destroyNode(arvore_transferida.raiz); // Liberar a memória alocada
                    delete[] aux->registros;
                    delete aux;
                    return BPlusTree(0);    // Retornar uma árvore B+ vazia
                } 
                for(int j = 0; j < aux->ocupacao; j++){
                    vetor_disco.push_back(aux->registros[j]);
                }

                if (!folha) {
                    // Adicionar os nós filhos à pilha
                    aux->filhos = new No<RegistroBPT>*[grau];
                    for (size_t i = 0; i <= ocupacao; ++i) {
                        pilha_no.push({aux, aux->filhos[i]});
                    } 
                } 

                // Atualizar o ponteiro do pai para os filhos
               /*  if (ancestral) {
                    ancestral->filhos[ocupacao] = aux;
                } else {
                    // Atualizar o nó raiz
                    arvore_transferida.raiz = aux;
                }
            }
            arquivo_bin.close();
            return arvore_transferida;
        } else {
            cout<<"Aconteceu algum erro com o arquivo binário\n";
        }
    } */

};
#endif

int main(int argc, char const* argv[]){
    const int max = 3;
    //TESTE COM ARVORE EM MEMÓRIA 
    BPlusTree arvoreTeste(max);
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

    //arvoreTeste.imprimir_arvore();
    const string nome_arquivo = "indice.bin";
    //arvoreTeste.serializeBPlusTree(arvoreTeste, nome_arquivo);
    arvoreTeste.transferir_arvore_bin(arvoreTeste,nome_arquivo);
    //BPlusTree novaArvore = novaArvore.deserializeBPlusTree("indice.bin");
    BPlusTree novaArvore = novaArvore.deserializeBPlusTree(nome_arquivo);
    novaArvore.imprimir_arvore();
} 