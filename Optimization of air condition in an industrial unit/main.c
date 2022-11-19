#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <omp.h>

int R = 6;
int C = 7;
int matOg[6][7]={
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,1,1,1,0,0},
            {0,0,1,1,1,0,0},
            {0,0,1,1,1,0,0},
            {0,0,0,0,0,0,0}
            };
int matCalor[6][7];
pthread_mutex_t lock;

int analyzeMatrix(){
  for(int i = 0; i<R ; i++){
    for(int j = 0; j<C ; j++){
      if(matCalor[i][j] == 0)
        return 0;
    }
  }
  return 1;
}

int randomValue(int n,int m){
  return m + rand()%n;
}

int randomMovement(int n){
  int m = (-1) + rand() % (2 - (-1));
  n += m;
  return n;
}

void *setTemp(void *t_id){
  srand(time(NULL));
  int x,y;
  x = randomValue(R,0);
  y = randomValue(C,0);
  int check = 0;
  while(check == 0){
    if(t_id == 0){
      check = analyzeMatrix();
    }

    pthread_mutex_lock(&lock);
    if(matOg[x][y] == 0){
      matCalor[x][y] = randomValue(61,0);
    }else{
      matCalor[x][y] = randomValue((101-61),61);
    }
    pthread_mutex_unlock(&lock);
    
    x = randomMovement(x);
    y = randomMovement(y);
    
    if(y == -1)
      y++;
    else if(y == C){
      y--;
    }
    if(x == -1)
      x++;
    else if(x == R){
      x--;
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  //Number of threads
  printf("\nMatriz de Temperatura:\n\n");
  int nThreads;
  if (argc>1){
      nThreads = atoi(argv[1]);
  }else{
    nThreads = 4;
  }
  omp_set_num_threads(nThreads);
  
//Print Matrix
  for(int i=0; i< R ;i++ ){
      for(int j=0; j< C ;j++ ){
          matCalor[i][j] = 0;
          if(matOg[i][j] == 0){
              printf("C ");
          }
          else{
              printf("H ");
          }
      }
      printf("\n");
    }
  printf("\n");
  int     rc;
  pthread_t threads[nThreads];
  long int i;

  
  for(i=0; i<nThreads; i++){
    printf("Thread %d\n", i);
      rc = pthread_create(&threads[i], NULL, setTemp, (void*) i);
      if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
      }
  }

    //Print Temperature Matrix
  printf("\nMatriz con valores:\n\n");
  for(int row = 0; row < R; row++){
    for(int col = 0; col < C; col++){
      printf("%d ", matCalor[row][col]);
    }
    printf("\n");
  }

  //Calculo de la ecuación de calor
  int max_iter_time = 10,  alpha = 2, delta_x = 5;

  //Parametros
  float delta_t = (pow(delta_x,2)) / (4 * alpha);
  float gamma = (alpha * delta_t) / (pow(delta_x,2));

  //Plot
  int u[max_iter_time][R][C];
  
  for(int i = 0; i < max_iter_time; i++){
    for(int j = 0; j < R; j++){
      for(int k = 0; k < C; k++){
        u[i][j][k] = matCalor[j][k];
      }
    }
  }

  
 int k, l, j;
 #pragma omp parallel for private(k, i, j) shared (u)
  for(k = 0; k < max_iter_time - 1; k++){
    for(l = 1; i < R - 1; l+= delta_x){
      for(j = 1; j < C - 1; j+= delta_x){
        u[k + 1][l][j] = gamma * (u[k][l+1][j] + u[k][l-1][j] + u[k][l][j+1] + u[k][l][j-1] - 4*u[k][l][j]) + u[k][l][j];      
      }
    }
  }
  
  printf("\nDear manager \nThis is the heat equation for the factory: \n  y = %d * (%f/(%d)^2) \n",alpha,delta_t,delta_x);

  float average = 0;
  
  printf("\nMatriz de temperatura despues de 10 unidades de tiempo\n\n");
  for(int j = 0; j < R; j++){
    for(int k = 0; k < C; k++){
      printf("%d ", u[max_iter_time-1][j][k]);
      average = average + u[max_iter_time-1][j][k];
    }
    printf("\n");
  }
  printf("\n");

  average = average/(R*C);
  printf("The average temperature in the factory is %f \nThanks\n",average);
  
  return 0;
}
