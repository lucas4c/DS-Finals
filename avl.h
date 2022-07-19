struct nodoAvl //Declarando a estrutura de AVL
{
    char palavra[100]; //Atributo que vai segurar o valor da palavra do l�xico e tamb�m vai servir de chave
    float polaridade; //Atributo que segura o valor da polaridade, n�o � atributo chave
    struct nodoAvl *esq; //Sub�rvores
    struct nodoAvl *dir;
    int altura; //Atributo que vai segurar a altura de toda sub�rvore da AVL
};
typedef struct nodoAvl Avl; //Declarando constante pra apontar para a estrutura da AVL

int max(int a, int b); //Fun��o que retorna o maior entre dois n�meros

int altura(Avl *arvore); //Fun��o auxiliar que retona a altura de um nodo

Avl* novoNodo(char palavra[100], float polaridade); //Fun��o auxiliar que inicializa um novo nodo

Avl* rotacaoDir(Avl *y); //Fun��o para fazer rota��o a direita

Avl* rotacaoEsq(Avl *x); //Fun��o para fazer a rota��o a esquerda

int calcBalanceamento(Avl *arvore); //Fun��o pra calcular o balanceamento de uma AVL

Avl* inserirAvl(Avl *arvore, char palavra[100], float polaridade); //Fun��o para inserir um nodo em uma AVL

float consultarPolaridadeAvl(Avl *arvore, char palavra[100]); //Fun��o de consulta espec�fica para retornar a polaridade de um nodo

void imprimirAvl(Avl *arvore); //Fun��o auxiliar para imprimir a �rvore usando caminhamento central � esquerda
