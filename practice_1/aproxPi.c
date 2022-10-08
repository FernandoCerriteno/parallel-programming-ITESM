#include <stdio.h>      //Libreria para trabajar en c
#include <pthread.h>    //Libreria para trabajar con threads
#include <stdlib.h>     //Libreria para usar numeros aleatorios
#include <time.h>       //Libreria para generar semillas y que los numeros aleatorios no sean los mismos
#include <math.h>       //Libreria para hacer calculos matematicos

#define NUM_THREADS 10  //El numero de hilos con los que se trabajara
#define NUM_POINTS 100000//El numero de puntos que se usaran en MonteCarlo

unsigned int seed;
long pointPerThread;
long pointsCircle;

//      Creacion de un numero aleatorio entre 0 y 1
double randomNum(){
        return (double)rand_r(&seed) / (double)((unsigned)RAND_MAX + 1);
}

//      Funcion que se le pasa a los threads
void *monteCarlo(void *threadid){
        double x, y;
        int *count_in = (int *)malloc(sizeof(int));
        *count_in = 0;

        int i;
        for(i=0;i<pointPerThread;i++){
                x = randomNum();
                y = randomNum();
                if(sqrt( pow(( x - 0.5), 2) + pow((y - 0.5), 2)) <= 0.5){
                        *count_in += 1;
                }
        }
        pthread_exit((void *) count_in);
}

int main(int argc, char *argv[]){
        clock_t t;
        t = clock();
        pthread_t threads[NUM_THREADS];
        int points = NUM_POINTS, num_threads = NUM_THREADS, rc;
        double pi;
        long i;
        void *count_inside_thread;

        seed = time(NULL);      //Semilla para el numero aleatorio
        pointPerThread = points/num_threads;
        printf("\n--------------------------------------------------------\n");
        printf("Number of threads created = %d\nNumber of points = %d\nPoints per thread = %ld\n",num_threads,points,pointPerThread);

        for(i = 0; i<num_threads ; i++){
                rc = pthread_create(&threads[i],NULL,monteCarlo,(void *)i);
                if (rc){
                        printf("ERROR; return code from pthread_create() is %d\n", rc);
                        exit(-1);
                }
        }

        for(i = 0; i < num_threads; i++){
                pthread_join(threads[i], &count_inside_thread);
                pointsCircle += *(long *) count_inside_thread;
        }

        pi = (4.0 * (float)pointsCircle) / (float)points;
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;

        printf("Number of points inside the Circle - %ld\n",pointsCircle);
        printf("--------------------------------------------------------\n");
        printf("\nThe value of pi is ≈ %f\n\n",pi);
        printf("The program took %f seconds to execute\n\n",time_taken);
        pthread_exit(NULL);
        return 0;
}
