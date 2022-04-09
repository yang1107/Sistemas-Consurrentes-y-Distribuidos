#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std;
// ----------------------------------------------------------------------------
int material, veces=20;
sem_t puede_producir,fuma1,fuma2,fuma3;

// función que simula la acción de fumar  como un retardo aleatorio de la hebra

void  fumar() 
{ 
   //  inicializa la semilla aleatoria  (solo la primera vez)
   static bool primera_vez = true ;
   if ( primera_vez )
   {   primera_vez = false ;
      srand( time(NULL) );    
   }
   
   // calcular un numero aleatorio de milisegundos (entre 1/10 y 2 segundos)
   const unsigned miliseg = 100U + (rand() % 1900U) ; 

   // retraso bloqueado durante 'miliseg' milisegundos
   usleep( 1000U*miliseg ); 
}
// ----------------------------------------------------------------------------

// falta: resto de funciones 
// ..............

void * estanquero(void *){
	
	for(int i=0;i<veces;i++){
		sem_wait(&puede_producir);
		material=rand()%4+1;  //1+rand()%(4-1) 
		if(material==1){
			cout<<"Se ha generado tabaco"<<endl;
			sem_post(&fuma1);
		}
		else{
			if(material==2){
				cout<<"Se ha generado papel"<<endl;
				sem_post(&fuma2);
			}
			else{
				cout<<"Se ha generado cerilla"<<endl;
				sem_post(&fuma3);
			}	
		}	
	}
	return NULL;
}





void * fumadoruno(void *){
	for(int i=0;i<veces;i++){
		cout<<"El fumador 1 esta esperando al tabaco"<<endl;
		sem_wait(&fuma1);
		cout<<"El fumador 1 esta fumando con el tabaco generado"<<endl;
		sem_post(&puede_producir);
		fumar();
		cout<<"El fumador 1 ha terminado de fumar"<<endl;
	}
	return NULL;
}

void * fumadordos(void *){
	for(int i=0;i<veces;i++){
		cout<<"El fumador 2 esta esperando al papel"<<endl;
		sem_wait(&fuma2);
		cout<<"El fumador 2 esta fumando con el papel generado"<<endl;
		sem_post(&puede_producir);
		fumar();
		cout<<"El fumador 2 ha terminado de fumar"<<endl;
	}
	return NULL;
}

void * fumadortres(void *){
	for(int i=0;i<veces;i++){
		cout<<"El fumador 3 esta esperando a la cerilla"<<endl;
		sem_wait(&fuma3);
		cout<<"El fumador 3 esta fumando con la cerilla generado"<<endl;
		sem_post(&puede_producir);
		fumar();
		cout<<"El fumador 3 ha terminado de fumar"<<endl;
	}
	return NULL;
}


// ----------------------------------------------------------------------------

int main()
{
  // falta: creación hebras ....
  sem_init(&puede_producir,0,1);
  sem_init(&fuma1,0,0);
  sem_init(&fuma2,0,0);
  sem_init(&fuma3,0,0);
  
  pthread_t h_estanquero,h_fumador1,h_fumador2,h_fumador3;
  
  pthread_create(&h_estanquero,NULL,estanquero,NULL);
  pthread_create(&h_fumador1,NULL,fumadoruno,NULL);
  pthread_create(&h_fumador2,NULL,fumadordos,NULL);
  pthread_create(&h_fumador3,NULL,fumadortres,NULL);
  
  pthread_join(h_estanquero,NULL);
  pthread_join(h_fumador1,NULL);
  pthread_join(h_fumador2,NULL);
  pthread_join(h_fumador3,NULL);
  
  sem_destroy(&puede_producir);
  sem_destroy(&fuma1);
  sem_destroy(&fuma2);
  sem_destroy(&fuma3);
  
   pthread_exit(NULL);
   
  return 0 ;
}
