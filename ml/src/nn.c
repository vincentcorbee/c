#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define MATRIX_PRINT(matrix) matrix_print(matrix, #matrix, 0)
#define MATRIX_AT(matrix, row, col) (matrix).data[(row) * (matrix).stride + (col)]

#define NEURAL_NETWORK_PRINT(nn) neural_network_print(nn, #nn)
#define NEURAL_NETWORK_INPUT(nn) (nn).as[0]
#define NEURAL_NETWORK_OUTPUT(nn) (nn).as[(nn).layer_count]

#define ARRAY_LENGTH(array) sizeof((array)) / sizeof((array[0]))

float sigmoidf(float x)
{
  return 1.f / (1.f + expf(-x));
}

float mse(float predicted, float actual)
{
  return 0.5 * ((predicted - actual) * (predicted - actual));
}

float rand_float()
{
  return (float)rand() / (float)RAND_MAX;
}

typedef struct Matrix Matrix;

Matrix matrix_alloc(size_t rows, size_t cols);
Matrix matrix_create(size_t rows, size_t cols);
Matrix matrix_row(Matrix matrix, size_t row);
void matrix_fill(Matrix src, float value);
void matrix_rand(Matrix src, float lo, float hi);
void matrix_copy(Matrix dst, Matrix src);
void matrix_add(Matrix a, Matrix b);
void matrix_subtract(Matrix dst, Matrix a, Matrix b);
void matrix_dot(Matrix dst, Matrix a, Matrix b);
void matrix_map(Matrix src, float (*activation)(float value));
void matrix_transpose(Matrix dst, Matrix src);
float matrix_get(Matrix src, size_t row, size_t col);
void matrix_set(Matrix src, size_t row, size_t col, float value);
Matrix matrix_set_data(Matrix src, float *data);
void matrix_print(Matrix src, const char *name, int padding);

typedef struct NeuralNetwork NeuralNetwork;

NeuralNetwork neural_network_alloc(size_t *config, size_t config_count);
void neural_network_print(NeuralNetwork nn, const char *name);
void neural_network_rand(NeuralNetwork src, float lo, float hi);
void neural_network_zero(NeuralNetwork nn);
void neural_network_finite_diff(NeuralNetwork nn, NeuralNetwork g, float eps, Matrix ti, Matrix to);
void neural_network_backpropagation(NeuralNetwork nn, NeuralNetwork g, Matrix ti, Matrix to);
float neural_network_cost(NeuralNetwork nn, Matrix ti, Matrix to);
void neural_network_learn(NeuralNetwork nn, NeuralNetwork g, float rate);

typedef struct NeuralNetwork
{
  size_t layer_count;
  Matrix *ws;
  Matrix *bs;
  Matrix *as;
} NeuralNetwork;

typedef struct Matrix
{
  size_t rows;
  size_t cols;
  size_t stride;
  float *data;
} Matrix;

Matrix matrix_alloc(size_t rows, size_t cols)
{
  Matrix m;

  m.rows = rows,
  m.cols = cols,
  m.stride = cols,
  m.data = malloc(rows * cols * sizeof(*m.data));

  assert(m.data != NULL);

  return m;
}

Matrix matrix_create(size_t rows, size_t cols)
{
  return (Matrix){
      .rows = rows,
      .cols = cols,
      .stride = cols,
      .data = NULL};
}

void matrix_copy(Matrix dst, Matrix src)
{
  assert(src.rows == dst.rows);
  assert(dst.cols == dst.cols);

  for (size_t i = 0; i < dst.rows; i++)
  {
    for (size_t j = 0; j < dst.cols; j++)
    {
      MATRIX_AT(dst, i, j) = MATRIX_AT(src, i, j);
    }
  }
}

Matrix matrix_row(Matrix src, size_t row)
{
  return (Matrix){
      .rows = 1,
      .cols = src.cols,
      .stride = src.stride,
      .data = &MATRIX_AT(src, row, 0)};
}

Matrix matrix_col(Matrix src, size_t start, size_t end)
{
  size_t cols = end - start + 1;
  Matrix dst = {
      .rows = src.rows,
      .cols = cols,
      .stride = src.stride,
      .data = src.data + start,
  };

  return dst;
}

float matrix_get(Matrix src, size_t row, size_t col)
{
  return MATRIX_AT(src, row, col);
}

