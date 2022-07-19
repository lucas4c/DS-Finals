#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "avl.c"
#include "abp.c"

//Inicializando vari�veis globais

extern int abpComparison; //Vari�vel global para contar as compara��es na ABP, vinda de abp.c
extern int avlComparison; //Vari�vel global para contar as compara��es na AVL, vinda de avl.c
extern int abpInsertion; //Vari�vel global para contar as inser��es na ABP, vinda de ABP.c
extern int avlInsertion; //Vari�vel global para contar as inser��es na AVL, vinda de avl.c
extern int rotation; //Vari�vel global para contar as rota��es na AVL, vinda de avl.c


int main(int argc, char *argv[]) //Exemplo de chamada do programa: nomeDoPrograma.exe <arquivo_lexico.txt> <arquivo_de_entrada.txt> <arquivo_de_saida.txt> <Tipo de �rvore (AVL ou ABP)>
{

    char *token; //Vari�vel auxiliar que vai segurar os tokens na hora de ler o arquivo de entrada
    char delim[64] = {" ,.&*%\?!;/-'@\"$#=><()][}{:\n\t"}; //Vari�vel auxiliar para guardar os diferentes separadores de palavras nos textos

    setlocale(LC_ALL, ""); //Deixa leg�veis os textos em portugu�s

    clock_t insertStart, insertEnd; //Declara as vari�veis de contagem de tempo de execu��o de inser��o
    clock_t comparisonStart, comparisonEnd; //E compara��o

    float insertTime; //Declara a vari�vel que vai segurar o valor final do tempo de inser��o
    float comparisonTime; //E do tempo de compara��o

    clock_t start, end; //Inicializa a vari�vel para a contagem do tempo de execu��o

    const char EXEMPLO_DE_CHAMADA[200] = "lexico.exe <lexico.txt> <arquivo_de_entrada.txt> <arquivo_de_saida.txt> <Tipo de �rvore (abp ou avl)>";

    FILE * lexiconFile; //Arquivo contendo o l�xico
    FILE * inputFile; //Arquivo de entrada
    FILE * outputFile; //Arquivo de sa�da

    char lexiconWord[100]; //Vari�vel que vai reter o valor das palavras tirada do arquivo l�xico
    float lexiconPolarity; //Vari�vel que vai reter o valor da polaridade tirada do arquivo l�xico

    char linha[1024]; //Vari�vel auxiliar que vai ser tokenizada e vai prover as palavras para as consultas nas ABPs
    char linhaPrint[1024]; //Vari�vel que vai receber o valor dos textos tirados do arquivo de entrada, e ser� printada no arquivo de sa�da
    char inputWord[100]; //Vari�vel que vai receber o valor de cada palavra lida do arquivo de entrada

    Abp* lexiconAbp = NULL; //Inicializa as ABPs (N�o consegui pensar em um jeito mais elegante)
    Avl* lexiconAvl = NULL;

    if (argc!=5) //Caso o programa n�o receba os argumentos corretamente
    {
        printf("\nNumero incorreto de parametros.\nExemplo de chamada: %s\n", EXEMPLO_DE_CHAMADA);
        return 1; //Para a execu��o
    }
    else //Caso o programa receba o n�mero correto de argumentos
    {

        printf("\nAN�LISE DE SENTIMENTOS\n");

        lexiconFile = fopen(argv[1], "r"); //Abre o arquivo do l�xico
        if(!lexiconFile) //Caso n�o seja capaz de abrir o arquivo do l�xico
        {
            printf("\nErro ao abrir o arquivo %s\n", argv[1]);
            return 1; //Para a execu��o
        }
        else //Caso o arquivo seja aberto com sucesso
        {
            printf("\nArquivo %s aberto com sucesso.\n", argv[1]);

            insertStart = clock(); //Inicia o tempo de inser��o

            if (strcmp(argv[4], "abp")==0) { //Caso o usu�rio selecione abp como o quarto argumento

                printf("\n�rvore selecionada: ABP\n");

                while (!feof(lexiconFile)) //Percorre o arquivo inteiro (N�o queria usar feof mas foi o que funcionou melhor
                {
                    fscanf(lexiconFile, "%[^;];%f\n", lexiconWord, &lexiconPolarity); //Retira as informa��es desejadas do arquivo formatado, e guarda nas vari�veis
                    lexiconAbp = inserirAbp(lexiconAbp, lexiconWord, lexiconPolarity); //Insere na �rvore
                }

            } else if(strcmp(argv[4], "avl")==0) { //Caso o usu�rio seleciona avl como o quarto argumento

                printf("\n�rvore selecionada: AVL\n");

                while (!feof(lexiconFile)) //Percore o arquivo inteiro
                {
                    fscanf(lexiconFile, "%[^;];%f\n", lexiconWord, &lexiconPolarity); //Retira as informa��es desejadas do arquivo formatado, e guarda nas vari�veis
                    lexiconAvl = inserirAvl(lexiconAvl, lexiconWord, lexiconPolarity); //Insere na �rvore
                }

            } else { //Caso o usu�rio n�o selecione um tipo de �rvore v�lido

                printf("\nTipo de �rvore selecionado incorreto: Exemplo de chamada: %s\n", EXEMPLO_DE_CHAMADA);
                return 1; //Encerra o programa

            }

            insertEnd = clock(); //Termina a contagem do tempo de inser��o
            float insertTime = (float) (insertEnd - insertStart) / CLOCKS_PER_SEC * 1000; //Calcula o delta

            fclose(lexiconFile); //Fechando o arquivo com o l�xico

            inputFile = fopen(argv[2], "r"); //Abrindo o arquivo de entrada
            if(!inputFile) { //Caso  n�o seja poss�vel abrir o arquivo de entrada

                printf("\nErro a abrir o arquivo %s\n", argv[2]);
                return 1; //Encerra o programa

            } else {

                printf("\nArquivo %s aberto com sucesso\n", argv[2]);

                outputFile = fopen(argv[3], "w"); //Abrindo o arquivo de sa�da
                if (!outputFile) { //Caso  n�o seja poss�vel abrir o arquivo de sa�da

                    printf("\nErro ao abrir o arquivo %s\n", argv[3]);
                    return 1; //Encerra o programa

                } else { //Caso seja poss�vel abrir o arquivo de sa�da

                    printf("\nArquivo %s aberto com sucesso\n", argv[3]);

                    comparisonStart = clock(); //Inicia a contagem do tempo de compara��o

                    //Declarando vari�veis que ser�o usadas para a retirada de informa��es do arquivo de entrada
                    float wordPolarity; //Vari�vel que vai guardar a polaridade de uma palavra
                    float polaritySum; //Guarda a soma de polaridades de um texto
                    int wordCount; //Guarda o n�mero de palavras em um texto (fiz com m�dia no come�o)
                    float overallPolarity; //Guarda a m�dia de polaridade de um texto

                    while(fgets(linha, sizeof(linha), inputFile)) { //Recebendo dados da linha do arquivo de entradas

                        strcpy(linhaPrint, linha); //Copia o valor do gets() para uma vari�vel que n�o vai mudar conforme o strtok()

                        //Inicializa as vari�veis auxiliares
                        wordCount=0;
                        wordPolarity=0;
                        polaritySum=0;
                        overallPolarity=0;

                        token = strtok(linha, delim); //Tokeniza a linha e guarda em token

                        if (token!=NULL) //Caso o ponteiro n�o seja null (da erro de execu��o se for)
                            strcpy(inputWord, token); //Copia o valor pra vari�vel de compara��o

                        while(token!=NULL) { //Percorre o texto inteiro
                            if(strcmp(argv[4], "abp")==0) { //Caso tenha inicializado uma abp
                                wordPolarity = consultarPolaridadeAbp(lexiconAbp, inputWord); //Insere na abp
                            }
                            if(strcmp(argv[4], "avl")==0) //Caso tenh ainicializaco uma AVL
                                wordPolarity = consultarPolaridadeAvl(lexiconAvl, inputWord); //Insere na AVL
                            if(wordPolarity) //Caso haja um valor de polaridade para a palavra
                                wordCount++; //Conta a palavra para fazer a m�dia (fiz assim no come�o mas mudei pra soma no final)

                            polaritySum = polaritySum + wordPolarity; //Atualiza a soma da polaridade do texto

                            token = strtok(NULL, delim); //Acessa o pr�ximo token da linha
                            if(token!=NULL) //Testa se � NULL
                                strcpy(inputWord, token); //Se existir, copia para a pr�xima palavra;
                        }

                        overallPolarity = polaritySum / wordCount; //Calcula a m�dia de polaridade das palavras que t�m valor l�xico

                        fprintf(outputFile,"%.1f, %s", polaritySum, linhaPrint); //Printa os valores da soma de polaridade e os textos originais no arquivo de sa�da

                    }

                    comparisonEnd = clock(); //Termina o tempo de compara��o
                    float comparisonTime = (float) (comparisonEnd - comparisonStart) / CLOCKS_PER_SEC * 1000; //Calcula o delta

                    printf("\nArquivo %s gerado com sucesso. \n", argv[3]);

                    //Printa as informa��es de compara��o de performance entre as �rvores
                    printf("\nTempo de Inser��o: %.5f ms\nTempo de compara��o: %.5f ms\n", insertTime, comparisonTime);
                    printf("\nInser��o em ABP: %d\nInser��o em AVL: %d\n", abpInsertion, avlInsertion);
                    printf("\nCompara��o em ABP: %d\nCompara��o de AVL: %d \nRota��o em AVL: %d\n", abpComparison, avlComparison, rotation);

                    fclose(outputFile); //Fechando arquivo de sa�da
                }
                fclose(inputFile); //Fechando o arquivo de entrada
            }
        }
    }
}

//Termina a execu��o do programa
