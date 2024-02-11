/* #include <bits/stdc++.h>

using namespace std;

struct node{
    int *data;
    node **ponteiro_filho;
    bool folha;
    int n;
} *raiz = NULL, *np = NULL, *no = NULL;

node *init(){
    int i;
    np = new node;
    np->data = new int[5];
    np->ponteiro_filho=new node *[6];
    np->folha = true;
    np->ponteiro_filho = 0;

    for(i = 0; i < 6; i++){
        np -> ponteiro_filho[i] = NULL;
    }
    return np;
}

void caminhar_arvore(node *p){
    cout<<endl;
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
    cout<<endl;
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

int dividir_filhos(node *x, int indice_no){
    int j, meio;
    node *np1,*np3,*y;
    np3 = init();
    np3->folha=true;
    if(indice_no == -1){ //Quando ele realmente for folha = quando não há mais filhos a direita dele
        meio = x->data[2];
        x->data[2] = 0;
        x -> n--;
        np1 = init();
        np1 -> folha = false;
        x -> folha = true;
        for(j = 3; j < 5; j++){
            np3 -> data[j-3] = x-> data[j];
            np3 -> ponteiro_filho[j-3] = x -> ponteiro_filho[j];
            np3 -> n++;
            x -> data[j] = 0;
            x -> n--;
        }
        for(j = 0; j < 6; j++){
            x -> ponteiro_filho[j] = NULL;
        }
        np1 -> data[0] = meio;
        np1 -> ponteiro_filho[np1->n] = x;
        np1 -> ponteiro_filho[np1 -> n+1] = np3;
        np1 -> n++;
        raiz = np1;
    }
    else {
        y = x -> ponteiro_filho[indice_no];
        meio = y -> data[2];
        y -> data[2] = 0;
        y -> n--;
        for(j = 3; j < 5; j++){
            np3 -> data[j - 3]= y-> data[j];
            np3 -> n++;
            y -> data[j] = 0;
            y -> n--;
        }
        x -> ponteiro_filho[indice_no+1] = y;
        x -> ponteiro_filho[indice_no+1] = np3;
    }
    return meio;
}

void inserir(int a){
    int i,temp;
    no = raiz;
    if (no == NULL){
        raiz = init();
        no = raiz;
    } else {
        if(no -> folha == true && no -> n == 5){
            temp = dividir_filhos(no, -1);
            no = raiz;
            for(i = 0; i < (no -> n); i++){
                if((a > no -> data[i]) && (a<no->data[i+1])){
                    i++;
                    break;
                } else if (a < no -> data[0]) {
                    break;
                } else {
                    continue;
                }
            }
            no = no -> ponteiro_filho[i];
        } 
        else {
            while(no -> folha == false){
                for(i = 0; i < (no -> n); i++){
                    if((a > no -> data[i]) && (a < no -> data[i+1])){
                        i++;
                        break;
                    }
                    else if (a < (no -> data[0])) {
                        break;
                    } else {
                        continue;
                    }
                }
                if(no -> ponteiro_filho[i] -> n == 5){
                    temp = dividir_filhos(no, i);
                    no -> data[no->n] = temp;
                    no -> n++;
                    continue;
                }
                else {
                    no = no -> ponteiro_filho[i];
                }
            }
        }  
    }
    no -> data[no -> n] = a;
    ordenar(no -> data, no->n);
    no -> n++;
}

int main(){
    int i, n, t;
    cout<<"insira numero de elementos para serem inseridos: \n";
    cin>>n;
    cout<<"insira o elemento: \n";
    for(i = 0; i< n; i++){
        cin>>t;
        inserir(t);
    }
    cout<<"caminhamento da arvore b+: \n";
    caminhar_arvore(raiz);
    return 0;
}
 */

#include <iostream>
using namespace std;

struct node {
    int *data;
    node **ponteiro_filho;
    bool folha;
    int n;
} *raiz = NULL, *np = NULL, *no = NULL;

node* init() {
    int i;
    np = new node;
    np->data = new int[5];
    np->ponteiro_filho = new node *[6];
    np->folha = true;
    np->n = 0;

    for (i = 0; i < 6; i++) {
        np->ponteiro_filho[i] = NULL;
    }
    return np;
}

void caminhar_arvore(node *p) {
    cout << endl;
    int i;
    for (i = 0; i < p->n; i++) {
        if (p->folha == false) {
            caminhar_arvore(p->ponteiro_filho[i]);
        }
        cout << " " << p->data[i];
    }

    if (p->folha == false) {
        caminhar_arvore(p->ponteiro_filho[i]);
    }
    cout << endl;
}

void ordenar(int *p, int n) {
    int i, j, temp;
    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++) {
            if (p[i] > p[j]) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

int dividir_filhos(node *x, int indice_no) {
    int j, meio;
    node *np1, *np3, *y;
    np3 = init();
    np3->folha = true;
    if (indice_no == -1) { 
        meio = x->data[2];
        x->data[2] = 0;
        x->n--;
        np1 = init();
        np1->folha = false;
        x->folha = true;
        for (j = 3; j < 5; j++) {
            np3->data[j - 3] = x->data[j];
            np3->ponteiro_filho[j - 3] = x->ponteiro_filho[j];
            np3->n++;
            x->data[j] = 0;
            x->n--;
        }
        for (j = 0; j < 6; j++) {
            x->ponteiro_filho[j] = NULL;
        }
        np1->data[0] = meio;
        np1->ponteiro_filho[np1->n] = x;
        np1->ponteiro_filho[np1->n + 1] = np3;
        np1->n++;
        raiz = np1;
    } else {
        y = x->ponteiro_filho[indice_no];
        meio = y->data[2];
        y->data[2] = 0;
        y->n--;
        for (j = 3; j < 5; j++) {
            np3->data[j - 3] = y->data[j];
            np3->n++;
            y->data[j] = 0;
            y->n--;
        }
        x->ponteiro_filho[indice_no + 1] = y;
        x->ponteiro_filho[indice_no + 1] = np3;
    }
    return meio;
}

void inserir(int a) {
    int i, temp;
    no = raiz;
    if (no == NULL) {
        raiz = init();
        no = raiz;
    } else {
        if (no->folha == true && no->n == 5) {
            temp = dividir_filhos(no, -1);
            no = raiz;
            for (i = 0; i < (no->n); i++) {
                if ((a > no->data[i]) && (a < no->data[i + 1])) {
                    i++;
                    break;
                } else if (a < no->data[0]) {
                    break;
                } else {
                    continue;
                }
            }
            no = no->ponteiro_filho[i];
        } else {
            while (no->folha == false) {
                for (i = 0; i < (no->n); i++) {
                    if ((a > no->data[i]) && (a < no->data[i + 1])) {
                        i++;
                        break;
                    } else if (a < (no->data[0])) {
                        break;
                    } else {
                        continue;
                    }
                }
                if (no->ponteiro_filho[i]->n == 5) {
                    temp = dividir_filhos(no, i);
                    no->data[no->n] = temp;
                    no->n++;
                    continue;
                } else {
                    no = no->ponteiro_filho[i];
                }
            }
        }
    }
    no->data[no->n] = a;
    ordenar(no->data, no->n);
    no->n++;
}

int main() {
    int i, n, t;
    cout << "Insira o número de elementos para serem inseridos: \n";
    cin >> n;
    cout << "Insira os elementos: \n";
    for (i = 0; i < n; i++) {
        cin >> t;
        inserir(t);
    }
    cout << "Caminhamento da árvore B+: \n";
    caminhar_arvore(raiz);
    return 0;
}
