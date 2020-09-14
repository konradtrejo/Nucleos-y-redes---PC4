#include<iostream>
#include<string>
#include<queue>
#include<cstdlib>
#include<bits/stdc++.h>
#include<omp.h>
#define max 50
#define inf 1061109567

using namespace std;

class Vertice{
    
        
    private: 
        int etiqueta;
        string _etiqueta;
    public:
        void setEtiqueta(int);
        int getEtiqueta();
        Vertice(int);
        Vertice();
        Vertice *next;

};

 Vertice:: Vertice(int etiqueta){
    
        this->etiqueta = etiqueta;
        this->next = NULL;

}; 

void Vertice::setEtiqueta(int etiqueta){
    this->etiqueta =etiqueta;

};
int Vertice::getEtiqueta(){
    return etiqueta;
}

Vertice::Vertice(){
    // constructor por defecto
}
class Arista{

   public:
        Vertice *vertice;
        int valor; // costo de la Arista           
        Arista(Vertice*, int valor);
        Arista();
        Arista(int);
        int index;
};
Arista::Arista(Vertice *v, int valor){
        vertice = new Vertice();
        this->valor= valor;
        this->vertice = v;
        v->next =NULL;
};
Arista::Arista(){
    // constructor por defecto
}
Arista::Arista(int valor){
    this->valor=valor;
}


class Grafo {
    
    public:
            int cantidad_vertices;
            int cantidad_aristas;
            Arista *aristas[max];
            Vertice * vertices[max];

            void insertar_Aristas(Vertice,Vertice,Arista);
            Grafo(int, int);
            void imprimir_grafo();
            Grafo();
};
Grafo::Grafo(int cantidad_vertices, int cantidad_aristas){
    this->cantidad_vertices = cantidad_vertices;
    this->cantidad_aristas= cantidad_aristas;
     aristas[cantidad_aristas];
     vertices[cantidad_vertices];
   /*   for(int i = 0; i < (cantidad_aristas); i ++){
        this->aristas[i] = NULL;
    } */

};
Grafo::Grafo(){
    // constructor por defecto
}
void Grafo::insertar_Aristas(Vertice vertice1,Vertice vertice2, Arista arista)
{
    if(vertice1.getEtiqueta()+1>0 && vertice1.getEtiqueta()<max && vertice2.getEtiqueta()+1>0 && vertice2.getEtiqueta()<max){
        Arista *arista_ = new Arista(&vertice1,arista.valor);
        arista_->vertice =&vertice1;
        arista_->vertice->next = &vertice2;
        this->aristas[arista.index] = arista_;
        
        

    }
}
void Grafo::imprimir_grafo(){
    /* for (int i = 0; i < cantidad_vertices; i++)
    {
       cout<< "vertice < "<<i+1<<" > = "<< vertices[i]->getEtiqueta()<<endl;
    } */
    
    for(int i= 0; i< cantidad_aristas; i++){
         cout<< "Arista <"<<aristas[i]->vertice->getEtiqueta() +1<<",  "<<aristas[i]->vertice->next->getEtiqueta()+1<<" > = " << aristas[i]->valor<<endl;
    }
}
class estado_recorrido{
    private: 
            int vertice_actual;
            int distancia;


    public: 
    

};

void Algoritmo_dijkstra_paralelo(Grafo *grafo, Vertice  Vertice_inicio, Vertice vetice_fin, int distancia[], int  par[]){
    


    Arista *recorrer = new Arista(&Vertice_inicio,grafo->aristas[0]->valor);
    int  v_recorrer =0;
    int  v_contiguo =0 ;
    int costo;
    int menor_distancia;

    #pragma omp parallel 
    distancia[Vertice_inicio.getEtiqueta()] = 0;
    v_recorrer = recorrer->valor;
    v_contiguo = recorrer->vertice->getEtiqueta();

        recorrer = grafo->aristas[v_recorrer];

        while(recorrer != NULL){

            v_contiguo = recorrer->vertice->getEtiqueta();
            costo = recorrer->valor;

            if((distancia[v_recorrer] + costo) < distancia[v_contiguo]){
                distancia[v_contiguo] = distancia[v_recorrer] + costo;
                par[v_contiguo] = v_recorrer;
            }
            recorrer->vertice = recorrer->vertice->next;
        }

       
        menor_distancia = inf;
        int i;

        #pragma omp parallel for

        for(int i = 1; i < (max + 1); i ++){
            if( (distancia[i] < menor_distancia)){
                v_recorrer = i;
                menor_distancia = distancia[i];
            }
        }
    }

