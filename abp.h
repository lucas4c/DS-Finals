struct nodoAbp{ //Declarando a estrutura da ABP
    char palavra[100]; //Atributo que vai segurar o valor fa palavra do l�xico e tamb�m vai ser atributo chave
    float polaridade; //Atributo que segura o valor da polaridade, n�o � atributo chave
    struct nodoAbp *esq; //Sub�rvores
    struct nodoAbp *dir;
};

typedef struct nodoAbp Abp; //Declarando constante pra apontar para a estrutura da ABP

Abp* inserirAbp(Abp *arvore, char palavra[100], float polaridade);//Fun��o que insere um nodo na ABP

float consultarPolaridadeAbp(Abp *arvore, char palavra[100]); //Fun��o de consulta espec�fica para retornar a polaridade de um nodo

void imprimirAbp(Abp *arvore); //Fun��o auxiliar para imprimir uma �rvore usando caminhamento pr�-fixado � esquerda
