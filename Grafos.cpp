#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define true 1
#define false 0
typedef int bool;
typedef int TIPOPESO;

typedef struct adjacencia {
	int vertice;
	TIPOPESO peso;
	struct adjacencia *prox;
}ADJACENCIA;

typedef struct vertice {
	ADJACENCIA *cab;
}VERTICE;

typedef struct grafo {
	int vertices;
	int arestas;
	VERTICE *adj;
}GRAFO;

GRAFO *criaGrafo (int v) {
	int i;
	
	GRAFO *g = (GRAFO*)malloc(sizeof(GRAFO));
	g->vertices = v;
	g->arestas = 0;
	g->adj = (VERTICE *)malloc(v * sizeof(VERTICE));

	for (i=0; i<v; i++){
		g->adj[i].cab=NULL;
	}
	return(g);
}

ADJACENCIA *criaAdj (int v, int peso){
	ADJACENCIA *temp = (ADJACENCIA*)malloc(sizeof(ADJACENCIA));
	temp->vertice = v;
	temp->peso = peso;
	temp->prox = NULL;
	return(temp);
}

bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p) {
	if(!gr) {
		return (false);
	}
	if((vf < 0)||(vf >= gr->vertices)){
		return (false);
	}
	if((vi < 0)||(vf >= gr->vertices)){
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

TIPOPESO calculaValorCaminho(GRAFO *gr, int origem, int destino) {
    int *distancia = (int *)malloc(gr->vertices * sizeof(int));
    bool *visitado = (bool *)malloc(gr->vertices * sizeof(bool));

    for (int i = 0; i < gr->vertices; i++) {
        distancia[i] = INT_MAX;
        visitado[i] = false;
    }

    distancia[origem] = 0;

    for (int count = 0; count < gr->vertices - 1; count++) {
        int u = -1;
        int min_distancia = INT_MAX;

        for (int i = 0; i < gr->vertices; i++) {
            if (!visitado[i] && distancia[i] < min_distancia) {
                u = i;
                min_distancia = distancia[i];
            }
        }

        if (u == -1) {
            break;
        }

        visitado[u] = true;
        ADJACENCIA *v = gr->adj[u].cab;

        while (v) {
            if (!visitado[v->vertice] && distancia[u] != INT_MAX && distancia[u] + v->peso < distancia[v->vertice]) {
                distancia[v->vertice] = distancia[u] + v->peso;
            }
            v = v->prox;
        }
    }

    int resultado = distancia[destino];

    free(distancia);
    free(visitado);

    if (resultado == INT_MAX) {
        return -1;
    } else {
        return resultado;
    }
}

int main() {
    GRAFO *grafo = criaGrafo(5);
    GRAFO *grafo1 = criaGrafo(5);

    criaArestaND(grafo, 0, 0, 0);
    criaArestaND(grafo, 0, 2, 0);
    criaArestaND(grafo, 2, 1, 0);
    criaArestaND(grafo, 2, 3, 0);
    criaArestaND(grafo, 1, 3, 0);
    criaArestaND(grafo, 1, 4, 0);
    criaArestaND(grafo, 3, 4, 0);
    
    imprimeMatrizAdjacencia(grafo);
    imprimeListaAdjacencia(grafo);
    
    criaAresta(grafo1, 0, 0, 1);
    criaAresta(grafo1, 0, 2, 2);
    criaAresta(grafo1, 2, 1, 3);
    criaAresta(grafo1, 2, 3, 4);
    criaAresta(grafo1, 1, 3, 5);
    criaAresta(grafo1, 1, 4, 6);
    criaAresta(grafo1, 3, 4, 7);

    imprimeMatrizAdjacencia(grafo1);
    imprimeListaAdjacencia(grafo1);
    
    int origem = 0;
    int destino = 4;
    TIPOPESO valorTotal = calculaValorCaminho(grafo1, origem, destino);

    if (valorTotal >= 0) {
        printf("\nValor total do caminho de {%d, %d} = %d\n", origem, destino, valorTotal);
    } else {
        printf("\nCaminho invalido ou nao encontrado.\n");
    }

    return 0;
}
