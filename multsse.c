#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xmmintrin.h>

void printMatriz(float *m,int w, int h){
   int i, j;

   printf("\n");

   for (j = 0; j < h; j++){
      for (i = 0; i < w; i++){
         int k = j * w + i;
         printf("%.2f ", m[k]);
      }
      printf("\n");
   }
}


//void transpose4x4_SSE(float *A, float *B, const int lda, const int ldb) {
void SSE_transpor4x4(float *A, float *B, const int wA, const int wB) {
    __m128 linha_1 = _mm_load_ps(&A[0*wA]);
    __m128 linha_2 = _mm_load_ps(&A[1*wA]);
    __m128 linha_3 = _mm_load_ps(&A[2*wA]);
    __m128 linha_4 = _mm_load_ps(&A[3*wA]);
     _MM_TRANSPOSE4_PS(linha_1, linha_2, linha_3, linha_4);
     _mm_store_ps(&B[0*wB], linha_1);
     _mm_store_ps(&B[1*wB], linha_2);
     _mm_store_ps(&B[2*wB], linha_3);
     _mm_store_ps(&B[3*wB], linha_4);
}

//void transpose_block_SSE4x4(float *A, float *B, const int n, const int m, const int lda, const int ldb ,const int block_size) {
void SSE_transpor(float *A, float *B, const int n, const int m, const int wA, const int wB ,const int bloco_tam) {
    //#pragma omp parallel for
    int i,j,j2,i2;
    for(i = 0; i < n; i += bloco_tam) {
        for(j = 0; j < m; j += bloco_tam) {
            int max_i2 = i + bloco_tam < n ? i + bloco_tam : n;
            int max_j2 = j + bloco_tam < m ? j + bloco_tam : m;

            if((i + bloco_tam) < n)
                max_i2 = i + bloco_tam;
            else
                max_i2 = n;

            if((j + bloco_tam) < m)
                max_i2 = j + bloco_tam;
            else
                max_i2 = m;

            for(i2 = i; i2 < max_i2; i2 += 4) {
                for(j2 = j; j2 < max_j2; j2 += 4) {
                    SSE_transpor4x4(&A[i2*wA + j2], &B[j2*wB + i2], wA, wB);
                }
            }
        }
    }
}



int main (int argc, char **argv){
    int i, j, m, n, q;
    int k, kA, kB, kC;
    int width = atoi(argv[1]), height = atoi(argv[2]);// align = atoi(argv[3]);


    //int width, height;



    float *A = NULL,*B = NULL,*Bt = NULL, *C = NULL;

    //float A[width*height] __attribute__((aligned(16)));
    //float Bt[width*height] __attribute__((aligned(16)));
    //float C[width*height] __attribute__((aligned(16)));

    A = (float*) malloc(width * height * sizeof(float));
    B = (float*) malloc(width * height * sizeof(float));
    Bt = (float*) malloc(width * height * sizeof(float));
    C = (float*) malloc(width * height * sizeof(float));
    #pragma omp parallel for
    for (j = 0; j < height; j++){
        for (i = 0; i < width; i++){
            k = j * width + i;
            A[k] = (float) k + 1;
            if (j == i)
                B[k] = 1.0f;
            else
                B[k] = 0.0f;
        }
    }


    SSE_transpor(B, Bt, width, height, width, height, 16);


    __m128 a_matriz, b_matriz, aux_vet, r_matriz;
    float auxB[4];
    float aux_sum[4];
    float vet_sum[4];
    for(j = 0; j < height; j++ ){
        for(i = 0; i < width; i++){
            kC = j * width + i;
            r_matriz = _mm_set1_ps(0.0f);
            for(m = 0; m < width; m+=4){
                kA = j * width + m;
                kB = i * width + m;
                /*auxB[0] = B[kB];
                auxB[1] = B[kB + width];
                auxB[2] = B[kB + (2*width)];
                auxB[3] = B[kB + (3*width)];*/
                //printf("%.2f %.2f %.2f %.2f ", auxB[0], auxB[1], auxB[2], auxB[3]);
                a_matriz = _mm_load_ps(&A[kA]);
                b_matriz = _mm_load_ps(&Bt[kB]);
                //b_matriz = _mm_set_ps (B[kB + (3*width)], B[kB + (2*width)], B[kB + width], B[kB]);
                //aux_vet = _mm_mul_ps(a_matriz, b_matriz);
                r_matriz = _mm_add_ps(_mm_mul_ps(a_matriz, b_matriz), r_matriz);
            }
            _mm_store_ps(aux_sum, r_matriz);

            C[kC] = aux_sum[0] + aux_sum[1] + aux_sum[2] + aux_sum[3];

        }
    }
    //printMatriz(C, width, height);

}
