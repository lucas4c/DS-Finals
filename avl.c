#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

// Inicializando vari�veis globais

int avlComparison = 0; //Vari�vel para contar as compara��es em consultas na AVL
int avlInsertion = 0; //Vari�vel para contar as inser��es na AVL
int rotation = 0; //Vari�vel para contar as rota��es na AVL

int max(int a, int b)   //Fun��o auxiliar pra saber o maior entre dois n�meros
{
    return (a > b)? a:b;
}

int altura(Avl *arvore)   //Fun��o auxiliar para retornar a altura da �rvore
{
    if (arvore==NULL)
        return 0;
    return arvore->altura;
}

Avl* novoNodo(char palavra[100], float polaridade)   //Retorna um nodo formatado
{
    Avl* nodo = (Avl*)malloc(sizeof(Avl));
    strcpy(nodo->palavra, palavra);
    nodo->polaridade = polaridade;
    nodo->esq = NULL;
    nodo->dir = NULL;
    return(nodo);
}

Avl* rotacaoDir(Avl *y)   //Faz a rota��o para direita na AVL
{
    Avl *x = y->esq;
    Avl *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = max(altura(y->esq), altura(y->dir))+1;
    x->altura = max(altura(x->esq), altura(x->esq))+1;

    return x;
}

Avl* rotacaoEsq(Avl *x)   //Faz a rota��o para esquerda na AVL
{
    Avl *y = x->dir;
    Avl *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = max(altura(x->esq), altura(x->dir))+1;
    y->altura = max(altura(y->esq), altura(y->dir))+1;

    return y;
}

int calcBalanceamento(Avl* arvore)   //Calcula o balanceamento da AVL
{
    if(arvore==NULL)
        return 0;
    return altura(arvore->esq) - altura(arvore->dir); //Retorna a subtra��o do valor das alturas das sub�rvores (Fator de balanceamento)
}

Avl* inserirAvl(Avl *arvore, char palavra[100], float polaridade)   //Insere um nodo na AVL
{

    if (arvore==NULL)
    {
        avlInsertion++; //Conta a inser��o
        return(novoNodo(palavra, polaridade)); //Procura um n� folha para inserir a �rvore
    }


    if (strcmp(palavra, arvore->palavra)<0) //Testa a ordem lexicogr�fica da palavra do nodo
        arvore->esq = inserirAvl(arvore->esq, palavra, polaridade); //Insere a esquerda se for menor
    else if (strcmp(palavra, arvore->palavra)>0)
        arvore->dir = inserirAvl(arvore->dir, palavra, polaridade); //Insere a direita se for maior
    else
        return arvore; //Caso for igual, o nodo n�o ser� inserido

    arvore->altura = 1+max(altura(arvore->esq), altura(arvore->dir)); //Recalcula a altura da �rvore

    int balanceamento = calcBalanceamento(arvore); //Calcula o balanceamento e procede para fazer as rota��es caso necess�rio

    if (balanceamento > 1 && strcmp(palavra, arvore->esq->palavra)<0) //Rota��o simples a direita
    {
        rotation++;
        return rotacaoDir(arvore);
    }


    if (balanceamento < -1 && strcmp(palavra, arvore->dir->palavra)>0) //Rota��o simples a esquerda
    {
        rotation++;
        return rotacaoEsq(arvore);
    }


    if (balanceamento > 1 && strcmp(palavra, arvore->esq->palavra)>0)    //Rota��o dupla a direita
    {
        rotation++;
        arvore->esq = rotacaoEsq(arvore->esq);
        return rotacaoDir(arvore);
    }

    if (balanceamento < -1 && strcmp(palavra, arvore->dir->palavra)<0)   //Rota��o dupla a esquerda
    {
        rotation++;
        arvore->dir = rotacaoDir(arvore->dir);
        return rotacaoEsq(arvore);
    }

    return arvore; //Retorna a arvore com o n� inserido

}

float consultarPolaridadeAvl(Avl *arvore, char palavra[100]) //Fun��o espec�fica para retornar a polaridade de um nodo
    {
    while (arvore!=NULL) //Caso a �rvore n�o esteja vazia
    {
        avlComparison++; //Conta a compara��o
        if (strcmp(arvore->palavra, palavra)==0) //Caso ache a palavra
        {
            avlComparison++; //Conta a compara��o
            return arvore->polaridade; //Retorna a polaridade da palavra
        }
        else //Caso n�o seja a palavra que est� procurando
            avlComparison++; //Conta a compara��o
            if (strcmp(palavra, arvore->palavra)<0) //Compara a ordem l�xicogr�fica
                arvore=arvore->esq; //Procura a esquerda se for menor

            else
                arvore=arvore->dir; //Procura a direita se for maior

    }
    return 0; //Caso n�o ache o nodo
}

void imprimirAvl(Avl *arvore)   //Caminhamento Central � esquerda
{
    if(arvore!=NULL)
    {
        imprimirAvl(arvore->esq);
        printf("%s [%.1f]", arvore->palavra, arvore->polaridade);
        imprimirAvl(arvore->dir);
    }
}
