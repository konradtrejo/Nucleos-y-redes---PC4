#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cassert>
#include <iostream>
#include <fstream>

#include "omp.h"

#define MAXIMO 2000000000

using namespace std;

/*
Ejemplo de entrada de archivo (grafo)
4
0 12 10 15
10 0 10 -5
15 -4 0 11
20 10 7 0

*/


/*********** Ficheros **********/

int *leer_fichero(string filename, int *N) {
	ifstream fichero(filename);
	fichero >> *N;
	
	int *mat;
	mat = (int *) malloc((*N) * (*N) * sizeof(int));
	
	for (int i = 0; i < (*N); i++){
		for (int j = 0; j < (*N); j++) {
			fichero >> mat[i*(*N) + j];
			//printf("%d ",mat[i*(*N) + j]);
		}
	}
	return mat;
}

void guardar_fichero(int *matriz, int n) {
	ofstream fichero("output.txt", ofstream::out);
	fichero << n << '\n';
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			fichero << matriz[i * n + j] << ' ';
		}
		fichero << '\n';
	}
}

/**************  Impresión de datos **************/

void imprimir_matriz(int *matriz, int n){
	printf("\n%d\n",n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d\t",matriz[i * n + j]);
		}
		printf("\n");
	}
}

void imprimir_vertices_distancias(int *vertices, int *distancias, int root, int n){
	printf("\nNodo raiz: %d",root);
	printf("\nVertices\tCercano\tDistancia\n");
	for (int i = 0; i < n; i++) {
		printf("%d\t\t%d\t%d\n",i,vertices[i],distancias[i]);
	}
}

/************** Otras funciones ***************/


int comparar_vectores(int *v_1, int *v_2, int n){
	
	int son_iguales=1;
	
	for (int i = 0; i < n; i++) {
		if(v_1[i]!=v_2[i]){
			printf("\nv1 es diferente de v2\n");
			son_iguales=0;
			break;
		}
	}
	printf("\nv1 es igual a v2\n");
	return son_iguales;
}

int *generar_grafo_random(int n,int maximo_peso,int minimo_peso) {
	srand (time(NULL));
	int *mat;
	mat = (int *) malloc(n * n * sizeof(int));
	
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++) {
			if(i==j){
				mat[i*n + j]=0;
			}else{
				if(rand()%10<8){	// 0 1 2 3 4 5 6 7 | 8 9 //probabilidad de vacio
					mat[i*n + j]=rand()%(maximo_peso - minimo_peso + 1) + minimo_peso;
				}
				else{
					mat[i*n + j]=MAXIMO;
				}
			}
			
		}
	}
	return mat;
}

/************* algoritmo *************/

void bellman_ford_serial(int n, int root, int *matriz, int *distancia, int *cercano, int *flag_ciclo_negativo) {

	//INICIALIZACION
    *flag_ciclo_negativo = 0;		//0 igual falso
    for (int i = 0; i < n; i++) {
        distancia[i] = MAXIMO;			//se inicia en infinito
    }	
    distancia[root] = 0;			//distancia de la raíz
	cercano[root]=root;
	

    //RELAJACION
    for (int i = 0; i < n - 1; i++) {// n - 1 iteration
        for (int u = 0; u < n; u++) {
            for (int v = 0; v < n; v++) {
                int peso = matriz[u * n + v];
                if (peso < MAXIMO && distancia[u] + peso < distancia[v]) {
                    distancia[v] = distancia[u] + peso;
					cercano[v]=u;
                }
            }
        }
    }

    //VERIFICACION DE CICLOS NEGATIVOS
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            int peso = matriz[u * n + v];
            if (peso < MAXIMO && distancia[u] + peso < distancia[v]) {
				*flag_ciclo_negativo = 1;
				return;
            }
        }
    }
}