void matrix_set(Matrix dest, size_t row, size_t col, float value)
{
  MATRIX_AT(dest, row, col) = value;
}

Matrix matrix_set_data(Matrix dest, float *data)
{
  dest.data = data;

  return dest;
}

void matrix_fill(Matrix dst, float value)
{
  for (size_t i = 0, l = dst.rows; i < l; i++)
  {
    for (size_t j = 0, l = dst.cols; j < l; j++)
    {
      MATRIX_AT(dst, i, j) = value;
    }
  }
}

void matrix_rand(Matrix src, float lo, float hi)
{
  for (size_t i = 0; i < src.rows; i++)
  {
    for (size_t j = 0; j < src.cols; j++)
      MATRIX_AT(src, i, j) = rand_float() * (hi - lo) + lo;
  }
}

void matrix_add(Matrix a, Matrix b)
{
  assert(a.rows == b.rows);
  assert(a.cols == b.cols);

  for (int i = 0; i < a.rows; i++)
  {
    for (int j = 0; j < a.cols; j++)
    {
      MATRIX_AT(a, i, j) += MATRIX_AT(b, i, j);
    }
  }
}

void matrix_subtract(Matrix dst, Matrix a, Matrix b)
{
  assert(a.rows == b.rows);
  assert(a.cols == b.cols);

  for (int i = 0; i < a.rows; i++)
  {
    for (int j = 0; j < a.cols; j++)
    {
      MATRIX_AT(dst, i, j) = MATRIX_AT(a, i, j) - MATRIX_AT(b, i, j);
    }
  }
}

void matrix_dot(Matrix dst, Matrix a, Matrix b)
{
  assert(a.cols == b.rows);
  assert(dst.rows == a.rows);
  assert(dst.cols == b.cols);

  for (int i = 0; i < a.rows; i++)
  {
    for (int j = 0; j < b.cols; j++)
    {
      MATRIX_AT(dst, i, j) = 0;

      for (int k = 0; k < a.cols; k++)
      {
        MATRIX_AT(dst, i, j) += MATRIX_AT(a, i, k) * MATRIX_AT(b, k, j);
      }
    }
  }
}

void matrix_transpose(Matrix dst, Matrix src)
{
  for (size_t i = 0; i < src.rows; i++)
  {
    for (size_t j = 0; j < src.cols; j++)
    {
      MATRIX_AT(dst, j, i) = MATRIX_AT(src, i, j);
    }
  }
}

void matrix_map(Matrix dst, float (*activation)(float value))
{
  for (size_t i = 0; i < dst.rows; i++)
  {
    for (size_t j = 0; j < dst.cols; j++)
      MATRIX_AT(dst, i, j) = activation(MATRIX_AT(dst, i, j));
  }
}

void matrix_print(Matrix src, const char *name, int padding)
{
  printf("%*s%s = [\n", padding, "", name);

  for (size_t i = 0; i < src.rows; i++)
  {
    printf("%*s  ", padding, "");
    for (size_t j = 0; j < src.cols; j++)
    {
      printf("%f ", MATRIX_AT(src, i, j));
    }

    printf("\n");
  }
  printf("%*s]\n", padding, "");
}

/* Neural network */

NeuralNetwork neural_network_alloc(size_t *config, size_t config_count)
{
  NeuralNetwork nn;

  nn.layer_count = config_count - 1;
  nn.ws = malloc(sizeof(*nn.ws) * nn.layer_count);
  assert(nn.ws != NULL);
  nn.bs = malloc(sizeof(*nn.bs) * nn.layer_count);
  assert(nn.bs != NULL);
  nn.as = malloc(sizeof(*nn.as) * config_count);
  assert(nn.as != NULL);

  nn.as[0] = matrix_alloc(1, config[0]);

  for (size_t i = 1; i < config_count; i++)
  {
    nn.ws[i - 1] = matrix_alloc(nn.as[i - 1].cols, config[i]);
    nn.bs[i - 1] = matrix_alloc(1, config[i]);
    nn.as[i] = matrix_alloc(1, config[i]);
  }

  return nn;
}

void neural_network_rand(NeuralNetwork nn, float lo, float hi)
{
  for (size_t i = 0; i < nn.layer_count; i++)
  {
    matrix_rand(nn.ws[i], lo, hi);
    matrix_rand(nn.bs[i], lo, hi);
  }
}

