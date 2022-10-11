/*---------------------------------------
UNIVERSIDAD DEL VALLE DE GUATEMALA
CC3056 - Programacion de Microprocesadores
Realizado por: Angel Castellanos, Javier Aljenadro Ramirez, Jose Pablo Santisteban, Mario Cristales
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

//Estructura
struct tallerSanta 
{
    //Hilos
    int elfos = 0;  //Elfos = hilos

    //Hilos por subrutina
    int hilosFunMateriaPrimas = 0;
    int hilosProduccion = 0;
    int hilosEmbalaje = 0;
    int hilosDistribucion = 0;

    //Juguetes
    int juguetesPlastico = 0;
    int juguetesMadera = 0;
    int juguetesMetal = 0;

    //Materia Prima
    int materiaPlasticoTotal = 0;
    int materiaPlastico = 0;
    int materiaMaderaTotal = 0;
    int materiaMadera = 0;
    int materiaMetalTotal = 0;
    int materiaMetal = 0;
    int materiaPlasticoProducida = 0;
    int materiaMaderaProducida = 0;
    int materiaMetalProducida = 0;

    //Fabricacion
    int juguetesFabricadosPlastico = 0;
    int juguetesFabricadosMadera = 0;
    int juguetesFabricadosMetal = 0;

    //Embalaje
    int juguetesEmbaladosPlastico = 0;
    int juguetesEmbaladosMadera = 0;
    int juguetesEmbaladosMetal = 0;

    //Distribucion
    int cantCamiones = 0;
    int cantJuguetes = 0;
};


//inicialización del mutex
pthread_mutex_t candado = PTHREAD_MUTEX_INITIALIZER;
bool exactpartition = true;

//Semaforos
sem_t semmateriaPrima;
sem_t semJuguetesProducidos;
sem_t semJuguetesEmbalados;



//40% de los hilos creados
//cada juguete costara 5 unidades de materia prima
void* funMateriaPrima (void* arg){
    
    tallerSanta *taller;
    taller = (tallerSanta*) arg;
    int id = pthread_self() - 2;

    cout<<"---------------------------------------------------"<< endl;
    cout << "El sector " << id << " de Elfos empezo la produccion de materia prima" << endl;
    cout<<"---------------------------------------------------"<< endl;
    

    //Se define que cantidad de hilos debe trabajar cada material
    int hilos = taller->hilosFunMateriaPrimas/3; 

    if(id < hilos){ //se tabaja para los hilos desde 0 hasta hilos
        while(taller->materiaPlasticoProducida < taller->materiaPlasticoTotal){ //mientras la materia prima de plastico sea menor a la total se seguira aumentando la materia prima
        //se utiliza una variable aparte de la que se toma el material para no generar un bucle infinito de produccion
        
            usleep(100000);
        
            //bloqueo de semaforo
            sem_wait(&semmateriaPrima);
            taller->materiaPlastico = taller->materiaPlastico + 1; //se aumenta la cantidad de materia prima
            taller->materiaPlasticoProducida = taller->materiaPlasticoProducida + 1;
            
            //desbloqueo de semaforo
            sem_post(&semmateriaPrima);
        }
        
    }
    else if(id >= hilos && id < hilos*2){ //se trabaja para los hilos desde hilos hasta hilos*2
        while(taller->materiaMaderaProducida < taller->materiaMaderaTotal){
        
            usleep(100000);
        
            //bloqueo de semaforo
            sem_wait(&semmateriaPrima);
            taller->materiaMadera = taller->materiaMadera +1; //se aumenta la cantidad de materia prima
            taller->materiaMaderaProducida = taller->materiaMaderaProducida + 1;
            
            //desbloqueo de semaforo
            sem_post(&semmateriaPrima);
        }
    }
    else if(id >= hilos*2){ //se trabaja para los hilos desde hilos*2 hasta el total de hilos para materia prima

        while(taller->materiaMetalProducida < taller->materiaMetalTotal){
        
            usleep(100000);
        
            //bloqueo de semaforo
            sem_wait(&semmateriaPrima);
            taller->materiaMetal = taller->materiaMetal + 1; //se aumenta la cantidad de materia
            taller->materiaMetalProducida = taller->materiaMetalProducida + 1;
            
            //desbloqueo de semaforo
            sem_post(&semmateriaPrima);
        }
    }
    


    return 0;
}

//30% de los hilos creados
void* produccion (void* arg){

    tallerSanta *taller;
    taller = (tallerSanta*) arg;
    int id = pthread_self() - 2;

    cout<<"---------------------------------------------------"<< endl;
    cout << "El sector "<< id <<" de Elfos empezo la produccion de Juguetes" << endl;
    cout<<"---------------------------------------------------"<< endl;

    int limiteinferior = taller->hilosFunMateriaPrimas;
    int limiteSuperior = taller->hilosProduccion + limiteinferior;
    int hilos = taller->hilosProduccion/3; //Se define que cantidad de hilos debe trabajar cada material

    
    if(limiteinferior <= id && id < limiteinferior + hilos){

        while(taller->materiaPlastico >= 5 ){
            
            //duerme el hilo por 1 segundos
            usleep(100000);

            //bloqueo de semaforo materia prima
            sem_wait(&semmateriaPrima);

            //resta de materia prima
            taller->materiaPlastico = taller->materiaPlastico-5;

            //desbloqueo de semaforo materia prima
            sem_post(&semmateriaPrima);


            //bloqueo de semaforo juguetes Producidos
            sem_wait(&semJuguetesProducidos);

            //creación del juguete
            taller->juguetesFabricadosPlastico ++;

            //desbloqueo de semaforo juguetes Producidos
            sem_post(&semJuguetesProducidos);         
            
        }        
    }
    else if(limiteinferior + hilos <= id && id < limiteinferior + hilos*2){

        while(taller->materiaMadera >= 5 ){
            
            //duerme el hilo por 1 segundos
            usleep(100000);

            //bloqueo de semaforo materia prima
            sem_wait(&semmateriaPrima);

            //resta de materia prima
            taller->materiaMadera = taller->materiaMadera-5;

            //desbloqueo de semaforo materia prima
            sem_post(&semmateriaPrima);


            //bloqueo de semaforo juguetes Producidos
            sem_wait(&semJuguetesProducidos);

            //creación del juguete
            taller->juguetesFabricadosMadera ++;

            //desbloqueo de semaforo juguetes Producidos
            sem_post(&semJuguetesProducidos);         
            
        }
    }
    else if(limiteinferior + hilos*2 <= id && id < limiteSuperior){

        while(taller->materiaMetal >= 5 ){
            
            //duerme el hilo por 1 segundos
            usleep(100000);

            //bloqueo de semaforo materia prima
            sem_wait(&semmateriaPrima);

            //resta de materia prima
            taller->materiaMetal = taller->materiaMetal-5;

            //desbloqueo de semaforo materia prima
            sem_post(&semmateriaPrima);


            //bloqueo de semaforo juguetes Producidos
            sem_wait(&semJuguetesProducidos);

            //creación del juguete
            taller->juguetesFabricadosMetal ++;

            //desbloqueo de semaforo juguetes Producidos
            sem_post(&semJuguetesProducidos);         
            
        }
    }
    return 0;
}

//20% de los hilos creados
void* embalaje (void* arg){

    tallerSanta *taller;
    taller = (tallerSanta*) arg;

    int id = pthread_self() - 2;

    cout << "---------------------------------------------------"<< endl;
    cout <<"El sector "<< id << " de Elfos empezo el embalaje de Juguetes" << endl;
    cout << "---------------------------------------------------"<< endl;


    int limiteinferior = taller->hilosFunMateriaPrimas + taller->hilosProduccion;
    int limiteSuperior = taller->hilosEmbalaje + limiteinferior;
    int hilos = taller->hilosEmbalaje/3; //Se define que cantidad de hilos debe trabajar cada material

    /*
    if(limiteinferior <= id && id < limiteinferior + hilos){
        while(){
        
            
        }
        
    }
    else if(limiteinferior + hilos <= id && id < limiteinferior + hilos*2){
        while(){
        

        }
    }
    else if(limiteinferior + hilos*2 <= id && id < limiteSuperior){

        while(){
        
            
        }
    }*/

    /*
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

    }*/

    return 0;
    
    
}

