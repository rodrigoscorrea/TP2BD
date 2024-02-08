#include <fstream>
#include <iostream>
using namespace std;

template <typename T> //deixando genérico
struct NoBPT {
    int m; // m da árvore, grau
    int n; // ocupação atual do nó
    bool folha; // checa se é uma folha ou não
    T chaves; //chaves do índice
    NoBPT* filho; //ponteiros das chaves
    NoBTP* prox; //ponteiro pra próxima árvore

    /* No(int m) : m(m), n(0), folha(false), item(new Reg[m-1]), filho(new No*[m]){
        for(int i=0; i<m; i++){
            filho[i] = nullptr;
        }
    } não sei se é necessário esse construtor */
};

template <typename T>
class BPTree {
    private:
        NoBPT* raiz;
        int m;
    public:
        //função construtora
        BPTree(int _m) : m(_m) {
            raiz = new NoBPT<T>;
            raiz->folha = true;
        } 
        No* getraiz(){
            return this->raiz;
        }
};