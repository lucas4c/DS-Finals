struct nodoAbp{ //Declarando a estrutura da ABP
    char palavra[100]; //Atributo que vai segurar o valor fa palavra do léxico e também vai ser atributo chave
    float polaridade; //Atributo que segura o valor da polaridade, não é atributo chave
    struct nodoAbp *esq; //Subárvores
    struct nodoAbp *dir;
};

typedef struct nodoAbp Abp; //Declarando constante pra apontar para a estrutura da ABP

Abp* inserirAbp(Abp *arvore, char palavra[100], float polaridade);//Função que insere um nodo na ABP

float consultarPolaridadeAbp(Abp *arvore, char palavra[100]); //Função de consulta específica para retornar a polaridade de um nodo

void imprimirAbp(Abp *arvore); //Função auxiliar para imprimir uma árvore usando caminhamento pré-fixado à esquerda