//10% de los hilos creados
void* distribucion (void* arg){

    tallerSanta *taller;
    taller = (tallerSanta*) arg;
    int id = pthread_self() - 2;

    //Importar variables de la struct
    int cantCamiones;
    int cajasPorCamion = 10;
    int cantJuguetesTotal = taller->juguetesEmbaladosPlastico + taller->juguetesEmbaladosMadera + taller->juguetesEmbaladosMetal;
    int cantJuguetes = taller->juguetesEmbaladosPlastico + taller->juguetesEmbaladosMadera + taller->juguetesEmbaladosMetal;

    cout << "---------------------------------------------------"<< endl;
    cout << "El sector " << id << " de Elfos empezo a distribuir los juguetes" << endl << "--- A alegrar la Navidad de muchos ninos! ---" << endl;
    cout << "---------------------------------------------------"<< endl;

    //se distribuiran los juguetes mientras la cantidad de juguetes empacados sea mayor a 0 y dependiendo de la cantidad de juguetes que se pidieron
    
    //bloqueo mutex
    pthread_mutex_lock(&candado);
    
    if (cantJuguetes >= 10) {

        //duerme el hilo por 0.1 segundo
        usleep(100000);
        cantCamiones = cantJuguetes/cajasPorCamion;
        cantJuguetes = cantJuguetes - cantCamiones*cajasPorCamion;

        cout << cantCamiones << " camiones han sido distribuidos para entregar " << cantJuguetesTotal << " juguetes!" << endl;
        taller->cantJuguetes = cantJuguetes;

    } else {
        usleep(100000);
        cantCamiones = 0;
        cout << "No hay suficientes juguetes para distribuir" << endl;
    }
    
    //desbloqueo mutex
    pthread_mutex_unlock(&candado);

    return 0;
}


