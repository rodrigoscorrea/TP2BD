#ifndef BPlusTree_H
#define BPlusTree_H

#include <iostream>

template <typename T>
struct No {
    bool folha;
    std::size_t grau; 
    std::size_t ocupacao;  
    T* registros;
    No<T>** filhos;
    No<T>* ancestral;

public:
    No(std::size_t _grau) {
        this->folha = false;
        this->grau = _grau;
        this->ocupacao = 0;

        T* registro_aux = new T[grau-1];
        for(int i=0; i<grau-1; i++){
            registro_aux[i] = 0;
        }
        this->registros = registro_aux;

        No<T>** filhos_aux = new No<T>*[grau];
        for(int i=0; i<grau; i++){
            filhos_aux[i] = nullptr;
        }
        this->filhos = filhos_aux;

        this->ancestral = nullptr;

    }
};

//Registro que será usado na árvore
struct RegistroBPT{
    int chave, valor;

    RegistroBPT(int chave, int valor){ //Construtor para operações
        this->chave = chave;
        this->valor = valor;
    }

    RegistroBPT(){ //Construtor para inicializações
        this->chave = 0;
        this->valor = 0;
    }
};

template <typename T>
class BPlusTree {
    No<T>* raiz;
    std::size_t grau;

public:
    BPlusTree(std::size_t _grau) {
        this->raiz = nullptr;
        this->grau = _grau;
    }
    ~BPlusTree() {
        deletar(this->raiz);
    }

    No<T>* get_raiz(){
        return this->raiz;
    }