void bellman_ford_paralelo(int n, int root, int *matriz, int *distancia, int *cercano, int *flag_ciclo_negativo, int p) {

	omp_set_num_threads(p);		//define la cantidad de procesos

	//INICIALIZACION
    *flag_ciclo_negativo = 0;		//0 igual falso
	
	#pragma omp parallel for
    for (int i = 0; i < n; i++) {
        distancia[i] = MAXIMO;			//se inicia en infinito
    }	
    distancia[root] = 0;			//distancia de la raíz
	cercano[root]=root;

    //RELAJACION
	
    int inicio_rango[p], final_rango[p];
    int promedio = n / p;
	#pragma omp parallel for
    for (int i = 0; i < p; i++) {
        inicio_rango[i] = promedio * i;
        final_rango[i] = promedio * (i + 1);
        if (i == p - 1) {
            final_rango[i] = n;
        }
    }	
	
	#pragma omp parallel
	{
		int rango = omp_get_thread_num();
		for (int i = 0; i < n - 1; i++) {// n - 1 iteration
			for (int u = 0; u < n; u++) {
				for (int v = inicio_rango[rango]; v < final_rango[rango]; v++) {
					int peso = matriz[u * n + v];
					if (peso < MAXIMO && distancia[u] + peso < distancia[v]) {
						int dista_nueva = distancia[u] + peso;
						distancia[v] = dista_nueva;
						cercano[v]=u;
					}
				}
			}
		}
	}

    //VERIFICACION DE CICLOS NEGATIVOS
    for (int u = 0; u < n; u++) {
		//#pragma omp parallel for
        for (int v = 0; v < n; v++) {
            int peso = matriz[u * n + v];
            if (peso < MAXIMO && distancia[u] + peso < distancia[v]) {
				//#pragma omp critical
				*flag_ciclo_negativo = 1;
				return;
            }
        }
    }
}


int main(int argc, char **argv){
	int n; //cantidad de nodos
	int *matriz; //matriz que contiene los pesos con direccion
	int *distancia,*distancia_p; //tamaño final que devuelve el algoritmo 
	int *vertice_cercano,*vertice_cercano_p;
	int flag_ciclo_negativo,flag_ciclo_negativo_p; //resultado del algoritmo Bellman-Ford
	
	////
	int flag;
	printf("\nCargar archivo (0) o generar aleatorio (1): ");
	scanf("%d",&flag);
	
	
	if(flag==0){
		char filename[100];
		printf("\nIngrese el nombre: ");
		scanf("%s", filename);
		matriz = leer_fichero(filename,&n);
	}
	else{
		int maximo_peso=100, minimo_peso=-6;
		printf("\nIngrese cantidad de vertices: ");
		scanf("%d", &n);
		printf("\nIngrese peso maximo: ");
		scanf("%d", &maximo_peso);
		printf("\nIngrese peso minimo: ");
		scanf("%d", &minimo_peso);
		
		matriz = generar_grafo_random(n,maximo_peso,minimo_peso);
	}
	
	//designa el tamaño serial y paralelo
	distancia = (int *) malloc(sizeof(int) * n);
	distancia_p = (int *) malloc(sizeof(int) * n);
	vertice_cercano = (int *) malloc(sizeof(int) * n);
	vertice_cercano_p = (int *) malloc(sizeof(int) * n);
	
	//se imprime la matriz
	printf("\nImprimir matriz? (0): ");
	scanf("%d",&flag);
	if(flag==0){
		imprimir_matriz(matriz,n);
	}
	
	//cantidad de procesos
	int procesos;
	printf("\nCantidad de procesos: ");
	scanf("%d",&procesos);

	
	//define la raiz
	int raiz;
	printf("Ingrese la raiz: ");
	scanf("%d", &raiz);
	
	
	//se aplica el algoritmo serial
	double start_s, end_s;
	start_s = omp_get_wtime();
	bellman_ford_serial(n, raiz, matriz, distancia, vertice_cercano, &flag_ciclo_negativo);
	end_s = omp_get_wtime();
	
	double time_s =(double) (end_s - start_s);
	printf("Tiempo - Bellman-Ford Serial: %f\n",time_s);
	
	
	//se aplica el algoritmo parelelo
	double start_p, end_p;
	start_p = omp_get_wtime();
	bellman_ford_paralelo(n, raiz, matriz, distancia_p, vertice_cercano_p, &flag_ciclo_negativo_p, procesos);
	end_p = omp_get_wtime();
	
	double time_p = (double) (end_p - start_p);
	printf("Tiempo - Bellman-Ford Paralelo: %f\n",time_p);
	
	
	comparar_vectores(distancia,distancia_p,n);
	
	double S = time_s/time_p;
	double E = S/(procesos);
	
	printf("Aceleración S: %f \nEficiencia E: %f\n\n",S,E);
	
	printf("\nVer caminos? (0): ");
	scanf("%d",&flag);
	//se imprime si no tiene ciclos negativos
	if(flag==0){
		if(flag_ciclo_negativo==0){
			imprimir_vertices_distancias(vertice_cercano,distancia,raiz,n);
			guardar_fichero(matriz,n);
		}
		else{
			printf("Existe al menos un ciclo negativo\n");
		}
	}
	
	return 0;
}
