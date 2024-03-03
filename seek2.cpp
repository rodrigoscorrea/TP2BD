#include <iostream>
#include <fstream>
#include <sstream>
#include "bptree/bptreeStringSerializada.cpp"
#include "estruturas/definicoes.hpp"
#include "estruturas/parser.cpp"

int main(int argc, char const *argv[])
{   
    const string title = normalizar_string(remover_aspas_input(argv[1]));
    hashTable hash_table = hashTable(ARQUIVO_HASHTABLE, false);
    BPlusTreeString arvore_secundaria(ALTURA_ARVORE); 
    arvore_secundaria = arvore_secundaria.desserializar_arvore_s(ARQUIVO_ARVORE_SECUNDARIA);

    imprimir_registro(arvore_secundaria.buscar_registro_s(hash_table.arquivoIn, title));

    return 0;
}