#include <stdio.h>
#include <stdlib.h>

typedef struct Matrix{
  float** array;
  unsigned int x_size;
  unsigned int y_size;
} Matrix;

typedef struct Vector{
  float* vec;
  unsigned int size;
} Vector;

float** create_matrix(unsigned int* x_size, unsigned int* y_size){
  int i;

  float** matrix = (float**)malloc(*x_size*sizeof(float*));

  for(i=0; i<*x_size; ++i){
    matrix[i] = (float*)malloc(*y_size*sizeof(float));
  }
  return matrix;
}

float* create_vector(unsigned int *size){
  float* vec = (float*)malloc((*size)*sizeof(float));
  return vec;
}

void print_matrix(Matrix* matrix){
  int i,j;
  printf("Matrix:\n");
  for(i=0; i<(matrix->x_size); ++i){
    printf("[");
    for(j=0; j<(matrix->y_size)-1; ++j){
      printf("%f ", *(*((matrix->array)+i)+j));
    }
    printf("%f]\n", *(*((matrix->array)+i)+j));
  }
}

void print_vector(Vector* vector){
  int i;
  printf("Vector: [");
  for(i=0; i<(vector->size)-1; ++i){
    printf("%f, ", *((vector->vec)+i));
  }
  printf("%f]\n",*((vector->vec)+i));
}

void free_vector(Vector* vector){
  free(vector->vec);
}

void free_matrix(Matrix* matrix){
  int i;
  for(i=0; i<(matrix->x_size); ++i){
    free(*(matrix->array+i));
  }
  free(matrix->array);
}

Matrix blank_matrix(){
  Matrix blank;
  blank.array = NULL;
  blank.x_size = 0;
  blank.y_size = 0;
  return blank;
}

Matrix multiply_matrices(Matrix* m1, Matrix* m2){
  if(m1->y_size != m2->x_size || m1->array == NULL || m2->array == NULL){
    printf("Wrong matrix dimensions!\n");
    return blank_matrix();
  }else{
    int i,j,k,suma=0;
    Matrix result;
    result.x_size = m1->x_size;
    result.y_size = m2->y_size;
    result.array = create_matrix(&result.x_size, &result.y_size);
    for(i=0; i<m1->x_size; ++i){
      for(j=0; j<m2->y_size; ++j){
        for(k=0; k<m2->x_size; ++k){
          suma += *(*(m1->array+i)+k)*(*(*(m1->array+k)+j));
        }
        result.array[i][j]=suma;
        suma = 0;
      }
    }
    return result;
  }
}

Matrix add_matrices(Matrix* m1, Matrix* m2){
  if(m1->array == NULL || m2->array == NULL){
    printf("Null pointer in given matrix!\n");
    return blank_matrix();
  }else if(m1->x_size != m2->x_size || m1->y_size != m2->y_size){
    printf("Wrong matrix dimensions!\n");
    return blank_matrix();
  }else{
    int i,j;
    Matrix result;
    result.x_size = m1->x_size;
    result.y_size = m1->y_size;
    result.array = create_matrix(&result.x_size, &result.y_size);
    for(i=0; i<m1->x_size; ++i){
      for(j=0; j<m1->y_size; ++j){
        result.array[i][j] = m1->array[i][j]+m2->array[i][j];
      }
    }
    return result;
  }
}

Matrix subtract_matrices(Matrix* m1, Matrix* m2){
  if(m1->array == NULL || m2->array == NULL){
    printf("Null pointer in given matrix!\n");
    return blank_matrix();
  }else if(m1->x_size != m2->x_size || m1->y_size != m2->y_size){
    printf("Wrong matrix dimensions!\n");
    return blank_matrix();
  }else{
    int i,j;
    Matrix result;
    result.x_size = m1->x_size;
    result.y_size = m1->y_size;
    result.array = create_matrix(&result.x_size, &result.y_size);
    for(i=0; i<m1->x_size; ++i){
      for(j=0; j<m1->y_size; ++j){
        result.array[i][j] = m1->array[i][j]-m2->array[i][j];
      }
    }
    return result;
  }
}

int main(){
  // testing library
  Vector vector;
  Matrix matrix;
  vector.size = 3;
  matrix.x_size = 2;
  matrix.y_size = 2;
  vector.vec = create_vector(&vector.size);
  matrix.array = create_matrix(&matrix.x_size, &matrix.y_size);
  vector.vec[0] = 1;
  vector.vec[1] = 2;
  vector.vec[2] = 3;
  matrix.array[0][0] = 1;
  matrix.array[0][1] = 2;
  matrix.array[1][0] = 3;
  matrix.array[1][1] = 4;
  print_vector(&vector);
  print_matrix(&matrix);
  Matrix multiply_result = multiply_matrices(&matrix, &matrix);
  Matrix add_result = add_matrices(&matrix, &matrix);
  Matrix subtract_result = subtract_matrices(&matrix, &matrix);
  print_matrix(&multiply_result);
  print_matrix(&add_result);
  print_matrix(&subtract_result);

  free_vector(&vector);
  free_matrix(&matrix);
  free_matrix(&multiply_result);
  return 0;
}
