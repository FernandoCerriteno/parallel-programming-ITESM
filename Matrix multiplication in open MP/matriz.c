/*
 Fernando Cerriteño Magaña - A01702790
 Código que usa omp para multiplicar dos matrices de tamaño NxN

 Código de referencia:
 https://www.tutorialspoint.com/matrix-multiplication-algorithm#:~:text=The%20matrix%20multiplication%20can%20only,C%20will%20be%20(m%20x%20q)
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 10

int a[N][N];
int b[N][N];
int c[N][N];

int main(int argc, char** argv) {
        srand(time(NULL));

        int num_threads = 1;
        int i,j,k,r;

        if (argc > 1) {
                num_threads = atoi(argv[1]);
        }

        omp_set_num_threads(num_threads);
        for( i = 0; i < N;i++){
                for( j = 0; j < N; j++){
                        c[i][j] = 0;
                        a[i][j] = 1+rand() %25;
                        b[i][j] = 1+rand() %25;
                }
        }

        #pragma omp parallel for private(i,j,k) shared( a,b,c )
        for(i = 0;i < N;++i){
                for( j = 0; j < N; ++j){
                        for(k = 0; k < N; ++k){
                                c[i][j] += a[i][k] * b[k][j];
                        }
                }
        }

        int c_rows = sizeof(c) / sizeof(c[0]);
        int c_cols = sizeof(c[0]) / sizeof(c[0][0]);
        for(int i = 0;i < c_rows;i++){
                for(int j = 0; j < c_cols; j++){
                        printf("%d ", c[i][j]);
                }
                printf("\n");
        }
        return 0;
}
