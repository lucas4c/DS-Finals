struct nodoAvl //Declarando a estrutura de AVL
{
    char palavra[100]; //Atributo que vai segurar o valor da palavra do léxico e também vai servir de chave
    float polaridade; //Atributo que segura o valor da polaridade, não é atributo chave
    struct nodoAvl *esq; //Subárvores
    struct nodoAvl *dir;
    int altura; //Atributo que vai segurar a altura de toda subárvore da AVL
};
typedef struct nodoAvl Avl; //Declarando constante pra apontar para a estrutura da AVL

int max(int a, int b); //Função que retorna o maior entre dois números

int altura(Avl *arvore); //Função auxiliar que retona a altura de um nodo

Avl* novoNodo(char palavra[100], float polaridade); //Função auxiliar que inicializa um novo nodo

Avl* rotacaoDir(Avl *y); //Função para fazer rotação a direita

Avl* rotacaoEsq(Avl *x); //Função para fazer a rotação a esquerda

int calcBalanceamento(Avl *arvore); //Função pra calcular o balanceamento de uma AVL

Avl* inserirAvl(Avl *arvore, char palavra[100], float polaridade); //Função para inserir um nodo em uma AVL

float consultarPolaridadeAvl(Avl *arvore, char palavra[100]); //Função de consulta específica para retornar a polaridade de um nodo

void imprimirAvl(Avl *arvore); //Função auxiliar para imprimir a árvore usando caminhamento central à esquerda