void neural_network_print(NeuralNetwork nn, const char *name)
{
  char buf[256];

  printf("%s = [\n", name);

  for (size_t i = 0; i < nn.layer_count; i++)
  {
    snprintf(buf, sizeof(buf), "ws%zu", i);
    matrix_print(nn.ws[i], buf, 4);
    snprintf(buf, sizeof(buf), "bs%zu", i);
    matrix_print(nn.bs[i], buf, 4);
  }

  printf("]\n");
}

void neural_network_zero(NeuralNetwork nn)
{
  for (size_t i = 0; i < nn.layer_count; i++)
  {
    matrix_fill(nn.as[i], 0);
    matrix_fill(nn.ws[i], 0);
    matrix_fill(nn.bs[i], 0);
  }

  matrix_fill(nn.as[nn.layer_count], 0);
}

void neural_network_forward(NeuralNetwork nn)
{
  for (size_t i = 0; i < nn.layer_count; i++)
  {
    matrix_dot(nn.as[i + 1], nn.as[i], nn.ws[i]);
    matrix_add(nn.as[i + 1], nn.bs[i]);
    matrix_map(nn.as[i + 1], sigmoidf);
  }
}

float neural_network_cost(NeuralNetwork nn, Matrix ti, Matrix to)
{
  assert(ti.rows == to.rows);
  assert(to.cols == NEURAL_NETWORK_OUTPUT(nn).cols);

  size_t sample_count = ti.rows;

  float c = 0;

  for (size_t i = 0; i < sample_count; i++)
  {
    Matrix x = matrix_row(ti, i);
    Matrix y = matrix_row(to, i);

    matrix_copy(NEURAL_NETWORK_INPUT(nn), x);

    neural_network_forward(nn);

    for (size_t j = 0; j < to.cols; j++)
    {
      float d = MATRIX_AT(NEURAL_NETWORK_OUTPUT(nn), 0, j) - MATRIX_AT(y, 0, j);

      c += d * d;
    }
  }

  return c / sample_count;
}

void neural_network_finite_diff(NeuralNetwork nn, NeuralNetwork g, float eps, Matrix ti, Matrix to)
{
  float saved;
  float c = neural_network_cost(nn, ti, to);

  for (size_t i = 0; i < nn.layer_count; i++)
  {
    for (size_t j = 0; j < nn.ws[i].rows; j++)
    {
      for (size_t k = 0; k < nn.ws[i].cols; k++)
      {
        saved = MATRIX_AT(nn.ws[i], j, k);
        MATRIX_AT(nn.ws[i], j, k) += eps;
        MATRIX_AT(g.ws[i], j, k) = (neural_network_cost(nn, ti, to) - c) / eps;
        MATRIX_AT(nn.ws[i], j, k) = saved;
      }
    }

    for (size_t j = 0; j < nn.bs[i].rows; j++)
    {
      for (size_t k = 0; k < nn.bs[i].cols; k++)
      {
        saved = MATRIX_AT(nn.bs[i], j, k);
        MATRIX_AT(nn.bs[i], j, k) += eps;
        MATRIX_AT(g.bs[i], j, k) = (neural_network_cost(nn, ti, to) - c) / eps;
        MATRIX_AT(nn.bs[i], j, k) = saved;
      }
    }
  }
}

