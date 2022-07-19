#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "avl.c"
#include "abp.c"

//Inicializando variáveis globais

extern int abpComparison; //Variável global para contar as comparações na ABP, vinda de abp.c
extern int avlComparison; //Variável global para contar as comparações na AVL, vinda de avl.c
extern int abpInsertion; //Variável global para contar as inserções na ABP, vinda de ABP.c
extern int avlInsertion; //Variável global para contar as inserções na AVL, vinda de avl.c
extern int rotation; //Variável global para contar as rotações na AVL, vinda de avl.c


int main(int argc, char *argv[]) //Exemplo de chamada do programa: nomeDoPrograma.exe <arquivo_lexico.txt> <arquivo_de_entrada.txt> <arquivo_de_saida.txt> <Tipo de árvore (AVL ou ABP)>
{

    char *token; //Variável auxiliar que vai segurar os tokens na hora de ler o arquivo de entrada
    char delim[64] = {" ,.&*%\?!;/-'@\"$#=><()][}{:\n\t"}; //Variável auxiliar para guardar os diferentes separadores de palavras nos textos

    setlocale(LC_ALL, ""); //Deixa legíveis os textos em português

    clock_t insertStart, insertEnd; //Declara as variáveis de contagem de tempo de execução de inserção
    clock_t comparisonStart, comparisonEnd; //E comparação

    float insertTime; //Declara a variável que vai segurar o valor final do tempo de inserção
    float comparisonTime; //E do tempo de comparação

    clock_t start, end; //Inicializa a variável para a contagem do tempo de execução

    const char EXEMPLO_DE_CHAMADA[200] = "lexico.exe <lexico.txt> <arquivo_de_entrada.txt> <arquivo_de_saida.txt> <Tipo de árvore (abp ou avl)>";

    FILE * lexiconFile; //Arquivo contendo o léxico
    FILE * inputFile; //Arquivo de entrada
    FILE * outputFile; //Arquivo de saída

    char lexiconWord[100]; //Variável que vai reter o valor das palavras tirada do arquivo léxico
    float lexiconPolarity; //Variável que vai reter o valor da polaridade tirada do arquivo léxico

    char linha[1024]; //Variável auxiliar que vai ser tokenizada e vai prover as palavras para as consultas nas ABPs
    char linhaPrint[1024]; //Variável que vai receber o valor dos textos tirados do arquivo de entrada, e será printada no arquivo de saída
    char inputWord[100]; //Variável que vai receber o valor de cada palavra lida do arquivo de entrada

    Abp* lexiconAbp = NULL; //Inicializa as ABPs (Não consegui pensar em um jeito mais elegante)
    Avl* lexiconAvl = NULL;

    if (argc!=5) //Caso o programa não receba os argumentos corretamente
    {
        printf("\nNumero incorreto de parametros.\nExemplo de chamada: %s\n", EXEMPLO_DE_CHAMADA);
        return 1; //Para a execução
    }
    else //Caso o programa receba o número correto de argumentos
    {

        printf("\nANÁLISE DE SENTIMENTOS\n");

        lexiconFile = fopen(argv[1], "r"); //Abre o arquivo do léxico
        if(!lexiconFile) //Caso não seja capaz de abrir o arquivo do léxico
        {
            printf("\nErro ao abrir o arquivo %s\n", argv[1]);
            return 1; //Para a execução
        }
        else //Caso o arquivo seja aberto com sucesso
        {
            printf("\nArquivo %s aberto com sucesso.\n", argv[1]);

            insertStart = clock(); //Inicia o tempo de inserção

            if (strcmp(argv[4], "abp")==0) { //Caso o usuário selecione abp como o quarto argumento

                printf("\nÁrvore selecionada: ABP\n");

                while (!feof(lexiconFile)) //Percorre o arquivo inteiro (Não queria usar feof mas foi o que funcionou melhor
                {
                    fscanf(lexiconFile, "%[^;];%f\n", lexiconWord, &lexiconPolarity); //Retira as informações desejadas do arquivo formatado, e guarda nas variáveis
                    lexiconAbp = inserirAbp(lexiconAbp, lexiconWord, lexiconPolarity); //Insere na árvore
                }

            } else if(strcmp(argv[4], "avl")==0) { //Caso o usuário seleciona avl como o quarto argumento

                printf("\nÁrvore selecionada: AVL\n");

                while (!feof(lexiconFile)) //Percore o arquivo inteiro
                {
                    fscanf(lexiconFile, "%[^;];%f\n", lexiconWord, &lexiconPolarity); //Retira as informações desejadas do arquivo formatado, e guarda nas variáveis
                    lexiconAvl = inserirAvl(lexiconAvl, lexiconWord, lexiconPolarity); //Insere na árvore
                }

            } else { //Caso o usuário não selecione um tipo de árvore válido

                printf("\nTipo de árvore selecionado incorreto: Exemplo de chamada: %s\n", EXEMPLO_DE_CHAMADA);
                return 1; //Encerra o programa

            }

            insertEnd = clock(); //Termina a contagem do tempo de inserção
            float insertTime = (float) (insertEnd - insertStart) / CLOCKS_PER_SEC * 1000; //Calcula o delta

            fclose(lexiconFile); //Fechando o arquivo com o léxico

            inputFile = fopen(argv[2], "r"); //Abrindo o arquivo de entrada
            if(!inputFile) { //Caso  não seja possível abrir o arquivo de entrada

                printf("\nErro a abrir o arquivo %s\n", argv[2]);
                return 1; //Encerra o programa

            } else {

                printf("\nArquivo %s aberto com sucesso\n", argv[2]);

                outputFile = fopen(argv[3], "w"); //Abrindo o arquivo de saída
                if (!outputFile) { //Caso  não seja possível abrir o arquivo de saída

                    printf("\nErro ao abrir o arquivo %s\n", argv[3]);
                    return 1; //Encerra o programa

                } else { //Caso seja possível abrir o arquivo de saída

                    printf("\nArquivo %s aberto com sucesso\n", argv[3]);

                    comparisonStart = clock(); //Inicia a contagem do tempo de comparação

                    //Declarando variáveis que serão usadas para a retirada de informações do arquivo de entrada
                    float wordPolarity; //Variável que vai guardar a polaridade de uma palavra
                    float polaritySum; //Guarda a soma de polaridades de um texto
                    int wordCount; //Guarda o número de palavras em um texto (fiz com média no começo)
                    float overallPolarity; //Guarda a média de polaridade de um texto

                    while(fgets(linha, sizeof(linha), inputFile)) { //Recebendo dados da linha do arquivo de entradas

                        strcpy(linhaPrint, linha); //Copia o valor do gets() para uma variável que não vai mudar conforme o strtok()

                        //Inicializa as variáveis auxiliares
                        wordCount=0;
                        wordPolarity=0;
                        polaritySum=0;
                        overallPolarity=0;

                        token = strtok(linha, delim); //Tokeniza a linha e guarda em token

                        if (token!=NULL) //Caso o ponteiro não seja null (da erro de execução se for)
                            strcpy(inputWord, token); //Copia o valor pra variável de comparação

                        while(token!=NULL) { //Percorre o texto inteiro
                            if(strcmp(argv[4], "abp")==0) { //Caso tenha inicializado uma abp
                                wordPolarity = consultarPolaridadeAbp(lexiconAbp, inputWord); //Insere na abp
                            }
                            if(strcmp(argv[4], "avl")==0) //Caso tenh ainicializaco uma AVL
                                wordPolarity = consultarPolaridadeAvl(lexiconAvl, inputWord); //Insere na AVL
                            if(wordPolarity) //Caso haja um valor de polaridade para a palavra
                                wordCount++; //Conta a palavra para fazer a média (fiz assim no começo mas mudei pra soma no final)

                            polaritySum = polaritySum + wordPolarity; //Atualiza a soma da polaridade do texto

                            token = strtok(NULL, delim); //Acessa o próximo token da linha
                            if(token!=NULL) //Testa se é NULL
                                strcpy(inputWord, token); //Se existir, copia para a próxima palavra;
                        }

                        overallPolarity = polaritySum / wordCount; //Calcula a média de polaridade das palavras que têm valor léxico

                        fprintf(outputFile,"%.1f, %s", polaritySum, linhaPrint); //Printa os valores da soma de polaridade e os textos originais no arquivo de saída

                    }

                    comparisonEnd = clock(); //Termina o tempo de comparação
                    float comparisonTime = (float) (comparisonEnd - comparisonStart) / CLOCKS_PER_SEC * 1000; //Calcula o delta

                    printf("\nArquivo %s gerado com sucesso. \n", argv[3]);

                    //Printa as informações de comparação de performance entre as árvores
                    printf("\nTempo de Inserção: %.5f ms\nTempo de comparação: %.5f ms\n", insertTime, comparisonTime);
                    printf("\nInserção em ABP: %d\nInserção em AVL: %d\n", abpInsertion, avlInsertion);
                    printf("\nComparação em ABP: %d\nComparação de AVL: %d \nRotação em AVL: %d\n", abpComparison, avlComparison, rotation);

                    fclose(outputFile); //Fechando arquivo de saída
                }
                fclose(inputFile); //Fechando o arquivo de entrada
            }
        }
    }
}

//Termina a execução do programa