int main(){
    
    tallerSanta taller;
    
    cout<<"---------------------------------------------------"<< endl;
    cout << "Bienvenido a Santa's Factory S.A" << endl;
    cout << "Ingrese la cantidad de juguetes que desea: " << endl;
    cout<<"---------------------------------------------------"<< endl;
    cout << "Jueguetes de plastico: " << endl;
    cout<<"---------------------------------------------------"<< endl;
    cin >> taller.juguetesPlastico;
    cout << "Jueguetes de madera: " << endl;
    cout<<"---------------------------------------------------"<< endl;
    cin >> taller.juguetesMadera;
    cout << "Jueguetes de metal: " << endl;
    cout<<"---------------------------------------------------"<< endl;
    cin >> taller.juguetesMetal;

    int cantJuguetes = taller.juguetesPlastico + taller.juguetesMadera + taller.juguetesMetal;
    
    taller.cantJuguetes = cantJuguetes;

    taller.elfos = cantJuguetes/5;
    pthread_t threads[taller.elfos];
    
    taller.materiaPlasticoTotal = taller.juguetesPlastico*5;
    taller.materiaMaderaTotal = taller.juguetesMadera*5;
    taller.materiaMetalTotal = taller.juguetesMetal*5;

    taller.hilosFunMateriaPrimas = taller.elfos * 0.4;
    taller.hilosProduccion = taller.elfos * 0.3;
    taller.hilosEmbalaje = taller.elfos * 0.2;
    taller.hilosDistribucion = taller.elfos * 0.1;

    sem_init(&semmateriaPrima, 0, 1);
    sem_init(&semJuguetesProducidos, 0, 1);
    sem_init(&semJuguetesEmbalados, 0, 1);

    //creacion de hilos
    for (int i = 0; i < taller.hilosFunMateriaPrimas; i++){
        pthread_create(&threads[i], NULL, funMateriaPrima, (void*)&taller);
        usleep(1000); 
    }
    
    //sleep(3);

    for (int i = taller.hilosFunMateriaPrimas; i < taller.hilosFunMateriaPrimas + taller.hilosProduccion; i++){
        pthread_create(&threads[i], NULL, produccion, (void*)&taller);
        usleep(1000);
    }

    sleep(3);

    for (int i = taller.hilosFunMateriaPrimas + taller.hilosProduccion; i < taller.hilosFunMateriaPrimas + taller.hilosProduccion + taller.hilosEmbalaje; i++){
        pthread_create(&threads[i], NULL, embalaje, (void*)&taller);
        usleep(1000);
    }

    //sleep(3);
    
    for (int i = taller.hilosFunMateriaPrimas + taller.hilosProduccion + taller.hilosEmbalaje; i < taller.hilosFunMateriaPrimas + taller.hilosProduccion + taller.hilosEmbalaje + taller.hilosDistribucion; i++){
        pthread_create(&threads[i], NULL, distribucion, (void*)&taller);
        usleep(1000);
    }

    //sleep(3);

    //espera a que los hilos terminen
    for (int i = 0; i < taller.elfos; i++){
        pthread_join(threads[i], NULL);
    }

    cout << "FINISHED" << endl;
    
    return 0;
}
