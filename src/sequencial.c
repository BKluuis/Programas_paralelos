#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
  int row, col;
  float **mat;
} Matrix;

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

void matrixWrite(const char *file_name, Matrix *matrix, float time)
{
  FILE *file = fopen(file_name, "w");

  if (!file)
    printf("Erro ao escrever matriz\n");

  fprintf(file, "%d %d\n", matrix->row, matrix->col);
  for (int x = 0; x < matrix->row; ++x)
  {
    for (int y = 0; y < matrix->col; ++y)
    {
      fprintf(file, "c%d%d %f\n", x + 1, y + 1, matrix->mat[x][y]);
    }
  }

  fprintf(file, "%f", time);
  fclose(file);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Você deve fornecer o caminho de 2 arquivos contendo as matrizes\n");
    return 1;
  }

  Matrix m1, m2, res;

  matrixRead(argv[1], &m1);
  matrixRead(argv[2], &m2);
  matrixInit(&res, m1.col, m2.row);

  time_t start = clock();

  for (int i = 0; i < res.row; ++i)
  {
    for (int j = 0; j < res.col; ++j)
    {
      for (int k = 0; k < res.col; ++k)
      {
        res.mat[i][j] += m1.mat[i][k] * m2.mat[k][j];
      }
    }
  }

  time_t end = clock();

  float total_time = (float)(end - start) / CLOCKS_PER_SEC;

  matrixWrite("./mat/sequencial.txt", &res, total_time);

  return 0;
}