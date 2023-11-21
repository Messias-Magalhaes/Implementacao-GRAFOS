#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define true 1
#define false 0

#define V 6

typedef int bool;
typedef int TIPOPESO;

typedef struct adjacencia {
    int vertice;
    TIPOPESO peso;
    struct adjacencia *prox;
} ADJACENCIA;

typedef struct vertice {
    ADJACENCIA *cab;
} VERTICE;

typedef struct grafo {
    int vertices;
    int arestas;
    VERTICE *adj;
} GRAFO;

typedef struct fila {
    int *itens;
    int frente, fundo;
} FILA;

GRAFO *criaGrafo(int v) {
    int i;

    GRAFO *g = (GRAFO *)malloc(sizeof(GRAFO));
    g->vertices = v;
    g->arestas = 0;
    g->adj = (VERTICE *)malloc(v * sizeof(VERTICE));

    for (i = 0; i < v; i++) {
        g->adj[i].cab = NULL;
    }
    return (g);
}

ADJACENCIA *criaAdj(int v, int peso) {
    ADJACENCIA *temp = (ADJACENCIA *)malloc(sizeof(ADJACENCIA));
    temp->vertice = v;
    temp->peso = peso;
    temp->prox = NULL;
    return (temp);
}

bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p) {
    if (!gr) {
        return (false);
    }
    if ((vf < 0) || (vf >= gr->vertices)) {
        return (false);
    }
    if ((vi < 0) || (vf >= gr->vertices)) {
        return (false);
    }

    ADJACENCIA *novo = criaAdj(vf, p);

    novo->prox = gr->adj[vi].cab;
    gr->adj[vi].cab = novo;
    gr->arestas++;
    return (true);
}

bool criaArestaND(GRAFO *gr, int vi, int vf, TIPOPESO p) {
    if (!gr) {
        return false;
    }
    if (vf < 0 || vf >= gr->vertices) {
        return false;
    }
    if (vi < 0 || vi >= gr->vertices) {
        return false;
    }

    ADJACENCIA *novo = criaAdj(vf, p);
    novo->prox = gr->adj[vi].cab;
    gr->adj[vi].cab = novo;

    // Para um grafo não direcionado, adicione a aresta no sentido contrário também
    ADJACENCIA *novo_inverso = criaAdj(vi, p);
    novo_inverso->prox = gr->adj[vf].cab;
    gr->adj[vf].cab = novo_inverso;

    gr->arestas++;
    return true;
}

void imprimeMatrizAdjacencia(GRAFO *gr) {
    int i, j;
    printf("\nMatriz de Adjacencia:\n");
    for (i = 0; i < gr->vertices; i++) {
        for (j = 0; j < gr->vertices; j++) {
            int encontrado = 0;
            ADJACENCIA *ad = gr->adj[i].cab;
            while (ad) {
                if (ad->vertice == j) {
                    encontrado = 1;
                    printf("1 ");
                    break;
                }
                ad = ad->prox;
            }
            if (!encontrado) {
                printf("0 ");
            }
        }
        printf("\n");
    }
}

void imprimeListaAdjacencia(GRAFO *gr) {
    int i;
    printf("\nLista de Adjacencia:\n");
    for (i = 0; i < gr->vertices; i++) {
        printf("v%d: ", i);
        ADJACENCIA *ad = gr->adj[i].cab;
        while (ad) {
            printf("v%d(%d) ", ad->vertice, ad->peso);
            ad = ad->prox;
        }
        printf("\n");
    }
}

bool filaVazia(FILA *fila) {
    return (fila->frente == -1);
}

void enfileirar(FILA *fila, int item) {
    if (fila->fundo == -1) {
        fila->frente = 0;
    }
    fila->fundo++;
    fila->itens[fila->fundo] = item;
}

int desenfileirar(FILA *fila) {
    int item = fila->itens[fila->frente];
    fila->frente++;
    if (fila->frente > fila->fundo) {
        fila->frente = fila->fundo = -1;
    }
    return item;
}

void buscaLargura(GRAFO *gr, int inicio) {
    FILA *fila = (FILA *)malloc(sizeof(FILA));
    fila->itens = (int *)malloc(gr->vertices * sizeof(int));
    fila->frente = -1;
    fila->fundo = -1;

    bool *visitado = (bool *)malloc(gr->vertices * sizeof(bool));

    for (int i = 0; i < gr->vertices; i++) {
        visitado[i] = false;
    }

    visitado[inicio] = true;
    enfileirar(fila, inicio);

    printf("\nBusca em Largura a partir do vertice %d: ", inicio);

    while (!filaVazia(fila)) {
        int vertice = desenfileirar(fila);
        printf("%d ", vertice);

        ADJACENCIA *ad = gr->adj[vertice].cab;
        while (ad) {
            int adjacente = ad->vertice;
            if (!visitado[adjacente]) {
                visitado[adjacente] = true;
                enfileirar(fila, adjacente);
            }
            ad = ad->prox;
        }
    }

    free(fila->itens);
    free(fila);
    free(visitado);
    printf("\n");
}

void imprimeVetor(int *vetor, int tamanho, char *mensagem) {
    printf("%s", mensagem);
    for (int i = 0; i < tamanho; i++) {
        printf(" %d", vetor[i]);
    }
    printf("\n");
}

void dfs(GRAFO *gr, int inicio, int *visitado, int *sequencia, int *ordemVisitados, int *contador, int *seqCount) {
    visitado[inicio] = 1;
    ordemVisitados[*contador] = inicio;
    (*contador)++;

    ADJACENCIA *ad = gr->adj[inicio].cab;
    while (ad) {
        if (!visitado[ad->vertice]) {
            dfs(gr, ad->vertice, visitado, sequencia, ordemVisitados, contador, seqCount);
        }
        ad = ad->prox;
    }
    sequencia[*seqCount] = inicio;
    (*seqCount)++;
}

void buscaEmProfundidade(GRAFO *gr, int inicio) {
    int *visitado = (int *)malloc(gr->vertices * sizeof(int));
    int *sequencia = (int *)malloc(gr->vertices * sizeof(int));
    int *ordemVisitados = (int *)malloc(gr->vertices * sizeof(int));
    int contador = 0;
    int seqCount = 0;

    for (int i = 0; i < gr->vertices; i++) {
        visitado[i] = 0;
    }

    dfs(gr, inicio, visitado, sequencia, ordemVisitados, &contador, &seqCount);
    
    printf("\nBusca em largura a partir do indice %d: \n", inicio);

    imprimeVetor(ordemVisitados, contador, "\nOrdem de visitacaoo dos vertices:");
    imprimeVetor(sequencia, seqCount, "\nVetor de sequencia:");

    free(visitado);
    free(sequencia);
    free(ordemVisitados);
}

int minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

void printSolution(int dist[]) {
    printf("Vertice \t Distancia do Vertice Inicial\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

void dijkstra(int graph[V][V], int src) {
    int dist[V];

    bool sptSet[V];

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);

        sptSet[u] = true;

        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    printSolution(dist);
}

int main() {

    int graph[V][V] = {{0, 1, 4, 0, 0, 0},
                       {1, 0, 4, 2, 7, 0},
                       {4, 4, 0, 3, 5, 0},
                       {0, 2, 3, 0, 4, 6},
                       {0, 7, 5, 4, 0, 7},
                       {0, 0, 0, 6, 7, 0}};

    dijkstra(graph, 0);

    return 0;
}
