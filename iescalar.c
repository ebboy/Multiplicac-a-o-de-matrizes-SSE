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

int main (int argc, char **argv){
   float *M = NULL;
    int width = atoi(argv[1]), height = atoi(argv[2]);// align = atoi(argv[3]);



   int iM, jM, kM, L, i;
   int coord, identidade, diag;// align = atoi(argv[3]);
   float soma_col;

   M = (float*) malloc(width * height * sizeof(float));

   for (jM = 0; jM < height; jM++){
      for (iM = 0; iM < width; iM++){
         int kM = jM * width + iM;
         //M[kM] = (float) kM + 1;
         /**/
         if (jM == iM)
            M[kM] = 1.0f;
         else
            M[kM] = 0.0f;

      }
   }

   for (L = 0; L < height; L++){
        soma_col = 0;
        diag = L * width + L;
        if(M[diag] != 1){
            identidade = 0;
            break;
        }
        for(i = 0; i < width; i++){
            coord = L * height + i;
            soma_col += M[coord] * M[coord];
        }
        if(soma_col != 1){
            identidade = 0;
            break;
        } else{
            identidade = 1;
        }
    }

   //0printMatrix(M, width, height);
   //printf("\n%d\n", steps);
   printf("\n E identidade : %d\n", identidade);

   free (M);
   return EXIT_SUCCESS;
}
