#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_VALUE 35

void gen_matrix(int row, int column, const char* file_name){
    
  FILE *matrix;

  matrix = fopen(file_name, "w");

  if(!matrix){
    printf("Ocorreu um erro ao abrir o arquivo destino\n");
  }
  
  srand(time(NULL));

  fprintf(matrix, "%d %d\n", row, column);
  
  for(int i = 1; i <= row; ++i){  
    for(int j = 1; j <= column; ++j){
      fprintf(matrix, "%f ", drand48() * (rand() % MAX_VALUE));
    }
    fputc('\n', matrix);
  }
  
  fclose(matrix);
}

int main(int argc, char* argv[]){  
  if(argc != 5){
    printf("Você deve fornecer 4 e apenas 4 argumentos\n");
    return 1;
  }

  if(atoi(argv[2]) != atoi(argv[3])){
    printf("A multiplicação de matrizes só é possível se as matrizes tiverem dimensões AxN * NxB\n");
    printf("Dimensões passadas %dx%d - %dx%d\n", atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    return 1;
  }

  srand(time(NULL));

  float discard_number = drand48();
  
  gen_matrix(atoi(argv[1]), atoi(argv[2]), "./mat/matriz1.txt");
  gen_matrix(atoi(argv[3]), atoi(argv[4]), "./mat/matriz2.txt");

  return 0;
}