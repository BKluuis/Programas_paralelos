#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

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

typedef struct t_return{
  iarray array;
  float time;
} t_return;

Matrix *matrix1 = NULL;
Matrix *matrix2 = NULL;
Matrix *result = NULL;

void matrixAlloc(Matrix *(*matrix), int row, int col)
{
  (*matrix) = malloc(sizeof(Matrix));
  (*matrix)->row = row;
  (*matrix)->col = col;

  (*matrix)->mat = malloc(sizeof(float *) * row);
  for (int i = 0; i < row; i++)
  {
    (*matrix)->mat[i] = malloc(sizeof(float) * col);
  }
}

void matrixFree(Matrix *(*matrix))
{
  for (int i = 0; i < (*matrix)->row; i++)
  {
    free((*matrix)->mat[i]);
  }
  free((*matrix)->mat);
}

void matrixRead(const char *file_name, Matrix *(*matrix))
{
  FILE *matrix_file;
  int row, column;

  matrix_file = fopen(file_name, "r");
  fscanf(matrix_file, "%d %d\n", &row, &column);
  matrixAlloc(matrix, row, column);

  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < column; j++)
    {
      fscanf(matrix_file, "%f", &(*matrix)->mat[i][j]);
    }
  }
  fclose(matrix_file);
}

void matrixWrite(const char *file_name, Matrix *matrix, iarray *indexes, float time)
{
  FILE *file = fopen(file_name, "w");

  if (!file)
    printf("Erro ao escrever matriz\n");

  fprintf(file, "%d %d\n", matrix->row, matrix->col);
  for (int i = 0; i < indexes->size; ++i)
  {
    int x = indexes->elements[i] / matrix->row;
    int y = indexes->elements[i] % matrix->row;

    fprintf(file, "c%d%d %f\n", x + 1, y + 1, matrix->mat[x][y]);
  }

  fprintf(file, "%f", time);
  fclose(file);
}

void *calculate_elements(void *arg)
{
  iarray *indexes = (iarray *)arg;
  struct timespec _start;

  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_start);
  for (int i = 0; i < indexes->size; i++)
  {
    int x = indexes->elements[i] / result->row;
    int y = indexes->elements[i] % result->row;

    result->mat[x][y] = 0;
    for (int j = 0; j < matrix1->col; ++j)
    {
      result->mat[x][y] += matrix1->mat[x][j] * matrix2->mat[j][y];
    }
  }

  struct timespec _end;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_end);
  float total_time = (float)(_end.tv_sec - _start.tv_sec) + (float)(_end.tv_nsec - _start.tv_nsec) / 1000000000;

  t_return *r = {indexes, 0};
  r->time = total_time;

  pthread_exit((void*)r);
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    printf("Você deve fornecer o caminho de 2 arquivos contendo as matrizes e o número de elementos por thread\n");
    return 1;
  }

  int p = atoi(argv[3]);

  matrixRead(argv[1], &matrix1);
  matrixRead(argv[2], &matrix2);
  matrixAlloc(&result, matrix1->row, matrix2->col);

  int threads_length = ceilf((float)(matrix1->row * matrix2->col) / p);
  pthread_t threads[threads_length];
  void *thread_return;

  for (int i = 0; i < threads_length; ++i)
  {
    int *indexes = calloc(p, sizeof(int));
    iarray *arg = calloc(1, sizeof(iarray));
    arg->size = 0;
    arg->elements = indexes;

    for (int j = 0; j < p; j++)
    {
      if ((p * i + j) >= (result->row * result->col))
        break;
      arg->size = j + 1;
      arg->elements[j] = (p * i + j);
    }
    pthread_create(&threads[i], NULL, calculate_elements, (void *)arg);
  }

  for (int i = 0; i < threads_length; ++i)
  {
    pthread_join(threads[i], &thread_return);

    char file_name[30];
    sprintf(file_name, "./mat/threads/%d.txt", ((t_return*)thread_return)->array.elements[0]);
    matrixWrite(file_name, result, &((t_return*)thread_return)->array, ((t_return*)thread_return)->time);
  }

  matrixFree(&matrix1);
  matrixFree(&matrix2);
  matrixFree(&result);
  free(matrix1);
  return 0;
}