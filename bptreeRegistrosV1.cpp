#ifndef BPlusTree_H
#define BPlusTree_H

#include <iostream>
//Registro que será usado na árvore
struct RegistroBPT{ //VERIFICAR DEPOIS SE DÁ PRA FAZER SETANDO AO INVÉS DOS DOIS PARA DIFERENCIAR MAIS AINDA
    int chave, valor;

    /* RegistroBPT(int chave, int valor){ //Construtor para operações
        this->chave = chave;
        this->valor = valor;
    }

    RegistroBPT(){ //Construtor para inicializações
        this->chave = 0;
        this->valor = 0;
    } */
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
            //filhos_aux[i] = nullptr;
            this->filhos[i] = nullptr;
        }
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

    No<RegistroBPT>* busca_BPlusTree(No<RegistroBPT>* node, RegistroBPT chave_busca){ //Raiz deve ser passada como parâmetro para node
        if(node == nullptr) { // Caso raiz nula
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
                    if(i == (cursor->ocupacao -1)){
                        cursor = cursor->filhos[i+1];
                        break;
                    }
                }
            }

            //busca se a chave existe no nó folha
            for(int i=0; i<cursor->ocupacao; i++){
                if(cursor->registros[i].chave == chave_busca.chave){
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

    void remove(RegistroBPT alvo_remocao) { // remove um registro da árvore
        
        No<RegistroBPT>* cursor = this->raiz;

        //Chegar até as folhas
        cursor = busca_BPlusTree(cursor,alvo_remocao);
        if (cursor == nullptr){
            std::cout<<"Dado nao encontrado na arvore\n";
            return;
        }

        //Se há somente a raiz e ela não tem filhos ainda
        int filhos_index = 0;
        for(int i = 0; i < this->grau; i++){
            if(cursor->filhos[i] == nullptr){
                filhos_index++;
            }
        }
        if(filhos_index == this->grau){
            for(int i = 0; i < filhos_index; i++){
                if(cursor->registros[i].chave == alvo_remocao.chave){
                    cursor->registros[i] = RegistroBPT(0,0);
                    cursor->ocupacao--;
                }
            }
            std::cout<<alvo_remocao.chave<<"removido\n";
            return;
        }

        //demais remoções
        int irmao_index =-1;
        for(int i=0; i<cursor->ancestral->ocupacao+1;i++){
            if(cursor == cursor->ancestral->filhos[i]){
                irmao_index = i;
            }
        }
        int indice_esquerda=irmao_index-1;
        int indice_direita=irmao_index+1;


        //find alvo_remocao
        /* int exclusao_index=-1;
        for(int i=0; i< cursor->ocupacao; i++){
            if(cursor->registros[i] == alvo_remocao){
                exclusao_index = i;
                break;
            }
        } */
        
        if(irmao_index==-1){
            std::cout<<"Dado nao encontrado para remocao\n";
            return; //
        }

        //remover dado
        for(int i=irmao_index; i<cursor->ocupacao-1;i++){
            cursor->registros[i] = cursor->registros[i+1];
        }
        cursor->registros[cursor->ocupacao-1] = RegistroBPT(0,0);
        cursor->ocupacao--;

        //caso cursor seja raiz e esteja vazia
        if(cursor == this->raiz && cursor->ocupacao==0){
            deletar(this->raiz);
            this->raiz = nullptr;
            return;
        }
        cursor->filhos[cursor->ocupacao] = cursor->filhos[cursor->ocupacao+1];
        cursor->filhos[cursor->ocupacao+1] = nullptr;

        //checagem de underflow
        if(cursor == this->raiz){
            return;
        }
        if(cursor->ocupacao < grau/2){//underflow 

            if(indice_esquerda >= 0){// se existir um irmão à esquerda
                No<RegistroBPT>* irmao_esquerda= cursor->ancestral->filhos[indice_esquerda];

                if(irmao_esquerda->ocupacao > grau/2){ //se ocupação do irmão autorizar ceder dado para balanceamento
                    RegistroBPT* temp = new RegistroBPT[cursor->ocupacao+1];

                    for(int i=0; i<cursor->ocupacao; i++){
                        temp[i]=cursor->registros[i];
                    }

                    inserir_registro(temp,irmao_esquerda->registros[irmao_esquerda->ocupacao -1],cursor->ocupacao);
                    for(int i=0; i<cursor->ocupacao+1; i++){
                        cursor->registros[i] = temp[i];
                    }
                    cursor->ocupacao++;
                    delete[] temp;

                    cursor->filhos[cursor->ocupacao] = cursor->filhos[cursor->ocupacao-1];
                    cursor->filhos[cursor->ocupacao-1] = nullptr;

                    irmao_esquerda->registros[irmao_esquerda->ocupacao-1] = RegistroBPT(0,0);
                    irmao_esquerda->ocupacao--;
                    irmao_esquerda->filhos[irmao_esquerda->ocupacao] = irmao_esquerda->filhos[irmao_esquerda->ocupacao+1]; 
                    irmao_esquerda->filhos[irmao_esquerda->ocupacao+1]= nullptr;

                    cursor->ancestral->registros[indice_esquerda] = cursor->registros[0];

                    return;
                }
            }
            if(indice_direita <= cursor->ancestral->ocupacao){// Se existir um irmão à direita
                No<RegistroBPT>* irmao_direito = cursor->ancestral->filhos[indice_direita];

                if(irmao_direito->ocupacao >grau/2){
                    RegistroBPT* temp = new RegistroBPT[cursor->ocupacao+1];

                    for(int i=0; i<cursor->ocupacao; i++){
                        temp[i]=cursor->registros[i];
                    }
                    inserir_registro(temp,irmao_direito->registros[0],cursor->ocupacao);
                    for(int i=0; i<cursor->ocupacao+1; i++){
                        cursor->registros[i] = temp[i];
                    }
                    cursor->ocupacao++;
                    delete[] temp;

                    cursor->filhos[cursor->ocupacao] = cursor->filhos[cursor->ocupacao-1];
                    cursor->filhos[cursor->ocupacao-1] = nullptr;

                    for(int i=0; i<irmao_direito->ocupacao-1;i++){
                        irmao_direito->registros[i] = irmao_direito->registros[i+1];
                    }
                    irmao_direito->registros[irmao_direito->ocupacao-1] = RegistroBPT(0,0);
                    irmao_direito->ocupacao--;
                    irmao_direito->filhos[irmao_direito->ocupacao] = irmao_direito->filhos[irmao_direito->ocupacao+1]; //cursor
                    irmao_direito->filhos[irmao_direito->ocupacao+1]= nullptr;

                    cursor->ancestral->registros[indice_direita-1] = irmao_direito->registros[0];
                    return;
                }
            }

            //se mesmo após receber dos irmãos ainda não satisfazer a condição de ocupação, realizar merge dos nós

            if(indice_esquerda>=0){
                No<RegistroBPT>* irmao_esquerda = cursor->ancestral->filhos[indice_esquerda];

                //merge two leaf node
                for(int i=0; i<cursor->ocupacao; i++){
                    irmao_esquerda->registros[irmao_esquerda->ocupacao+i]=cursor->registros[i];
                }
                irmao_esquerda->filhos[irmao_esquerda->ocupacao] = nullptr;
                irmao_esquerda->ocupacao = irmao_esquerda->ocupacao+cursor->ocupacao;
                irmao_esquerda->filhos[irmao_esquerda->ocupacao] = cursor->filhos[cursor->ocupacao];

                remover_ancestral(cursor, indice_esquerda, cursor->ancestral);
                for(int i=0; i<cursor->ocupacao;i++){
                    cursor->registros[i]=RegistroBPT(0,0);
                    cursor->filhos[i] = nullptr;
                }
                cursor->filhos[cursor->ocupacao] = nullptr;

                delete[] cursor->registros;
                delete[] cursor->filhos;
                delete cursor;

                return;

            }
            if(indice_direita<=cursor->ancestral->ocupacao){
                No<RegistroBPT>* irmao_direito = cursor->ancestral->filhos[indice_direita];

                //realizar o merge de dois nós folhas
                for(int i=0; i<irmao_direito->ocupacao; i++){
                    cursor->registros[i+cursor->ocupacao]=irmao_direito->registros[i];
                }  
                cursor->filhos[cursor->ocupacao] = nullptr;
                cursor->ocupacao = irmao_direito->ocupacao+cursor->ocupacao;
                cursor->filhos[cursor->ocupacao] = irmao_direito->filhos[irmao_direito->ocupacao];

                remover_ancestral(irmao_direito, indice_direita-1, cursor->ancestral);

                for(int i=0; i<irmao_direito->ocupacao;i++){
                    irmao_direito->registros[i]=RegistroBPT(0,0);
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

    void remover_ancestral(No<RegistroBPT>* node, int index, No<RegistroBPT>* ancestral){
        No<RegistroBPT>* remover = node;
        No<RegistroBPT>* cursor = ancestral;
        RegistroBPT target = cursor->registros[index];

        //caso cursor seja raiz e não haja mais dados no seu vetor de registros
        if(cursor == this->raiz && cursor->ocupacao==1){
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

        //remove dado
        for(int i=index; i<cursor->ocupacao-1;i++){
            cursor->registros[i] = cursor->registros[i+1];
        }
        cursor->registros[cursor->ocupacao-1] = RegistroBPT(0,0);

        //remove ponteiro
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

        //checagem de underflow
        if(cursor == this->raiz){
            return;
        }
        if(cursor->ocupacao < grau/2){//underflow 

            int sib_index =-1;
            for(int i=0; i<cursor->ancestral->ocupacao+1;i++){
                if(cursor == cursor->ancestral->filhos[i]){
                    sib_index = i;
                }
            }
            int indice_esquerda=sib_index-1;
            int indice_direita=sib_index+1;

            if(indice_esquerda >= 0){
                No<RegistroBPT>* irmao_esquerda= cursor->ancestral->filhos[indice_esquerda];

                if(irmao_esquerda->ocupacao > grau/2){ 
                    RegistroBPT* temp = new RegistroBPT[cursor->ocupacao+1];

                    for(int i=0; i<cursor->ocupacao; i++){
                        temp[i]=cursor->registros[i];
                    }

                    inserir_registro(temp, cursor->ancestral->registros[indice_esquerda],cursor->ocupacao);
                    for(int i=0; i<cursor->ocupacao+1; i++){
                        cursor->registros[i] = temp[i];
                    }
                    cursor->ancestral->registros[indice_esquerda] = irmao_esquerda->registros[irmao_esquerda->ocupacao-1];
                    delete[] temp;

                    No<RegistroBPT>** child_temp = new No<RegistroBPT>*[cursor->ocupacao+2];
                    for(int i=0; i<cursor->ocupacao+1; i++){
                        child_temp[i]=cursor->filhos[i];
                    }
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

            if(indice_direita <= cursor->ancestral->ocupacao){
                No<RegistroBPT>* irmao_direita = cursor->ancestral->filhos[indice_direita];

                if(irmao_direita->ocupacao > grau/2){
                    RegistroBPT* temp = new RegistroBPT[cursor->ocupacao+1];

                    for(int i=0; i<cursor->ocupacao; i++){
                        temp[i]=cursor->registros[i];
                    }
                    inserir_registro(temp,cursor->ancestral->registros[sib_index],cursor->ocupacao);
                    for(int i=0; i<cursor->ocupacao+1; i++){
                        cursor->registros[i] = temp[i];
                    }
                    cursor->ancestral->registros[sib_index] = irmao_direita->registros[0];
                    delete[] temp;

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

            //Casos de merge - critério igual ao da função acima 
            if(indice_esquerda>=0){ 
                No<RegistroBPT>* irmao_esquerda = cursor->ancestral->filhos[indice_esquerda];

                irmao_esquerda->registros[irmao_esquerda->ocupacao] = cursor->ancestral->registros[indice_esquerda];
                //merge nos dois nós folha
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
                
                remover_ancestral(cursor, indice_esquerda,cursor->ancestral);
                return;

            }
            if(indice_direita<=cursor->ancestral->ocupacao){
                No<RegistroBPT>* irmao_direita = cursor->ancestral->filhos[indice_direita];

                cursor->registros[cursor->ocupacao] = cursor->ancestral->registros[indice_direita-1];
                //merge nos dois nós folha
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
                
                irmao_direita->ocupacao = irmao_direita->ocupacao+cursor->ocupacao+1;
                remover_ancestral(irmao_direita, indice_direita-1,cursor->ancestral);
                return;
            }
        }
        else{
            return;
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

int main(){
    const int max = 3;

    //Teste com registro

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

    arvoreTeste.imprimir_arvore();
    //Teste com inteiros
    //BPlusTree<int>arvoreTeste(max);
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

    /* arvoreTeste.inserir_arvore(10);
    arvoreTeste.inserir_arvore(15);
    arvoreTeste.inserir_arvore(5);
    arvoreTeste.remove(39);
    arvoreTeste.remove(15);
    arvoreTeste.imprimir_no(arvoreTeste.busca_BPlusTree(arvoreTeste.get_raiz(), 10));
    arvoreTeste.imprimir_arvore(); */
}