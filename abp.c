#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abp.h"

//inicializando variáveis globais

int abpComparison = 0; //Variável para contar as comparações em consultas na ABP
int abpInsertion = 0; //Variável para contar as inserções na ABP

Abp* inserirAbp(Abp *arvore, char palavra[100], float polaridade) { //Função que insere um nó em uma ABP

    if (arvore==NULL) { //Caso ache um nó folha, declara um nodo, similar a função novoNodo() da AVL;
        abpInsertion++; //Conta a inserção
        arvore = (Abp*)malloc(sizeof(Abp));
        strcpy(arvore->palavra, palavra);
        arvore->polaridade = polaridade;
        arvore->esq = NULL;
        arvore->dir = NULL;
    }
    else //Caso a árvore não esteja vazia
        if (strcmp(palavra, arvore->palavra) < 0) //Compara a ordem léxicográfica da palavra do nodo
            arvore->esq = inserirAbp(arvore->esq, palavra, polaridade); //Insere na esquerda se for menor
        else
            arvore->dir = inserirAbp(arvore->dir, palavra, polaridade); //Insere na direita se for maior

    return arvore; //Retorna a árvore

}

float consultarPolaridadeAbp(Abp *arvore, char palavra[100]) { //Função específica para retornar a polaridade de um nodo
    while (arvore!=NULL) { //Caso a árvore não esteja vazia
        abpComparison++; //Conta a comparação
        if (strcmp(arvore->palavra, palavra)==0) { //Caso ache a palavra
            abpComparison++; //Conta a comparação
            return arvore->polaridade; //Retorna a polaridade da palavra
        }
        else { //Caso não seja a palavra que está procurando
            abpComparison++; //Conta a comparação
            if (strcmp(palavra, arvore->palavra)<0) { //Compara a ordem léxicográfica
                arvore=arvore->esq; //Procura a esquerda se for menor
            }
            else {
                arvore=arvore->dir; //Procura a direita se for maior
            }
        }
    }
    return 0; //Caso não ache o nodo
}

void imprimirAbp(Abp *arvore) { //Caminhamento pré-fixado a esquerda
    if(arvore!=NULL) {
        printf("| (%s) (%.1f) |", arvore->palavra, arvore->polaridade);
        imprimirAbp(arvore->esq);
        imprimirAbp(arvore->dir);
    }
}
