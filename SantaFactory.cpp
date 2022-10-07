/*---------------------------------------
UNIVERSIDAD DEL VALLE DE GUATEMALA
CC3056 - Programacion de Microprocesadores
Realizado por: Angel Castellanos, Javier Aljenadro Ramirez, Jose Pablo Santisteban
Proyecto 02 - Santa's Factory // Laboratorio 5
---------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <chrono>
#include <semaphore.h>
#include <cmath>
using namespace std;

//Variables globales    
int materiaPrima = 0;
int juguetesProducidos = 0;
int productosEmpacados = 0;
int productosNoEmpacados = 0;
bool disponibilidad = false;
int cantJuguetes = 0;
int materiaAproducir = 0;

//inicialización del mutex
pthread_mutex_t candado = PTHREAD_MUTEX_INITIALIZER;
bool exactpartition = true;

sem_t semmateriaPrima;
sem_t semJuguetesProducidos;
sem_t semJuguetesEmbalados;



//40% de los hilos creados
//cada juguete costara 5 unidades de materia prima
void* funMateriaPrima (void* arg){

    cout<<"---------------------------------------------------"<< endl;
    cout << "Se empezo la produccion de materia prima" << endl;
    cout<<"---------------------------------------------------"<< endl;

    while(materiaPrima < materiaAproducir){
        
        sleep(2);
    
        //bloqueo de semaforo
        sem_wait(&semmateriaPrima);
        materiaPrima++;
        
        //desbloqueo de semaforo
        sem_post(&semmateriaPrima);
    }

    return 0;
}

//30% de los hilos creados
void* produccion (void* arg){

    cout<<"---------------------------------------------------"<< endl;
    cout << "Se empezo la produccion de Juguetes" << endl;
    cout<<"---------------------------------------------------"<< endl;
    
    //verifica la cantidad minima de materia prima creada para empezara producir juguetes
    while(materiaPrima >=5){

        //duerme el hilo por 3 segundos
        sleep(3);

        //bloqueo de semaforo
        sem_wait(&semmateriaPrima);
        // Se resta la maetria prima utilizada
        materiaPrima = materiaPrima - 5;
        
        //desbloqueo de semaforo
        sem_post(&semmateriaPrima);

        //bloqueo de semaforo
        sem_wait(&semJuguetesProducidos);
        
        // Crea un juguete
        juguetesProducidos ++;

        //desbloqueo de semaforo
        sem_post(&semJuguetesEmbalados);

        

    }

    return 0;
}

//20% de los hilos creados
void* embalaje (void* arg){
    cout << "---------------------------------------------------"<< endl;
    cout << "Se empezo el embalaje de Juguetes" << endl;
    cout << "---------------------------------------------------"<< endl;

    
    //se ejecuta mientras la cantidad de juguetes producidos sea mayor a 10
    while(juguetesProducidos >= 10){
        
        //duerme el hilo por 2 segundos
        sleep(2);
        
        //hace un lote de empaque de 10 juguetes 

        //bloqueo de semaforo
        sem_wait(&semJuguetesEmbalados);
        
        productosEmpacados = productosEmpacados + floor(juguetesProducidos/10);
        juguetesProducidos = juguetesProducidos - floor(juguetesProducidos/10)*10;

        //desbloqueo de semaforo
        sem_post(&semmateriaPrima);

    }

    return 0;
    
    
}

//10% de los hilos creados
void* distribucion (void* arg){
    
    cout << "---------------------------------------------------"<< endl;
    cout << "Distribucion de juguetes iniciada, a alegrar la Navidad de muchos ninos!" << endl;
    cout << "---------------------------------------------------"<< endl;

    //se distribuiran los juguetes mientras la cantidad de juguetes emp

}


int main(){
    
    cout<<"---------------------------------------------------"<< endl;
    cout << "Bienvenido a Santa's Factory S.A" << endl;
    cout << "Ingrese la cantidad de juguetes que desea: " << endl;
    cout<<"---------------------------------------------------"<< endl;
    cin >> cantJuguetes;

    int n = cantJuguetes;
    pthread_t threads[n];

    int hilosFunMateriaPrima = n * 0.4;
    int hilosProduccion = n * 0.3;
    int hilosEmbalaje = n * 0.2;
    int hilosDistribucion = n * 0.1;
    materiaAproducir = cantJuguetes * 5;

    sem_init(&semmateriaPrima, 0, 1);
    sem_init(&semJuguetesProducidos, 0, 1);
    sem_init(&semJuguetesEmbalados, 0, 1);

    //creacion de hilos
    for (int i = 0; i < hilosFunMateriaPrima; i++){
        pthread_create(&threads[i], NULL, funMateriaPrima, NULL);
        usleep(100);  
    }
    
    usleep(10);

    for (int i = hilosFunMateriaPrima; i < hilosFunMateriaPrima + hilosProduccion; i++){
        pthread_create(&threads[i], NULL, produccion, NULL);
        usleep(100);
    }

    usleep(10);

    for (int i = hilosFunMateriaPrima + hilosProduccion; i < hilosFunMateriaPrima + hilosProduccion + hilosEmbalaje; i++){
        pthread_create(&threads[i], NULL, embalaje, NULL);
        usleep(100);
    }

    usleep(10);
    
    for (int i = hilosFunMateriaPrima + hilosProduccion + hilosEmbalaje; i < hilosFunMateriaPrima + hilosProduccion + hilosEmbalaje + hilosDistribucion; i++){
        pthread_create(&threads[i], NULL, distribucion, NULL);
        usleep(100);
    }

    usleep(10);

    //espera a que los hilos terminen
    for (int i = 0; i < n; i++){
        pthread_join(threads[i], NULL);
    }
    
    
    return 0;
}
