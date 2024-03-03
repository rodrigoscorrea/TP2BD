#include <iostream>
#include <fstream>
#include <sstream>
#include "bptree/bptreeSerializada.cpp"
#include "estruturas/estruturaBlocoRegistro.hpp"
#include "estruturas/definicoes.hpp"
#include "hash/hashTable.cpp"

int main(int argc, char const *argv[])
{   
    const int id = stoi(argv[1]);
    hashTable hash_table = hashTable(ARQUIVO_HASHTABLE, false);
    BPlusTree arvore_primaria(ALTURA_ARVORE_PRIMARIA); 
    arvore_primaria = arvore_primaria.desserializar_arvore(ARQUIVO_ARVORE_PRIMARIA);

    imprimir_registro(arvore_primaria.buscar_registro_bpt(hash_table.arquivoIn, id));

    return 0;
}