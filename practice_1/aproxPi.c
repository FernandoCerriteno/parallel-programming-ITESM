/*      07/10/2022
        Fernando Cerriteño Magaña - A01702790
        Para cambiar el numero de threads, cambiar el valor que se encuentra en la 
        variable NUM_THREADS
        Para la realizacion de este codigo se uso como referencia el codigo de 
        Cesar Augusto Garcia Perez - https://github.com/VictorRodriguez/operating-systems-lecture/blob/master/labs/04/pi.c
*/

#include <stdio.h>      //Libreria para trabajar en c
#include <pthread.h>    //Libreria para trabajar con threads
#include <stdlib.h>     //Libreria para usar numeros aleatorios
#include <time.h>       //Libreria para generar semillas y que los numeros aleatorios no sean los mismos
#include <math.h>       //Libreria para hacer calculos matematicos

#define NUM_THREADS 10  //El numero de hilos con los que se trabajara
#define NUM_POINTS 100000//El numero de puntos que se usaran en MonteCarlo

unsigned int seed;      //Semilla que se usara en la creacion del numero aleatorio
long pointPerThread;    //Puntos que tiene que generar cada hilo
long pointsCircle;      //Puntos que quedan dentro del circulo

//      Creacion de un numero aleatorio entre 0 y 1
double randomNum(){
        return (double)rand_r(&seed) / (double)((unsigned)RAND_MAX + 1);
}

//      Funcion que se le pasa a los threads
void *monteCarlo(void *threadid){
        double x, y;                                    //Variables que generaran una coordenada aleatoria x,y
        int *count_in = (int *)malloc(sizeof(int));     //Variable para llevar conteo de los puntos dentro del circulo
        *count_in = 0;

        int i;
        for(i=0;i<pointPerThread;i++){                  //Loop for que genera las coordenadas aleatorias
                x = randomNum();
                y = randomNum();
                if(sqrt( pow(( x - 0.5), 2) + pow((y - 0.5), 2)) <= 0.5){       //Condicional que detecta si la coordenada esta adentro del circulo.
                        *count_in += 1;                 //Si las coordenadas del punto entran en el area del circulo, se le suma 1 al contador
                }
        }
        pthread_exit((void *) count_in);
}

//Funcion principal
int main(int argc, char *argv[]){
        clock_t t;                                              //Variable para llevar registro del tiempo
        t = clock();
        pthread_t threads[NUM_THREADS];                         //Creacion de los threads
        int points = NUM_POINTS, num_threads = NUM_THREADS, rc; //Asignacion del numero de puntos y del numero de hilos para hacer calculos con ellos
        double pi;                                              //Variable de tipo double en donde se guardara el valor final de pi
        long i;                                                 //Entero i para los bucles tipo for
        void *count_inside_thread;                              //Contador del numero de puntos dentro del circulo por thread

        seed = time(NULL);                                      //Semilla para el numero aleatorio
        pointPerThread = points/num_threads;                    //Puntos que cada thread tendra que generar
        printf("\n--------------------------------------------------------\n");
        printf("Number of threads created = %d\nNumber of points = %d\nPoints per thread = %ld\n",num_threads,points,pointPerThread);

        for(i = 0; i<num_threads ; i++){                        //Bucle for donde se le dan instrucciones a los threads
                rc = pthread_create(&threads[i],NULL,monteCarlo,(void *)i);     //Creacion en la variable rc para detectar si hubo errores
                if (rc){
                        printf("ERROR; return code from pthread_create() is %d\n", rc);
                        exit(-1);
                }
        }

        for(i = 0; i < num_threads; i++){                       //Una vez que el bucle finalizo, se hace un join para obtener el numero de puntos dentro del circulo
                pthread_join(threads[i], &count_inside_thread);
                pointsCircle += *(long *) count_inside_thread;
        }

        pi = (4.0 * (float)pointsCircle) / (float)points;       //Se calcula el valor de pi en base al area del cuadrado por los puntos dentro del circulo sobre el numero de puntos totales
        t = clock() - t;                                        //Se obtiene el tiempo de ejecucion total
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
        
        //Se imprimen resultados
        printf("Number of points inside the Circle - %ld\n",pointsCircle);
        printf("--------------------------------------------------------\n");
        printf("\nThe value of pi is ≈ %f\n\n",pi);
        printf("The program took %f seconds to execute\n\n",time_taken);
        pthread_exit(NULL);
        return 0;
}
