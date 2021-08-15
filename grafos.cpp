#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <iostream>
#include <gvc.h>
#include <cgraph.h>


typedef struct adjacencia{
    int vertice;
    int peso;
    struct adjacencia* prox;
}ADJACENCIA;

typedef struct vertice{
    ADJACENCIA *cab;
    bool chave;
}VERTICE;

typedef struct grafo{
    int vertices;
    int arestas;
    VERTICE *adj;
}GRAFO;

typedef struct Lista{
    int vertice;
    Lista* prox;
}LISTA;

typedef struct Registro{
    int vertice;
    Registro* prox;
}REGISTRO;

typedef struct Fila{
    REGISTRO* cab;
    REGISTRO* final;
}FILA;


#define ERRO 9999999999
#define BRANCO 0
#define CINZA 1
#define PRETO 2


//------------------------------------Grafos indice da estrutura-------------------------------------------//
GRAFO *criaGrafo(int v); //-------------------------------------------------------Cria a estrutua de grafos//
ADJACENCIA *criaAdjacencia(int v, int peso); //-------------------------Cria a adjacencia entre os vertices//
bool criaAresta(GRAFO* gr, int vi, int vf, int peso);//------------------------------------Crias as arestas//
bool excluiArestas(GRAFO* gr, int vi, int vf);//------------------------------------------exclui as arestas//
void visitaP(GRAFO* g, int i, int* cor,Lista* *lista);//--------------------------------Percorre as arestas//
void inserirNaLista(Lista* *lista, int vertice);//---------------------------insere na lista se tiver chave//
LISTA* buscaProfundidade(GRAFO *g);//--------------------------------------prepara a busca por profundidade//
void realoc(GRAFO * g,int vi,int vf, int peso);//-------------------------------inverte a direção da aresta//
void adjacencia2incidencia(GRAFO* g);//---------------------------converte de lista adjacencia a incidencia//
void testeCriacao();//---------------------------------------------------------testa a criação da estrutura//
void inserirNaLista(Lista **lista, int vertice);//------------------------------------------insere ne lista//
int visitaL(GRAFO* grafo, int vertice, int* cor);//------------------------------------percorre as arestas//
void inserirNaFila(FILA *fila, REGISTRO *registro);//----------------------------------------insere na fila//
REGISTRO *excluirDafila(FILA *fila);//-------------------------------------------------------Exclui da fila//
FILA criarFila();//-----------------------------------------------------------------------------cria a fila//
REGISTRO *criarRegistro(int vertice);//-------------------------------------------------------cria registro//
//---------------------------------------------------------------------------------------------------------//





GRAFO *criaGrafo(int v){
    GRAFO *g = (GRAFO*) malloc(sizeof(GRAFO));
    g->vertices = v;
    g->arestas = 0;
    g->adj = (VERTICE*) malloc(v * sizeof(VERTICE));
    for(int i = 0; i< v; i++){
        g->adj[i].cab = NULL;
        g->adj[i].chave = false;
        if(i%3 == 0)
            g->adj[i].chave = true;
    }

    return g;
}

ADJACENCIA *criaAdjacencia(int v, int peso){
    ADJACENCIA *temp = (ADJACENCIA*) malloc(sizeof(ADJACENCIA));
    temp->vertice = v;
    temp->peso = peso;
    temp->prox = NULL;
    return temp;
}

bool criaAresta(GRAFO* gr, int vi, int vf, int peso){
    if(!gr)
        return false;

    if((vf<0) || (vf >= gr->vertices))
        return false;

    if((vi<0) || vi >= gr->vertices)
        return false;

    ADJACENCIA *novo = criaAdjacencia(vf,peso);
    novo->prox = gr->adj[vi].cab;
    gr->adj[vi].cab = novo;
    gr->arestas++;
    return true;
}

bool excluiArestas(GRAFO* gr, int vi, int vf){
    ADJACENCIA *iteracao = gr->adj[vi].cab;
    ADJACENCIA *back = NULL;

    while(iteracao && iteracao->vertice != vf){
        back = iteracao;
        iteracao = iteracao->prox;
    }

    if(!iteracao)
        return false;

    if(back && iteracao->prox)
        back->prox = iteracao->prox;
    else if (back)
        back->prox = NULL;
    else if(iteracao->prox)
        gr->adj[vi].cab = iteracao->prox;
    else
        gr->adj[vi].cab = NULL;

    free(iteracao);
    gr->arestas --;

    return true;
}



void imprime(GRAFO *gr){

    if(!gr) return;

    printf("Vertices: %d. Arestas %d. \n",gr->vertices, gr->arestas);
    for(int i = 0; i<gr->vertices; i++){
        printf("v%d",i);
        if(gr->adj[i].chave)
            printf("K: ");
        else printf(": ");
        ADJACENCIA *adj = gr->adj[i].cab;
        while(adj){
            printf("v%d(%d) ", adj->vertice, adj->peso);
            adj = adj->prox;
        }
        printf("\n");
    }
}



//BUSCA EM PROFUNDIDADE

