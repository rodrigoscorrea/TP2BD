#include <bits/stdc++.h>
using namespace std;
#define max 3


class bptree; 
class node{
    bool folha;
    int *chave, tamanho;
    node **ponteiro;
    friend class bptree;

    public:
        node();
};

class bptree{
    node *raiz;
    void inserir_interno(int, node* , node*);
    void remover_interno(int, node*, node* );
    node *achar_pai(node*, node*);
    public: 
        bptree();
        void busca_arvore(int);
        void inserir_arvore(int);
        void remover_arvore(int);
        void imprimir_arvore(node*);
        node* get_raiz();
};

node::node(){
    chave = new int[max];
    ponteiro = new node *[max + 1];

}

bptree::bptree(){
    raiz = NULL;
}

void bptree::inserir_arvore(int x){
    if(raiz == NULL){
        raiz = new node;
        raiz -> chave[0] = x;
        raiz -> folha = true;
        raiz -> tamanho = 1;
    } else {
        node *cursor = raiz;
        node *pai;
        while(cursor -> folha == false){
            pai = cursor;
            for(int i = 0; i < cursor -> tamanho; i++){
                if(x < cursor ->chave[i]){
                    cursor = cursor -> ponteiro[i];
                    break;
                }
                if(i == cursor -> tamanho-1){
                    cursor = cursor->ponteiro[i+1];
                    break;
                }
            }
        }
        if (cursor->tamanho < max){
            int i = 0;
            while(x > cursor -> chave[i] && i<cursor->tamanho)
            i++;
            for(int j = cursor -> tamanho; j > i; j--){
                cursor->chave[j] = cursor->chave[j-1];
            }
            cursor->chave[i] = x;
            cursor->tamanho++;
            cursor->ponteiro[cursor->tamanho] = cursor->ponteiro[cursor->tamanho-1];
            cursor->ponteiro[cursor->tamanho-1] = NULL;
        } else {
            node *nova_folha = new node;
            int no_virtual[max+1];
            for(int i = 0; i < max; i++){
                no_virtual[i] = cursor->chave[i];
            }
            int i = 0, j;
            while(x > no_virtual[i] && i < max)
            i++;
            for(int j = max + 1; j > i; j--){
                no_virtual[j] = no_virtual[j-1];
            }
            no_virtual[i] = x;
            nova_folha->folha = true;
            cursor->tamanho=(max+1)/2;
            nova_folha->tamanho = max+1-(max+1)/2;
            cursor->ponteiro[cursor->tamanho] = nova_folha;
            nova_folha->ponteiro[nova_folha->tamanho] = cursor->ponteiro[max];
        }
    }
}

void bptree::inserir_interno(int x, node* cursor, node* filho){
    if(cursor -> tamanho < max){
        int i = 0;
        while (x > cursor->chave[i] && i<cursor->tamanho)
        i++;
        for(int j = cursor->tamanho;j > i ; j--){
            cursor -> chave[j] = cursor->chave[j - 1];
        }
        cursor-> chave[i] = x;
        cursor->tamanho++;
        cursor->ponteiro[i+1] = filho;
    } else {
        node* novo_no_interno = new node;
        int chave_virtual[max+1];
        //node* chave_virtual_no[max+1];
        node* ponteiro_virtual[max+2];
        for(int i = 0; i < max; i++){
            chave_virtual[i] = cursor -> chave[i];
        }
        for(int i = 0; i < max+1; i++){
            ponteiro_virtual[i] = cursor -> ponteiro[i];
        }
    }
}

node* bptree::achar_pai(node* cursor, node* filho){
    node* pai;
    if (cursor -> folha || (cursor->ponteiro[0]) -> folha) {
        return NULL; //Raiz não tem pai
    }

    for(int i = 0; i < (cursor->tamanho+1); i++){
        if(cursor -> ponteiro[i] == filho){
            pai = cursor;
            return pai;
        } else {
            pai = achar_pai(cursor->ponteiro[i], filho);
            if (pai != NULL){
                return pai;
            }
        } 
    }
    return pai;
}
void bptree::remover_interno(int x, node* cursor, node* filho) {
    int pos;
    for (pos = 0; pos < cursor->tamanho; pos++) {
        if (cursor->chave[pos] == x) {
            break;
        }
    }

    for (int i = pos; i < cursor->tamanho - 1; i++) {
        cursor->chave[i] = cursor->chave[i + 1];
    }

    for (int i = pos + 1; i < cursor->tamanho; i++) {
        cursor->ponteiro[i] = cursor->ponteiro[i + 1];
    }

    cursor->tamanho--;

    /* if (cursor->tamanho < (max + 1) / 2) {
        // Reorganizar a árvore se o número de chaves ficar abaixo do mínimo permitido
        // (nesse ponto, a redistribuição ou a fusão de nós podem ser necessárias)
        // Implementação de reorganização não incluída nesta resposta
    } */
}
void bptree::remover_arvore(int x){
    if(raiz == NULL){
        cout<<"arvore esta vazia";
    } else {
        node* cursor = raiz;
        node* pai;
        int irmao_esquerda, irmao_direita;
        while(cursor->folha == false){
            for(int i = 0; i < cursor->tamanho; i++){
                pai = cursor;
                irmao_esquerda = i-1;
                irmao_direita = i+1;
                if(x < cursor->chave[i]){
                    cursor = cursor->ponteiro[i];
                    break;
                }
                if(i == cursor->tamanho-1){
                    irmao_esquerda = i;
                    irmao_direita = i+2;
                    cursor = cursor->ponteiro[i+1];
                    break;
                }
            }
        }
        bool achado = false;
        int pos;
        for(pos = 0; pos < cursor -> tamanho; pos++){
            if(cursor->chave[pos] == x){
                achado = true;
                break;
            }
        }
        if(!achado){
            cout<<"nao foi achado\n";
            return;
        }
        if(cursor -> tamanho == 0){
            cout<<"arvore morreu\n";
            delete[] cursor->chave;
            delete[] cursor->ponteiro;
            delete[] cursor;
            raiz = NULL;  
        }
        //return;
        node* no_direita = cursor->ponteiro[irmao_direita];
        for(int i = cursor -> tamanho, j =0; j< no_direita -> tamanho; i++,j++){
            cursor->chave[i] = no_direita->chave[j];
        }
        cursor->ponteiro[cursor->tamanho] = NULL;
        cout<<"agrupando dois nos folhas\n";
        remover_interno(pai->chave[irmao_direita-1], pai, no_direita);
        delete[] no_direita->chave;
        delete[] no_direita->ponteiro;
        delete[] no_direita;
    }
}

void bptree::imprimir_arvore(node* cursor){
    if(cursor != NULL){
        for(int i = 0; i < cursor->tamanho; i++){
            cout<<cursor->chave[i]<<" ";
        }
        cout<<endl;
        if(cursor->folha != true){
            for(int i=0; i< cursor->tamanho;i++){
                imprimir_arvore(cursor->ponteiro[i]);
            }
        }
    }
}

node *bptree::get_raiz(){
    return raiz;
}
int main(){
    
    bptree node;
    node.inserir_arvore(5);
    node.inserir_arvore(15);
    node.inserir_arvore(25);
    node.inserir_arvore(35);
    node.inserir_arvore(45);
    
    node.imprimir_arvore(node.get_raiz());
    //node.remover_arvore(15);
    //node.imprimir_arvore(node.get_raiz());
    return 0;
}