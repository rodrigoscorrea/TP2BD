#include <iostream>
#include <fstream>
#include <sstream>
#include "estruturas/estruturaBlocoRegistro.hpp"
#include "estruturas/definicoes.hpp"
#include "hash/hashTable.cpp"

int main(int argc, char const *argv[])
{   
    const int id = stoi(argv[1]);
    hashTable hash_table = hashTable(ARQUIVO_HASHTABLE, false);

    imprimir_registro(hash_table.busca_registro_hashtable(id));

    return 0;
}