void neural_network_backpropagation(NeuralNetwork nn, NeuralNetwork g, Matrix ti, Matrix to)
{
  assert(ti.rows == to.rows);
  assert(to.cols == NEURAL_NETWORK_OUTPUT(nn).cols);

  neural_network_zero(g);

  /*
    i = current sample
    l = current layer
    j = current activation
    k = previous activation
  */

  size_t sample_count = ti.rows;

  for (size_t i = 0; i < sample_count; i++)
  {
    matrix_copy(NEURAL_NETWORK_INPUT(nn), matrix_row(ti, i));

    neural_network_forward(nn);

    for (size_t j = 0; j <= nn.layer_count; j++)
    {
      matrix_fill(g.as[j], 0);
    }

    for (size_t j = 0; j < to.cols; j++)
    {
      MATRIX_AT(NEURAL_NETWORK_OUTPUT(g), 0, j) = MATRIX_AT(NEURAL_NETWORK_OUTPUT(nn), 0, j) - MATRIX_AT(to, i, j);
    }

    for (size_t l = nn.layer_count; l > 0; l--)
    {
      for (size_t j = 0; j < nn.as[l].cols; j++)
      {
        float a = MATRIX_AT(nn.as[l], 0, j);
        float da = MATRIX_AT(g.as[l], 0, j);

        MATRIX_AT(g.bs[l - 1], 0, j) += 2 * da * a * (1 - a);

        for (size_t k = 0; k < nn.as[l - 1].cols; k++)
        {
          /*
            j = weight matrix col
            k = weight matrix row
          */

          float pa = MATRIX_AT(nn.as[l - 1], 0, k);
          float w = MATRIX_AT(nn.ws[l - 1], k, j);

          MATRIX_AT(g.ws[l - 1], k, j) += 2 * da * a * (1 - a) * pa;
          MATRIX_AT(g.as[l - 1], 0, k) += 2 * da * a * (1 - a) * w;
        }
      }
    }
  }

  for (size_t i = 0; i < g.layer_count; i++)
  {
    for (size_t j = 0; j < g.ws[i].rows; j++)
    {
      for (size_t k = 0; k < g.ws[i].cols; k++)
      {
        MATRIX_AT(g.ws[i], j, k) /= sample_count;
      }
    }

    for (size_t j = 0; j < g.bs[i].rows; j++)
    {
      for (size_t k = 0; k < g.bs[i].cols; k++)
      {
        MATRIX_AT(g.bs[i], j, k) /= sample_count;
      }
    }
  }
}

void neural_network_learn(NeuralNetwork nn, NeuralNetwork g, float rate)
{
  for (size_t i = 0; i < nn.layer_count; i++)
  {
    for (size_t j = 0; j < nn.ws[i].rows; j++)
    {
      for (size_t k = 0; k < nn.ws[i].cols; k++)
      {
        MATRIX_AT(nn.ws[i], j, k) -= rate * MATRIX_AT(g.ws[i], j, k);
      }
    }

    for (size_t j = 0; j < nn.bs[i].rows; j++)
    {
      for (size_t k = 0; k < nn.bs[i].cols; k++)
      {
        MATRIX_AT(nn.bs[i], j, k) -= rate * MATRIX_AT(g.bs[i], j, k);
      }
    }
  }
}

float training_data_xor[] = {
    0, 0, 0,
    0, 1, 1,
    1, 0, 1,
    1, 1, 0};

float training_data_or[] = {
    0, 0, 0,
    0, 1, 1,
    1, 0, 1,
    1, 1, 1};

int main(void)
{
  srand(time(0));

  float *training_data = training_data_xor;

  float eps = 1e-1;
  float rate = 1e-1;

  Matrix train = matrix_set_data(matrix_create(4, 3), training_data);

  Matrix ti = matrix_col(train, 0, 1);

  Matrix to = matrix_col(train, 2, 2);

  size_t config[] = {2, 2, 1};

  NeuralNetwork nn = neural_network_alloc(config, ARRAY_LENGTH(config));
  NeuralNetwork g = neural_network_alloc(config, ARRAY_LENGTH(config));

  neural_network_rand(nn, 0, 1);

  printf("c = %f\n", neural_network_cost(nn, ti, to));

  for (size_t i = 0; i < 75000; i++)
  {
#if 0
    neural_network_finite_diff(nn, g, eps, ti, to);
#else
    neural_network_backpropagation(nn, g, ti, to);
#endif
    neural_network_learn(nn, g, rate);

    // printf("%zu: c = %f\n", i, neural_network_cost(nn, ti, to));
  }

  NEURAL_NETWORK_PRINT(nn);

  printf("c = %f\n", neural_network_cost(nn, ti, to));

  for (size_t i = 0; i < 2; i++)
  {
    for (size_t j = 0; j < 2; j++)
    {
      MATRIX_AT(NEURAL_NETWORK_INPUT(nn), 0, 0) = i;
      MATRIX_AT(NEURAL_NETWORK_INPUT(nn), 0, 1) = j;
      neural_network_forward(nn);
      printf("%zu ^ %zu = %f\n", i, j, MATRIX_AT(NEURAL_NETWORK_OUTPUT(nn), 0, 0));
    }
  }

  return 0;
}