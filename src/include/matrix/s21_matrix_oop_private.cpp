#include "s21_matrix_oop.h"

// private part of S21Matrix

// methods:
// *--------------------------------------------------------------------------*
// *--------------------------------------------------------------------------*

/**
 * @brief зануляет атрибуты матрицы
 * @note group: privat methods
 */

void S21Matrix::S21Nulling(void) {
  matrix_ = nullptr;
  rows_ = 0;
  cols_ = 0;
}

/**
 * @brief создает матрицу по указанным в классе значениям атрибутов rows_ и
 * cols_
 * @note group: privat methods
 */
void S21Matrix::S21CreateMatrix(void) {
  matrix_ = new double *[rows_]();
  for (int i = 0; i != rows_; i++) {
    matrix_[i] = new double[cols_]();
  }
}

/**
 * @brief очищает матрицу
 * @note group: privat methods
 */
void S21Matrix::S21RemoveMatrix(void) {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
    S21Nulling();
  }
}

/**
 * @brief копирует матрицу из other в текущую (this)
 * @note group: privat methods
 */
void S21Matrix::S21CopyMatrix(const S21Matrix &other) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

/**
 * @brief сравнивает два double с точностью до 9e-8
 * @return результат сравнения (false - не равны, true - равны)
 * @note group: privat methods
 */
bool S21Matrix::S21CompareDoubles(const double &first,
                                  const double &second) const {
  return (fabs(first - second) > EPSILON) ? false : true;
}

/**
 * @brief копирует строку матрицы (строка - row) в temp_row из matrix
 * @note group: privat methods
 */
void S21Matrix::S21RowCopy(const S21Matrix &matrix, double *temp_row,
                           const int &row) {
  for (int i = 0; i < matrix.cols_; i++) temp_row[i] = matrix.matrix_[row][i];
}

/**
 * @brief копирует столбец матрицы (строка - row) в temp_col из matrix
 * @note group: privat methods
 */
void S21Matrix::S21ColCopy(const S21Matrix &matrix, double *temp_col,
                           const int &col) {
  for (int i = 0; i < matrix.rows_; i++) temp_col[i] = matrix.matrix_[i][col];
}

/**
 * @brief Вспомогательная функция для перемножения стоки и столбца
 * @return возвращает результат перемножения строки на столбец
 * @note group: privat methods
 */
double S21Matrix::S21RowColMul(const double *temp_row, const double *temp_col,
                               const int col_size) {
  double mul_result = 0.0;
  for (int i = 0; i < col_size; i++) mul_result += temp_row[i] * temp_col[i];
  return mul_result;
}

/**
 * @brief вспомогательная функция для транспонирования матрицы, результат
 * транспонирования переносится в result
 * @note group: privat methods
 */
void S21Matrix::S21TransposeAdditional(const S21Matrix &result) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      result.matrix_[j][i] = matrix_[i][j];
    }
  }
}

/**
 * @brief вспомогательная функция, берет кофактор матрицы, результат переносится
 * в temp
 * @note group: privat methods
 */
void S21Matrix::S21GetCofactor(S21Matrix &temp, int row_income,
                               int column_income, int size) const {
  int i = 0, j = 0;
  for (int row = 0; row < size; row++) {
    for (int column = 0; column < size; column++) {
      if (row != row_income && column != column_income) {
        temp.matrix_[i][j++] = matrix_[row][column];
        if (j == size - 1) {
          j = 0;
          i++;
        }
      }
    }
  }
}

/**
 * @brief вспомогательная функция для расчета детерминанта, расчитывается
 * рекурсивно
 * @return результат расчета, (double)result
 * @note group: privat methods
 */
double S21Matrix::S21CalcDeterminant(int size) const {
  if (size == 1) return matrix_[0][0];
  double sign = 1.0, result = 0.0;
  S21Matrix temp(size - 1, size - 1);
  for (int column = 0; column < size; column++) {
    S21GetCofactor(temp, 0, column, size);
    result += sign * matrix_[0][column] * temp.S21CalcDeterminant(size - 1);
    sign *= -1;
  }
  return result;
}

/**
 * @brief вспомогательная функция для расчета матрицы алгебраических дополнений
 * @note group: privat methods
 */
void S21Matrix::S21CalcComplAdditional(S21Matrix &result) {
  if (rows_ == 1) {
    result.matrix_[0][0] = 1;
  } else {
    double minor = 0.0;
    S21Matrix temp(rows_ - 1, cols_ - 1);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        S21GetCofactor(temp, i, j, rows_);
        minor = temp.S21CalcDeterminant(temp.cols_);
        result.matrix_[i][j] = ((i + j) % 2 == 0 ? 1 : -1) * minor;
      }
    }
  }
}

// exceptions:
// *--------------------------------------------------------------------------*
// *--------------------------------------------------------------------------*

void S21Matrix::S21ExceptionDifferentMatrixDemension(
    const S21Matrix &other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::invalid_argument("Different demensions of matrices!");
}

void S21Matrix::S21ExceptionDoesNotExist(const S21Matrix &other) const {
  if (matrix_ == nullptr || other.matrix_ == nullptr)
    throw std::invalid_argument("Matrix does not exist!");
}

void S21Matrix::S21ExceptionDoesNotExist(void) const {
  if (matrix_ == nullptr) throw std::invalid_argument("Matrix does not exist!");
}

void S21Matrix::S21ExceptionDifferentDemensionMul(
    const S21Matrix &other) const {
  if (cols_ != other.rows_)
    throw std::invalid_argument("Different demensions of matrices for mul!");
}

void S21Matrix::S21ExceptionIsNotSquare(void) const {
  if (rows_ != cols_) throw std::invalid_argument("Matrix is not square!");
}

void S21Matrix::S21ExceptionDetIsZero(void) const {
  if (fabs(S21CalcDeterminant(rows_)) < EPSILON)
    throw std::invalid_argument("Determinant equal 0!");
}

void S21Matrix::S21ExceptionOutOfRange(const int &i, const int &j) const {
  if (i >= rows_ || j >= cols_ || i < 0 || j < 0)
    throw std::invalid_argument("Out of range!");
}