void visitaP(GRAFO* g, int i, int* cor,Lista* *lista){
    cor[i] = CINZA;
    ADJACENCIA *v = g->adj[i].cab;
    if(g->adj[i].chave)
        inserirNaLista(lista,i);
    while(v){
        if(cor[v->vertice] == BRANCO)
            visitaP(g, v->vertice, cor, lista);
        v = v->prox;
    }
    cor[i] = PRETO;
}

void inserirNaLista(Lista* *lista, int vertice) {

    if(!*lista){
        LISTA *novo =  (LISTA*) malloc(sizeof(LISTA));
        novo->prox = NULL;
        novo->vertice = vertice;
        *lista = novo;
        return;
    }

    Lista* i = *lista;

    while(i->prox){
        i = i->prox;
        if(i->vertice == vertice)
            return;
    }

    LISTA *novo = (LISTA*) malloc(sizeof(LISTA));
    novo->prox = NULL;
    novo->vertice = vertice;
    i->prox = novo;

}

LISTA* buscaProfundidade(GRAFO *g){
    int cor[g->vertices];
    Lista *lista = NULL;

    for (int i = 0; i < g->vertices; ++i)
        cor[i] = BRANCO;

    for (int i = 0; i < g->vertices; ++i){
        if(cor[i] == BRANCO)
            visitaP(g,i,cor,&lista);
    }

    return lista;
}

//BUSCA EM LARGURA

int visitaL(GRAFO* grafo, int vertice, int* cor, int* melhor){
    FILA fila = criarFila();
    int caminho = 0;
    cor[vertice] = PRETO;
    REGISTRO* u = criarRegistro(vertice);
    inserirNaFila(&fila,u);
    //printf("%d ",u->vertice);
    while(fila.cab){

        u = excluirDafila(&fila);

        if(grafo->adj[u->vertice].chave && melhor[vertice] > caminho){
            melhor[vertice] = caminho;
            caminho--;
        }

        caminho++;

        ADJACENCIA *v = grafo->adj[u->vertice].cab;
        while(v){
            if(cor[v->vertice] == BRANCO){
                cor[v->vertice] = PRETO;
                REGISTRO* aux = criarRegistro(v->vertice);
                inserirNaFila(&fila,aux);
                free(aux);
                //printf("%d ",v->vertice);
            }

            if(cor[v->vertice] == PRETO && melhor[vertice] > (caminho +melhor[v->vertice]))
                melhor[vertice] = caminho + melhor[v->vertice];

            v = v->prox;
        }

    }

    return melhor[vertice];
}

REGISTRO *criarRegistro(int vertice) {
    Registro* registro = (REGISTRO*) malloc(sizeof(REGISTRO));
    registro->vertice = vertice;
    registro->prox = NULL;

    return registro;
}

FILA criarFila() {
    FILA *fila = (FILA*) malloc(sizeof(Fila));
    fila->cab = NULL;
    fila->final = NULL;

    return *fila;
}

REGISTRO *excluirDafila(FILA *fila) {

    REGISTRO* registro = (REGISTRO*) malloc(sizeof(REGISTRO));
    registro->prox = NULL;

    REGISTRO *aux;

    if(!fila->cab)
        return NULL;

    aux = fila->cab;
    fila->cab = fila->cab->prox;
    registro->vertice = aux->vertice;
    free(aux);

    if(!fila->cab)
        fila->final = NULL;

    return registro;

}

void inserirNaFila(FILA *fila, REGISTRO *registro) {

    REGISTRO* novo = (REGISTRO*) malloc(sizeof(REGISTRO));
    novo->vertice = registro->vertice;
    novo->prox = NULL;
   if(fila->final)
       fila->final->prox = novo;
   else
       fila->cab = novo;

   fila->final = novo;
}

void buscaLargura(grafo* g){

    int cor[g->vertices];
    int melhor[g->vertices];

    for (int i = 0; i < g->vertices; ++i){
        melhor[i] = ERRO;
    }


    for (int i = 0; i < g->vertices; ++i) {
        for (int i = 0; i < g->vertices; ++i)
            cor[i] = BRANCO;
        //printf("\n");
        printf(" melhor caminho partindo de %d vale %d \n",i,visitaL(g, i, cor,melhor));

    }

}

//Conversão adjacencai-incidenca

void realoc(GRAFO * g,int vi,int vf, int peso){
    criaAresta(g,vf,vi,peso);
    excluiArestas(g,vi,vf);
}



void adjacencia2incidencia(GRAFO* g) {

    int tamanho = g->vertices;

    for(int i = 0; i<tamanho; i++){
        ADJACENCIA* iteration = g->adj[i].cab;
        while (iteration){
            ADJACENCIA *next = iteration->prox;
            realoc(g, i, iteration->vertice, iteration->peso);
            iteration= next;
        }
    }


}


void testeCriacao(){
    GRAFO* gr = criaGrafo(7);
    criaAresta(gr,0,1,2);
    criaAresta(gr,1,2,2);
    criaAresta(gr,2,0,12);
    criaAresta(gr,2,4,40);
    criaAresta(gr,3,1,3);
    criaAresta(gr,4,3,8);
    criaAresta(gr,5,6,1);

    imprime(gr);

    buscaLargura(gr);

//    LISTA* lista = buscaProfundidade(gr);
//    while(lista){
//        printf("%d| ", lista->vertice);
//        lista = lista->prox;
//    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    testeCriacao();
    return 0;
}