    No<T>* busca_BPlusTree(No<T>* node, T chave){ //Raiz deve ser passada como parâmetro para node
        if(node == nullptr) { // Caso raiz nula
            return nullptr;
        }
        else{
            No<T>* cursor = node; 

            while(!cursor->folha){ 
                for(int i=0; i<cursor->ocupacao; i++){ 
                    if(chave < cursor->registros[i]){ 
                        cursor = cursor->filhos[i];
                        break;
                    }
                    if(i == (cursor->ocupacao)-1){
                        cursor = cursor->filhos[i+1];
                        break;
                    }
                }
            }

            //busca se a chave existe no nó folha
            for(int i=0; i<cursor->ocupacao; i++){
                if(cursor->registros[i] == chave){
                    return cursor;
                }
            }
            std::cout<<"No nao encontrado\n";
            return nullptr;
        }
    }
    No<T>* BPlusTreeRangeSearch(No<T>* node, T chuva){ //Também usa raiz como parâmetro para node
        if(node == nullptr) { // para raiz nula
            return nullptr;
        }
        else{
            No<T>* cursor = node; 

            while(!cursor->folha){ 
                for(int i=0; i<cursor->ocupacao; i++){ 
                    if(chuva < cursor->registros[i]){ 
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
    /*
    int busca_profundidade(T inicio, T fim, T* result_data, int arr_length) {
        int index=0;

        No<T>* start_node = BPlusTreeRangeSearch(this->raiz,inicio);
        No<T>* cursor = start_node;
        T temp= cursor->registros[0];

        while(temp<=fim){
            if(cursor == nullptr){
                break;
            }
            for(int i=0; i< cursor->ocupacao;i++){
                temp = cursor->registros[i];
                if((temp >= inicio)&&(temp <= fim)){
                    result_data[index] = temp;
                    index++;
                }
            }
            cursor = cursor->filhos[cursor->ocupacao];
        }
        return index;
    } */
    bool busca(T data) {  //checa se registro existe ou não
        return busca_BPlusTree(this->raiz, data) != nullptr;
    }

    int busca_index(T* arr, T data, int len){
        int index = 0;
        for(int i=0; i<len; i++){
            if(data < arr[i]){
                index = i;
                break;
            }
            if(i==len-1){
                index = len;
                break;
            }
        }
        return index;
    }
    T* inserir_registro(T* vet, T data, int tamanho){
        int index = 0;
        for(int i=0; i<tamanho; i++){
            if(data < vet[i]){
                index = i;
                break;
            }
            if(i==tamanho-1){
                index = tamanho;
                break;
            }
        }

        for(int i = tamanho; i > index; i--){
            vet[i] = vet[i-1];
        }

        vet[index] = data;

        return vet;
    }
    No<T>** inserir_filho(No<T>** filhos, No<T>*filho,int tamanho,int index){
        for(int i= tamanho; i > index; i--){
            filhos[i] = filhos[i - 1];
        }
        filhos[index] = filho;
        return filhos;
    }
    No<T>* inserir_registro_filho(No<T>* no_insercao, T data, No<T>* filho){
        int registro_index=0;
        int filho_index=0;
        for(int i=0; i< no_insercao->ocupacao; i++){
            if(data < no_insercao->registros[i]){
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

        no_insercao->registros[registro_index] = data;
        no_insercao->filhos[filho_index] = filho;

        return no_insercao;
    }
    void inserir_ancestral(No<T>* pai,No<T>* filho, T data){
        
        No<T>* cursor = pai;
        if(cursor->ocupacao < this->grau-1){//checagem de overflow, caso negativo
            cursor = inserir_registro_filho(cursor,data,filho);
            cursor->ocupacao++;
        }
        else{//overflow positivo
            
            auto* novo_no = new No<T>(this->grau);
            novo_no->ancestral = cursor->ancestral;

            T* registros_aux = new T[cursor->ocupacao+1];
            for(int i=0; i<cursor->ocupacao; i++){
                registros_aux[i] = cursor->registros[i];
            }
            registros_aux = inserir_registro(registros_aux,data,cursor->ocupacao);

            auto** filhos_aux = new No<T>*[cursor->ocupacao+2];
            for(int i=0; i<cursor->ocupacao+1;i++){
                filhos_aux[i] = cursor->filhos[i];
            }
            filhos_aux[cursor->ocupacao+1] = nullptr;
            filhos_aux = inserir_filho(filhos_aux,filho,cursor->ocupacao+1,busca_index(registros_aux,data,cursor->ocupacao+1));

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

            T registros_ancestral = registros_aux[this->grau/2];

            delete[] registros_aux;
            delete[] filhos_aux;

            //checagem de no ancestral (balanceamento)
            if(cursor->ancestral == nullptr){//caso seja raiz, não há n
                auto* novo_ancestral = new No<T>(this->grau);
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
    void inserir_arvore(T data) {
        if(this->raiz == nullptr){ //caso de raiz
            this->raiz = new No<T>(this->grau);
            this->raiz->folha = true;
            this->raiz->registros[0] = data;
            this->raiz->ocupacao = 1; //
        }
        else{ 
            No<T>* cursor = this->raiz;

            //ir até as folhas
            cursor = BPlusTreeRangeSearch(cursor, data);

            
            if(cursor->ocupacao < (this->grau-1)){ //caso não haja overflow
                
                cursor->registros = inserir_registro(cursor->registros,data,cursor->ocupacao);
                cursor->ocupacao++;
               
                cursor->filhos[cursor->ocupacao] = cursor->filhos[cursor->ocupacao-1];
                cursor->filhos[cursor->ocupacao-1] = nullptr;
            }
            else{//overflow detectado
                
                auto* novo_no = new No<T>(this->grau);
                novo_no->folha = true;
                novo_no->ancestral = cursor->ancestral;

                T* registros_aux = new T[cursor->ocupacao+1];
                for(int i=0; i<cursor->ocupacao; i++){
                    registros_aux[i] = cursor->registros[i];
                }
                registros_aux = inserir_registro(registros_aux,data,cursor->ocupacao);

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
                T registros_ancestral = novo_no->registros[0];

                if(cursor->ancestral == nullptr){ //caso não haja nó ancestral -> cursor = raiz
                    auto* novo_ancestral = new No<T>(this->grau);
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

    void remove(T data) { // remove um registro da árvore
        
        No<T>* cursor = this->raiz;

        //Chegar até as folhas
        cursor = BPlusTreeRangeSearch(cursor,data);

        //make sibling index
        int irmao_index =-1;
        for(int i=0; i<cursor->ancestral->ocupacao+1;i++){
            if(cursor == cursor->ancestral->filhos[i]){
                irmao_index = i;
            }
        }
        int indice_esquerda=irmao_index-1;
        int indice_direita=irmao_index+1;


        //find data
        /* int exclusao_index=-1;
        for(int i=0; i< cursor->ocupacao; i++){
            if(cursor->registros[i] == data){
                exclusao_index = i;
                break;
            }
        } */
        //if data dosen't exist, nothing happen
        if(irmao_index==-1){
            std::cout<<"Dado nao encontrado para remocao\n";
            return; // there is no match remove value
        }

        //remove data
        for(int i=irmao_index; i<cursor->ocupacao-1;i++){
            cursor->registros[i] = cursor->registros[i+1];
        }
        cursor->registros[cursor->ocupacao-1] = 0;
        cursor->ocupacao--;

        //if cursor is raiz, and there are no more data -> clean!
        if(cursor == this->raiz && cursor->ocupacao==0){//raiz case
            deletar(this->raiz);
            this->raiz = nullptr;
            return;
        }
        cursor->filhos[cursor->ocupacao] = cursor->filhos[cursor->ocupacao+1];
        cursor->filhos[cursor->ocupacao+1] = nullptr;


        //underflow check
        if(cursor == this->raiz){
            return;
        }
        if(cursor->ocupacao < grau/2){//underflow case

            if(indice_esquerda >= 0){// left_sibiling exists
                No<T>* irmao_esquerda= cursor->ancestral->filhos[indice_esquerda];

                if(irmao_esquerda->ocupacao > grau/2){ //if data number is enough to use this node
                    T* temp = new T[cursor->ocupacao+1];

                    //copy registros
                    for(int i=0; i<cursor->ocupacao; i++){
                        temp[i]=cursor->registros[i];
                    }

                    //insert and rearrange
                    inserir_registro(temp,irmao_esquerda->registros[irmao_esquerda->ocupacao -1],cursor->ocupacao);
                    for(int i=0; i<cursor->ocupacao+1; i++){
                        cursor->registros[i] = temp[i];
                    }
                    cursor->ocupacao++;
                    delete[] temp;

                    //pointer edit
                    cursor->filhos[cursor->ocupacao] = cursor->filhos[cursor->ocupacao-1];
                    cursor->filhos[cursor->ocupacao-1] = nullptr;

                    //sibling property edit
                    irmao_esquerda->registros[irmao_esquerda->ocupacao-1] = 0;
                    irmao_esquerda->ocupacao--;
                    irmao_esquerda->filhos[irmao_esquerda->ocupacao] = irmao_esquerda->filhos[irmao_esquerda->ocupacao+1]; //cursor
                    irmao_esquerda->filhos[irmao_esquerda->ocupacao+1]= nullptr;

                    //parent property edit
                    cursor->ancestral->registros[indice_esquerda] = cursor->registros[0];

                    return;
                }
            }
            if(indice_direita <= cursor->ancestral->ocupacao){// right_sibiling exists
                No<T>* irmao_direito = cursor->ancestral->filhos[indice_direita];

                if(irmao_direito->ocupacao >grau/2){//if data number is enough to use this node
                    T* temp = new T[cursor->ocupacao+1];

                    //copy registros
                    for(int i=0; i<cursor->ocupacao; i++){
                        temp[i]=cursor->registros[i];
                    }
                    //insert and rearrange
                    inserir_registro(temp,irmao_direito->registros[0],cursor->ocupacao);
                    for(int i=0; i<cursor->ocupacao+1; i++){
                        cursor->registros[i] = temp[i];
                    }
                    cursor->ocupacao++;
                    delete[] temp;

                    //pointer edit
                    cursor->filhos[cursor->ocupacao] = cursor->filhos[cursor->ocupacao-1];
                    cursor->filhos[cursor->ocupacao-1] = nullptr;

                    //sibling property edit
                    for(int i=0; i<irmao_direito->ocupacao-1;i++){
                        irmao_direito->registros[i] = irmao_direito->registros[i+1];
                    }
                    irmao_direito->registros[irmao_direito->ocupacao-1] = 0;
                    irmao_direito->ocupacao--;
                    irmao_direito->filhos[irmao_direito->ocupacao] = irmao_direito->filhos[irmao_direito->ocupacao+1]; //cursor
                    irmao_direito->filhos[irmao_direito->ocupacao+1]= nullptr;

                    //parent property edit
                    cursor->ancestral->registros[indice_direita-1] = irmao_direito->registros[0];

                    return;
                }
            }

            //if sibling is not enought to use their data
            //we have to merge step

            if(indice_esquerda>=0){ // left_sibling exists
                No<T>* irmao_esquerda = cursor->ancestral->filhos[indice_esquerda];

                //merge two leaf node
                for(int i=0; i<cursor->ocupacao; i++){
                    irmao_esquerda->registros[irmao_esquerda->ocupacao+i]=cursor->registros[i];
                }
                //edit pointer
                irmao_esquerda->filhos[irmao_esquerda->ocupacao] = nullptr;
                irmao_esquerda->ocupacao = irmao_esquerda->ocupacao+cursor->ocupacao;
                irmao_esquerda->filhos[irmao_esquerda->ocupacao] = cursor->filhos[cursor->ocupacao];

                //parent property edit
                Removepar(cursor, indice_esquerda, cursor->ancestral);
                for(int i=0; i<cursor->ocupacao;i++){
                    cursor->registros[i]=0;
                    cursor->filhos[i] = nullptr;
                }
                cursor->filhos[cursor->ocupacao] = nullptr;

                delete[] cursor->registros;
                delete[] cursor->filhos;
                delete cursor;

                return;

            }
            if(indice_direita<=cursor->ancestral->ocupacao){ // right_sibiling exists
                No<T>* irmao_direito = cursor->ancestral->filhos[indice_direita];

                //merge two leaf node
                for(int i=0; i<irmao_direito->ocupacao; i++){
                    cursor->registros[i+cursor->ocupacao]=irmao_direito->registros[i];
                }
                //edit pointer
                cursor->filhos[cursor->ocupacao] = nullptr;
                cursor->ocupacao = irmao_direito->ocupacao+cursor->ocupacao;
                cursor->filhos[cursor->ocupacao] = irmao_direito->filhos[irmao_direito->ocupacao];

                //parent property edit
                Removepar(irmao_direito, indice_direita-1, cursor->ancestral);

                for(int i=0; i<irmao_direito->ocupacao;i++){
                    irmao_direito->registros[i]=0;
                    irmao_direito->filhos[i] = nullptr;
                }
                irmao_direito->filhos[irmao_direito->ocupacao] = nullptr;

                delete[] irmao_direito->registros;
                delete[] irmao_direito->filhos;
                delete irmao_direito;
                return;

            }

        }
        else{
            return;
        }
    }

    void Removepar(No<T>* node, int index, No<T>* ancestral){
        No<T>* remover = node;
        No<T>* cursor = ancestral;
        T target = cursor->registros[index];

        //if cursor is raiz, and there are no more data -> child node is to be raiz!
        if(cursor == this->raiz && cursor->ocupacao==1){//raiz case
            if(remover == cursor->filhos[0]){
                delete[] remover->registros;
                delete[] remover->filhos;
                delete remover;
                this->raiz = cursor->filhos[1];
                delete[] cursor->registros;
                delete[] cursor->filhos;
                delete cursor;
                return;
            }
            if(remover == cursor->filhos[1]){
                delete[] remover->registros;
                delete[] remover->filhos;
                delete remover;
                this->raiz = cursor->filhos[0];
                delete[] cursor->registros;
                delete[] cursor->filhos;
                delete cursor;
                return;
            }
        }

        //remove data
        for(int i=index; i<cursor->ocupacao-1;i++){
            cursor->registros[i] = cursor->registros[i+1];
        }
        cursor->registros[cursor->ocupacao-1] = 0;

        //remove pointer
        int index_remocao = -1;
        for(int i=0; i<cursor->ocupacao+1;i++){
            if(cursor->filhos[i] == remover){
                index_remocao = i;
            }
        }
        if(index_remocao == -1){
            return;
        }
        for(int i=index_remocao; i<cursor->ocupacao;i++){
            cursor->filhos[i] = cursor->filhos[i+1];
        }
        cursor->filhos[cursor->ocupacao] = nullptr;
        cursor->ocupacao--;

        //underflow check
        if(cursor == this->raiz){
            return;
        }
        if(cursor->ocupacao < grau/2){//underflow case

            int sib_index =-1;
            for(int i=0; i<cursor->ancestral->ocupacao+1;i++){
                if(cursor == cursor->ancestral->filhos[i]){
                    sib_index = i;
                }
            }
            int indice_esquerda=sib_index-1;
            int indice_direita=sib_index+1;

            if(indice_esquerda >= 0){// left_sibiling exists
                No<T>* irmao_esquerda= cursor->ancestral->filhos[indice_esquerda];

                if(irmao_esquerda->ocupacao > grau/2){ //if data number is enough to use this node
                    T* temp = new T[cursor->ocupacao+1];

                    //copy registros
                    for(int i=0; i<cursor->ocupacao; i++){
                        temp[i]=cursor->registros[i];
                    }

                    //insert and rearrange at cursor
                    inserir_registro(temp, cursor->ancestral->registros[indice_esquerda],cursor->ocupacao);
                    for(int i=0; i<cursor->ocupacao+1; i++){
                        cursor->registros[i] = temp[i];
                    }
                    cursor->ancestral->registros[indice_esquerda] = irmao_esquerda->registros[irmao_esquerda->ocupacao-1];
                    delete[] temp;

                    No<T>** child_temp = new No<T>*[cursor->ocupacao+2];
                    //copy child node
                    for(int i=0; i<cursor->ocupacao+1; i++){
                        child_temp[i]=cursor->filhos[i];
                    }
                    //insert and rearrange at child
                    inserir_filho(child_temp,irmao_esquerda->filhos[irmao_esquerda->ocupacao],cursor->ocupacao,0);

                    for(int i=0; i<cursor->ocupacao+2; i++){
                        cursor->filhos[i] = child_temp[i];
                    }
                    delete[] child_temp;
 
                    cursor->ocupacao++;
                    irmao_esquerda->ocupacao--;
                    return;

                }
            }

            if(indice_direita <= cursor->ancestral->ocupacao){// right_sibiling exists
                No<T>* irmao_direita = cursor->ancestral->filhos[indice_direita];

                if(irmao_direita->ocupacao > grau/2){//if data number is enough to use this node
                    T* temp = new T[cursor->ocupacao+1];

                    //copy registros
                    for(int i=0; i<cursor->ocupacao; i++){
                        temp[i]=cursor->registros[i];
                    }
                    //insert and rearrange at cursor
                    inserir_registro(temp,cursor->ancestral->registros[sib_index],cursor->ocupacao);
                    for(int i=0; i<cursor->ocupacao+1; i++){
                        cursor->registros[i] = temp[i];
                    }
                    cursor->ancestral->registros[sib_index] = irmao_direita->registros[0];
                    delete[] temp;

                    //insert and reaarange at child

                    cursor->filhos[cursor->ocupacao+1] = irmao_direita->filhos[0];
                    for(int i=0; i<irmao_direita->ocupacao; i++){
                        irmao_direita->filhos[i] = irmao_direita->filhos[i+1];
                    }
                    irmao_direita->filhos[irmao_direita->ocupacao] = nullptr;

                    cursor->ocupacao++;
                    irmao_direita->ocupacao--;
                    return;

                }
            }

            //if sibling is not enought to use their data
            //we have to merge step
            if(indice_esquerda>=0){ // left_sibling exists
                No<T>* irmao_esquerda = cursor->ancestral->filhos[indice_esquerda];

                irmao_esquerda->registros[irmao_esquerda->ocupacao] = cursor->ancestral->registros[indice_esquerda];
                //merge two leaf node
                for(int i=0; i<cursor->ocupacao; i++){
                    irmao_esquerda->registros[irmao_esquerda->ocupacao+i+1]=cursor->registros[i];
                }
                for(int i=0; i<cursor->ocupacao+1;i++){
                    irmao_esquerda->filhos[irmao_esquerda->ocupacao+i+1] = cursor->filhos[i];
                    cursor->filhos[i]->ancestral = irmao_esquerda;
                }
                for(int i=0; i<cursor->ocupacao+1; i++){
                    cursor->filhos[i] = nullptr;
                }
                irmao_esquerda->ocupacao = irmao_esquerda->ocupacao+cursor->ocupacao+1;
                //delete recursion
                Removepar(cursor, indice_esquerda,cursor->ancestral);
                return;

            }
            if(indice_direita<=cursor->ancestral->ocupacao){ // right_sibiling exists
                No<T>* irmao_direita = cursor->ancestral->filhos[indice_direita];

                cursor->registros[cursor->ocupacao] = cursor->ancestral->registros[indice_direita-1];
                //merge two leaf node
                for(int i=0; i<irmao_direita->ocupacao; i++){
                    cursor->registros[cursor->ocupacao+1+i]=irmao_direita->registros[i];
                }
                for(int i=0; i<irmao_direita->ocupacao+1;i++){
                    cursor->filhos[cursor->ocupacao+i+1] = irmao_direita->filhos[i];
                    irmao_direita->filhos[i]->ancestral=irmao_direita;
                }
                for(int i=0; i<irmao_direita->ocupacao+1; i++){
                    irmao_direita->filhos[i] = nullptr;
                }
                //edit pointer
                irmao_direita->ocupacao = irmao_direita->ocupacao+cursor->ocupacao+1;
                //parent property edit
                Removepar(irmao_direita, indice_direita-1,cursor->ancestral);
                return;
            }
        }
        else{
            return;
        }
    }

    void deletar(No<T>* cursor){
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

    /* void imprime_registro(RegistroBPT* registro){
        cout<<registro->chave;
        cout<<registro->valor;
        cout<<"\n";
    } */

    void imprimir_no(No<T>* cursor) {
        if (cursor != NULL) {
            for (int i = 0; i < cursor->ocupacao; ++i) {
                std::cout << cursor->registros[i] << " ";
            }
            std::cout <<"\n";

            if (!cursor->folha) {
                for (int i = 0; i < cursor->ocupacao + 1; ++i) {
                    imprimir_no(cursor->filhos[i]);
                }
            }
        } 
    }
};

#endif

int main(){
    const int max = 3;

    //Teste com registro

    /* BPlusTree<RegistroBPT*>arvoreTeste(max);
    RegistroBPT* reg1 = new RegistroBPT(5,4);
    RegistroBPT* reg2 = new RegistroBPT();
    
    arvoreTeste.insert(reg1);
    arvoreTeste.insert(reg2); */

    //Teste com inteiros
    BPlusTree<int>arvoreTeste(max);
    /* arvoreTeste.inserir_arvore(5);
    arvoreTeste.inserir_arvore(10);
    arvoreTeste.inserir_arvore(15);
    arvoreTeste.inserir_arvore(25);
    arvoreTeste.inserir_arvore(35);
    arvoreTeste.inserir_arvore(45);

    //Teste de busca - inteiros
    arvoreTeste.imprimir_no(arvoreTeste.busca_BPlusTree(arvoreTeste.get_raiz(), 45));

    arvoreTeste.remove(25);
    arvoreTeste.imprimir_no(arvoreTeste.busca_BPlusTree(arvoreTeste.get_raiz(), 25));
    arvoreTeste.remove(39);
    //Teste de arvore inexistente
    arvoreTeste.deletar(arvoreTeste.get_raiz());
    arvoreTeste.imprimir_arvore(); */

    arvoreTeste.inserir_arvore(10);
    arvoreTeste.inserir_arvore(15);
    arvoreTeste.remove(39);
}