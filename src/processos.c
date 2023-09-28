#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
  int row, col;
  float **mat;
} Matrix;

typedef struct iarray
{
  int size;
  int *elements;
} iarray;

void matrixInit(Matrix *matrix, int row, int col)
{
  matrix->row = row;
  matrix->col = col;

  matrix->mat = calloc(row, sizeof(float *));
  for (int i = 0; i < row; i++)
  {
    matrix->mat[i] = calloc(col, sizeof(float));
  }
}

void matrixRead(const char *file_name, Matrix *matrix)
{
  FILE *matrix_file;
  int row, column;

  matrix_file = fopen(file_name, "r");
  fscanf(matrix_file, "%d %d\n", &row, &column);
  matrixInit(matrix, row, column);

  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < column; j++)
    {
      fscanf(matrix_file, "%f", &matrix->mat[i][j]);
    }
  }
  fclose(matrix_file);
}

void matrixWrite(const char *file_name, Matrix *matrix, iarray *indexes,
                 float time)
{
  FILE *file = fopen(file_name, "w");

  if (!file)
    printf("Erro ao escrever matriz\n");

  fprintf(file, "%d %d\n", matrix->row, matrix->col);
  for (int i = 0; i <= indexes->size; ++i)
  {
    int x = indexes->elements[i] / matrix->row;
    int y = indexes->elements[i] % matrix->row;

    fprintf(file, "c%d%d %f\n", x + 1, y + 1, matrix->mat[x][y]);
  }

  fprintf(file, "%f", time);
  fclose(file);
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    printf("Você deve fornecer o caminho de 2 arquivos contendo as matrizes e "
           "o número de elementos por thread\n");
    return 1;
  }

  int p = atoi(argv[3]);
  pid_t pid = getpid();
  int indexes[p];
  iarray arg = {0, indexes};
  Matrix matrix1, matrix2, result;
  char file_name[50];

  matrixRead(argv[1], &matrix1);
  matrixRead(argv[2], &matrix2);
  matrixInit(&result, matrix1.col, matrix2.row);

  int n_processes = ceilf((float)(result.row * result.col) / p);
  // printf("Matriz1 col=%d, Matriz2 row=%d\n", matrix1.col, matrix2.row);
  // printf("Matriz resultante: row=%d, col=%d\n", result.row, result.col);
  // printf("Total: %d, em float: %f\n", result.row * result.col, (float)(result.row * result.col));
  // printf("Número de processos: %d\n", n_processes);
  // return 0;
  pid_t processes[n_processes];

  for (int i = 0; i < n_processes; i++)
  {
    arg.size = 0;
    for (int j = 0; j < p; j++)
    {
      if ((p * i + j) >= (result.row * result.col))
        break;
      arg.size = j;
      arg.elements[j] = (p * i + j);
    }

    sprintf(file_name, "./mat/processos/%d.txt", i);
    processes[i] = fork();
    // printf("Pid do processo atual: %d\n", processes[i]);
    if (processes[i] == 0)
    {
      pid = processes[i];
      break;
    }
  }

  if (pid == 0)
  {
    time_t start = clock();
    for (int i = 0; i <= arg.size; i++)
    {
      int x = arg.elements[i] / result.row;
      int y = arg.elements[i] % result.row;

      result.mat[x][y] = 0;
      for (int j = 0; j < matrix1.col; ++j)
      {
        result.mat[x][y] += matrix1.mat[x][j] * matrix2.mat[j][y];
      }
    }
    time_t end = clock();
    float total_time = (float)(end - start) / CLOCKS_PER_SEC;

    matrixWrite(file_name, &result, &arg, total_time);
    exit(EXIT_SUCCESS);
  }

  for (int i = 0; i < n_processes; i++)
  {
    wait(NULL);
  }

  return 0;
}