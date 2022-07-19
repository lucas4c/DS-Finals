#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

// Inicializando variáveis globais

int avlComparison = 0; //Variável para contar as comparações em consultas na AVL
int avlInsertion = 0; //Variável para contar as inserções na AVL
int rotation = 0; //Variável para contar as rotações na AVL

int max(int a, int b)   //Função auxiliar pra saber o maior entre dois números
{
    return (a > b)? a:b;
}

int altura(Avl *arvore)   //Função auxiliar para retornar a altura da árvore
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

Avl* rotacaoDir(Avl *y)   //Faz a rotação para direita na AVL
{
    Avl *x = y->esq;
    Avl *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = max(altura(y->esq), altura(y->dir))+1;
    x->altura = max(altura(x->esq), altura(x->esq))+1;

    return x;
}

Avl* rotacaoEsq(Avl *x)   //Faz a rotação para esquerda na AVL
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
    return altura(arvore->esq) - altura(arvore->dir); //Retorna a subtração do valor das alturas das subárvores (Fator de balanceamento)
}

Avl* inserirAvl(Avl *arvore, char palavra[100], float polaridade)   //Insere um nodo na AVL
{

    if (arvore==NULL)
    {
        avlInsertion++; //Conta a inserção
        return(novoNodo(palavra, polaridade)); //Procura um nó folha para inserir a árvore
    }


    if (strcmp(palavra, arvore->palavra)<0) //Testa a ordem lexicográfica da palavra do nodo
        arvore->esq = inserirAvl(arvore->esq, palavra, polaridade); //Insere a esquerda se for menor
    else if (strcmp(palavra, arvore->palavra)>0)
        arvore->dir = inserirAvl(arvore->dir, palavra, polaridade); //Insere a direita se for maior
    else
        return arvore; //Caso for igual, o nodo não será inserido

    arvore->altura = 1+max(altura(arvore->esq), altura(arvore->dir)); //Recalcula a altura da árvore

    int balanceamento = calcBalanceamento(arvore); //Calcula o balanceamento e procede para fazer as rotações caso necessário

    if (balanceamento > 1 && strcmp(palavra, arvore->esq->palavra)<0) //Rotação simples a direita
    {
        rotation++;
        return rotacaoDir(arvore);
    }


    if (balanceamento < -1 && strcmp(palavra, arvore->dir->palavra)>0) //Rotação simples a esquerda
    {
        rotation++;
        return rotacaoEsq(arvore);
    }


    if (balanceamento > 1 && strcmp(palavra, arvore->esq->palavra)>0)    //Rotação dupla a direita
    {
        rotation++;
        arvore->esq = rotacaoEsq(arvore->esq);
        return rotacaoDir(arvore);
    }

    if (balanceamento < -1 && strcmp(palavra, arvore->dir->palavra)<0)   //Rotação dupla a esquerda
    {
        rotation++;
        arvore->dir = rotacaoDir(arvore->dir);
        return rotacaoEsq(arvore);
    }

    return arvore; //Retorna a arvore com o nó inserido

}

float consultarPolaridadeAvl(Avl *arvore, char palavra[100]) //Função específica para retornar a polaridade de um nodo
    {
    while (arvore!=NULL) //Caso a árvore não esteja vazia
    {
        avlComparison++; //Conta a comparação
        if (strcmp(arvore->palavra, palavra)==0) //Caso ache a palavra
        {
            avlComparison++; //Conta a comparação
            return arvore->polaridade; //Retorna a polaridade da palavra
        }
        else //Caso não seja a palavra que está procurando
            avlComparison++; //Conta a comparação
            if (strcmp(palavra, arvore->palavra)<0) //Compara a ordem léxicográfica
                arvore=arvore->esq; //Procura a esquerda se for menor

            else
                arvore=arvore->dir; //Procura a direita se for maior

    }
    return 0; //Caso não ache o nodo
}

void imprimirAvl(Avl *arvore)   //Caminhamento Central à esquerda
{
    if(arvore!=NULL)
    {
        imprimirAvl(arvore->esq);
        printf("%s [%.1f]", arvore->palavra, arvore->polaridade);
        imprimirAvl(arvore->dir);
    }
}