void Algoritmo_dijkstra_serie(Grafo *grafo, Vertice  Vertice_inicio, Vertice vetice_fin, int distancia[], int  par[]){
    


    Arista *recorrer = new Arista(&Vertice_inicio,grafo->aristas[0]->valor);
    int  v_recorrer =0;
    int  v_contiguo =0 ;
    int costo;
    int menor_distancia;

   
    distancia[Vertice_inicio.getEtiqueta()] = 0;
    v_recorrer = recorrer->valor;
    v_contiguo = recorrer->vertice->getEtiqueta();

        recorrer = grafo->aristas[v_recorrer];

        while(recorrer != NULL){

            v_contiguo = recorrer->vertice->getEtiqueta();
            costo = recorrer->valor;

            if((distancia[v_recorrer] + costo) < distancia[v_contiguo]){
                distancia[v_contiguo] = distancia[v_recorrer] + costo;
                par[v_contiguo] = v_recorrer;
            }
            recorrer->vertice = recorrer->vertice->next;
        }

       
        menor_distancia = inf;
        int i;

     

        for(int i = 1; i < (max + 1); i ++){
            if( (distancia[i] < menor_distancia)){
                v_recorrer = i;
                menor_distancia = distancia[i];
            }
        }
    }

int leer_fichero(string filename) {
	ifstream fichero(filename);
    int N;
	fichero >> N;
	
	int *mat;
	mat = (int *) malloc((N) * (N) * sizeof(int));
	int count=0;
    int a;
	for (int i = 0; i < (N); i++){
		for (int j = 0; j < (N); j++) {
            fichero >> a;
            cout<<a<<endl;
             if (a !=0 &&  a!=2000000000){
                count++;
             }
			fichero >> mat[i*(N) + j];
			//printf("%d ",mat[i*(*N) + j]);
		}
	}
	return count;
}
// funcion Principal para llamar a la funcion
int main(){
    omp_set_num_threads(6);

    int cantidad_v = 100;
    int cantidad_a= 7;
    Vertice vertices[cantidad_v];
    Arista arista[cantidad_a];
    int distancia[max];
    int par[max];
    Grafo *grafo = new Grafo(5,7);

  

    for (int i = 0; i < cantidad_v; i++)
    {
        
        vertices[i].setEtiqueta(i);

    }

    srand((unsigned) time(NULL));

    for (int i = 0; i < 7; i++)
    {
        
        arista[i].valor= rand()%12 +1;
        arista[i].index= i;
        
    }


     for (int i = 0; i < 5; i++)
    {

        
       cout<<"Vertice( "<<i+1 <<" )" << vertices[i].getEtiqueta()<<endl;

        
    }
    cout<<"------------------------------------------------------"<<endl;
    grafo->insertar_Aristas(vertices[0],vertices[1],arista[0]);
     grafo->insertar_Aristas(vertices[0],vertices[2],arista[1]);
    grafo->insertar_Aristas(vertices[2],vertices[3],arista[2]);
    grafo->insertar_Aristas(vertices[2],vertices[4],arista[3]);
    grafo->insertar_Aristas(vertices[1],vertices[2],arista[4]);
    grafo->insertar_Aristas(vertices[3],vertices[4],arista[5]);
    grafo->insertar_Aristas(vertices[1],vertices[4],arista[6]); 
    grafo->imprimir_grafo();


    
    double start_s, end_s;
	start_s = omp_get_wtime();
	Algoritmo_dijkstra_serie(grafo,vertices[0],vertices[4],distancia,par);
	end_s = omp_get_wtime();
	
	double time_s = (double) (end_s - start_s);
	printf("Tiempo en serial : %f\n",time_s);

    double start_p, end_p;
	start_p = omp_get_wtime();
	Algoritmo_dijkstra_paralelo(grafo,vertices[0],vertices[4],distancia,par);
	end_p = omp_get_wtime();
	
	double time_p = (double) (end_p - start_p);
	printf("Tiempo en Paralelo : %f\n",time_p);


}
