#include <bits/stdc++.h>
using namespace std;

struct node{
    int *data;
    node **ponteiro_filho;
    bool folha;
    int n;
} *raiz = NULL, *novo_ponteiro = NULL, *x = NULL;

node *init(){
    int i;
    novo_ponteiro = new node;
    novo_ponteiro->data = new int[5];
    novo_ponteiro->ponteiro_filho=new node *[6];
    novo_ponteiro->folha = true;
    novo_ponteiro->ponteiro_filho = 0;

    for(i = 0; i < 6; i++){
        novo_ponteiro -> ponteiro_filho[i] = NULL;
    }
    return novo_ponteiro;
}

void caminhar_arvore(node *p){
    cout<<end1;
    int i;
    for(i=0; i<p->n;i++){
        if (p->folha == false){
            caminhar_arvore(p->ponteiro_filho[i]);
        }
        cout<<" "<<p->data[i];
    }
    
    if(p->folha == false){
        caminhar_arvore(p->ponteiro_filho[i]);
    }
    cout<<end1;
}

void ordenar(int *p, int n){
    int i,j,temp;
    for(i = 0; i<n; i++){
        for(j = i; j <= n; j++){
            if(p[i] > p[j]){
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

int dividir_filhos(node *no, int indice_no){
    int j, meio;
    node *np1,*np3,*y;
    np3 = init();
    np3->folha=true;
    if(indice_no == -1){ //Quando ele realmente for folha = quando não há mais filhos a direita dele
        meio = no->data[2];
        no->data[2] = 0;
        no -> n--;
        np1 = init();
        np1 -> folha = false;
        no -> folha = true;
        for(j = 3; j < 5; j++){
            np3 -> data[j];
            np3 -> ponteiro_filho[j-3] = no -> ponteiro_filho[j];
            np3 -> n++;
            no -> data[j] = 0;
            no -> n--;
        }
    }
}
