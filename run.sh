#!/bin/bash

echo "1. upload <file>: Programa que fará a carga inicial dos dados de entrada que irá criar um banco de dados composto pelos seguintes arquivos:"
echo "Arquivo de dados organizado por hashing"
echo "Arquivo de índice primário usando uma B+Tree armazenada em memória secundária"
echo "Arquivo de índice secundário usando uma outra B+Tree em memória secundária"
echo "Digite o nome do arquivo de entrada:"
./upload

echo "2. findrec <ID>: Programa que busca diretamente no arquivo de dados por um registro com o ID informado e, se existir, retorna os campos do registro, a quantidade de blocos lidos para encontrá-lo e a quantidade total de blocos do arquivo de dados;"
echo "Digite o id de busca:"
./findrec

echo "3. seek1 <ID>: Programa que devolve o registro com ID igual ao informado, se existir, pesquisando através do arquivo de índice primário, mostrando todos os campos, a quantidade de blocos lidos para encontrá-lo no arquivo de índice e a quantidade total de blocos do arquivo de índice primário;"
echo "Digite o id de busca:"
./seek1

echo "4. seek2 <Titulo>: Programa que mostra os dados do registro que possua o Título igual ao informado, se existir, pesquisando através do arquivo de índice secundário, informando a quantidade de blocos lidos para encontrá-lo no arquivo de índice e a quantidade total de blocos do arquivo de índice secundário"
echo "Digite o título da busca:"
./seek2