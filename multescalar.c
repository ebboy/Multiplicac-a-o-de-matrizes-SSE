#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void printMatrix(float *m, float w, float h){
   int i, j;

   printf("\n");

   for (j = 0; j < h; j++){
      for (i = 0; i < w; i++){
         int k = j * w + i;
         printf("%.1f ", m[k]);
      }
      printf("\n");
   }

}
void transporEscalar4x4(float *A, float *B, int wA, int wB){
    int i;
    //float C[4];
    for(i = 0; i < 4; i++){
        B[i] = A[i*wA];
        B[1*wB + i] = A[i*wA+1];
        B[2*wB + i] = A[i*wA+2];
        B[3*wB + i] = A[i*wA+3];

    }
}

void transporEscalarBloco(float *A, float *B, const int n, const int m, const int lda, const int ldb ,const int block_size) {
    //#pragma omp parallel for
    int i,j,j2,i2;
    for(i=0; i<n; i+=block_size) {
        for(j=0; j<m; j+=block_size) {
            int max_i2 = i+block_size < n ? i + block_size : n;
            int max_j2 = j+block_size < m ? j + block_size : m;
            for(i2=i; i2<max_i2; i2+=4) {
                for(j2=j; j2<max_j2; j2+=4) {
                    transporEscalar4x4(&A[i2*lda +j2], &B[j2*ldb + i2], lda, ldb);
                }
            }
        }
    }
}


int main (int argc, char **argv){
   float *A = NULL, *B = NULL, *C = NULL, *Bt = NULL;
   int iA, jA, iB, jB, iC, jC;
   int width = atoi(argv[1]), height = atoi(argv[2]);// align = atoi(argv[3]);

   int kC = 0;// align = atoi(argv[3]);

   /*scanf("%d", &width);
   scanf("%d", &height);*/

   A = (float*) malloc(width * height * sizeof(float));
   B = (float*) malloc(width * height * sizeof(float));
   C = (float*) malloc(width * height * sizeof(float));
   Bt = (float*) malloc(width * height * sizeof(float));

   printf("\nMultiplicando matriz\n");
   for (jC = 0; jC < height; jC++){
      for (iC = 0; iC < width; iC++){
         int kC = jC * width + iC;
         A[kC] = (float) kC + 1;
         if (jC == iC)
            B[kC] = 1.0f;
         else
            B[kC] = 0.0f;

      }
   }
    transporEscalarBloco(B, Bt, width, height, width, height, 16);

   int steps= 0;
   for (jC = 0; jC < height; jC++){
      for (iC = 0; iC < width; iC++){
         int kC = jC * width + iC;
         float aux = 0.0f;
         //jA = jC;
         for (iA = 0; iA < width; iA++){
            //jB = iA;
           // iB = iC;
            int kA = jC * width + iA;
            int kB = iC * width + iA;
            aux += A[kA] * Bt[kB];

            steps++;
         }
         C[kC] = aux;
      }//end-      for (iC = 0; iC < 2; iC++){
   }//end-for (jC = 0; jC < 2; jC++){

   //printMatrix(C, width, height);
   printf("\n%d\n", steps);
   free (A);
   free (B);
   free (C);
   return EXIT_SUCCESS;
}
