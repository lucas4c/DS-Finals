#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abp.h"

//inicializando vari�veis globais

int abpComparison = 0; //Vari�vel para contar as compara��es em consultas na ABP
int abpInsertion = 0; //Vari�vel para contar as inser��es na ABP

Abp* inserirAbp(Abp *arvore, char palavra[100], float polaridade) { //Fun��o que insere um n� em uma ABP

    if (arvore==NULL) { //Caso ache um n� folha, declara um nodo, similar a fun��o novoNodo() da AVL;
        abpInsertion++; //Conta a inser��o
        arvore = (Abp*)malloc(sizeof(Abp));
        strcpy(arvore->palavra, palavra);
        arvore->polaridade = polaridade;
        arvore->esq = NULL;
        arvore->dir = NULL;
    }
    else //Caso a �rvore n�o esteja vazia
        if (strcmp(palavra, arvore->palavra) < 0) //Compara a ordem l�xicogr�fica da palavra do nodo
            arvore->esq = inserirAbp(arvore->esq, palavra, polaridade); //Insere na esquerda se for menor
        else
            arvore->dir = inserirAbp(arvore->dir, palavra, polaridade); //Insere na direita se for maior

    return arvore; //Retorna a �rvore

}

float consultarPolaridadeAbp(Abp *arvore, char palavra[100]) { //Fun��o espec�fica para retornar a polaridade de um nodo
    while (arvore!=NULL) { //Caso a �rvore n�o esteja vazia
        abpComparison++; //Conta a compara��o
        if (strcmp(arvore->palavra, palavra)==0) { //Caso ache a palavra
            abpComparison++; //Conta a compara��o
            return arvore->polaridade; //Retorna a polaridade da palavra
        }
        else { //Caso n�o seja a palavra que est� procurando
            abpComparison++; //Conta a compara��o
            if (strcmp(palavra, arvore->palavra)<0) { //Compara a ordem l�xicogr�fica
                arvore=arvore->esq; //Procura a esquerda se for menor
            }
            else {
                arvore=arvore->dir; //Procura a direita se for maior
            }
        }
    }
    return 0; //Caso n�o ache o nodo
}

void imprimirAbp(Abp *arvore) { //Caminhamento pr�-fixado a esquerda
    if(arvore!=NULL) {
        printf("| (%s) (%.1f) |", arvore->palavra, arvore->polaridade);
        imprimirAbp(arvore->esq);
        imprimirAbp(arvore->dir);
    }
}
