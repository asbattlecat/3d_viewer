#include "s21_matrix_oop.h"

#include <QDebug>
#include <string>

// PUBLIC PART OF S21Matrix

// constructors:
// *--------------------------------------------------------------------------*
// *--------------------------------------------------------------------------*

/**
 * @brief Cоздает матрицу по умолчанию, 0 строк, 0 столбцов, нулл на матрицу
 * @note group: constructor
 */
S21Matrix::S21Matrix() {
  rows_ = 0;
  cols_ = 0;
  matrix_ = nullptr;
}

/**
 * @brief создает матрицу с заданными rows и cols
 * @note group: constructor
 */
S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  S21CreateMatrix();
}

/**
 * @brief копирование матрицы из other в текщ
 * @note group: constructor
 */
S21Matrix::S21Matrix(const S21Matrix &other)
    : rows_(other.rows_), cols_(other.cols_) {
  S21CreateMatrix();
  S21CopyMatrix(other);
}

/**
 * @brief перемещение матрицы из other в текщ
 * @note group: constructor
 */
S21Matrix::S21Matrix(S21Matrix &&other)
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.rows_ = 0, other.cols_ = 0;
  other.matrix_ = nullptr;
}
/**
 * @brief деструктор
 * @note group: destructor
 */
S21Matrix::~S21Matrix() { S21RemoveMatrix(); }

// methods:
// *--------------------------------------------------------------------------*
// *--------------------------------------------------------------------------*

/**
 * @brief Преобразование матрицы в единичкую
 */
void S21Matrix::SetIdentity() {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      if (i == j) {
        matrix_[i][j] = 1;
      } else {
        matrix_[i][j] = 0;
      }
    }
  }
}

/**
 * @brief Метод для сравнения двух матриц
 * @return true - матрицы равны; false - матрицы не равны/разная
 * размерность/размерность меньше или равна 0
 * @note group: method
 */
bool S21Matrix::EqMatrix(const S21Matrix &other) const {
  bool result_value = true;
  if (cols_ != other.cols_ || rows_ != other.rows_ || rows_ <= 0 ||
      other.rows_ <= 0 || cols_ <= 0 || other.rows_ <= 0)
    result_value = false;
  for (int i = 0; i != other.rows_ && result_value; i++) {
    for (int j = 0; j != other.cols_ && result_value; j++) {
      if (!S21CompareDoubles(matrix_[i][j], other.matrix_[i][j]))
        result_value = false;
    }
  }
  return result_value;
}

/**
 * @brief Метод для суммы other с текущей, меняет текущую матрицу
 * @note group: method
 */
void S21Matrix::SumMatrix(const S21Matrix &other) {
  S21ExceptionDoesNotExist(other);
  S21ExceptionDifferentMatrixDemension(other);
  for (int i = 0; i != rows_; i++) {
    for (int j = 0; j != cols_; j++) {
      matrix_[i][j] = matrix_[i][j] + other.matrix_[i][j];
    }
  }
}

/**
 * @brief Метод для разницы other с текущей, меняет текущую матрицу
 * @note group: method
 */
void S21Matrix::SubMatrix(const S21Matrix &other) {
  S21ExceptionDoesNotExist(other);
  S21ExceptionDifferentMatrixDemension(other);
  for (int i = 0; i != rows_; i++) {
    for (int j = 0; j != cols_; j++) {
      matrix_[i][j] = matrix_[i][j] - other.matrix_[i][j];
    }
  }
}

/**
 * @brief Метод для умножения текущей матрицы на число
 * @note group: method
 */
void S21Matrix::MulNumber(const double num) {
  S21ExceptionDoesNotExist();
  for (int i = 0; i != rows_; i++) {
    for (int j = 0; j != cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

/**
 * @brief Метод для перемножения двух матриц. Если умножается матрица на вектор,
 * то матрица - это this, вектор - other. Правило: this.cols == this.rows
 * @return в this записывается матрица размерностью [cols][other.rows]
 * @note group: method
 */
void S21Matrix::MulMatrix(const S21Matrix &other) {
  S21ExceptionDoesNotExist(other);
  S21ExceptionDifferentDemensionMul(other);
  S21Matrix temp(rows_, other.cols_);
  double *temp_row = new double[cols_]();
  double *temp_col = new double[other.rows_]();
  for (int row = 0; row != rows_; row++) {
    S21RowCopy(*this, temp_row, row);
    for (int column = 0; column != other.cols_; column++) {
      S21ColCopy(other, temp_col, column);
      temp.matrix_[row][column] = S21RowColMul(temp_row, temp_col, cols_);
    }
  }
  *this = std::move(temp);
  delete[] temp_col;
  delete[] temp_row;
}

/**
 * @brief Метод для транспонирования текущей матрицы
 * @return Транспонированную матрицу
 * @note group: method
 */
S21Matrix S21Matrix::Transpose(void) {
  S21ExceptionDoesNotExist();
  S21Matrix result(cols_, rows_);
  S21TransposeAdditional(result);
  return result;
}

/**
 * @brief Расчитывает матрицу алгебраических дополнений
 * @return возвращает новую матрицу алг. доп.
 * @note group: method
 */
S21Matrix S21Matrix::CalcComplements(void) {
  S21ExceptionDoesNotExist();
  S21ExceptionIsNotSquare();
  S21Matrix result(rows_, cols_);
  S21CalcComplAdditional(result);
  return result;
}

/**
 * @brief Расчитывает определитель матрицы
 * @return возвращает double в виде значения определителя матрицы
 * @note group: method
 */
double S21Matrix::Determinant(void) {
  S21ExceptionDoesNotExist();
  S21ExceptionIsNotSquare();
  double result = 0.0;
  result = S21CalcDeterminant(rows_);
  return result;
}

/**
 * @brief Расчитывает обратную матрицу
 * @return Возврает новую матрицу, обратную
 * @note group: method
 */
S21Matrix S21Matrix::InverseMatrix() {
  S21ExceptionDoesNotExist();
  double inverse_det = 0.0;
  S21ExceptionDetIsZero();
  S21Matrix result(rows_, cols_);
  S21CalcComplAdditional(result);
  S21Matrix result_copy(result);
  inverse_det = 1.0 / S21CalcDeterminant(rows_);
  result_copy.S21TransposeAdditional(result);
  for (int i = 0; i != rows_; i++) {
    for (int j = 0; j != cols_; j++) {
      result.matrix_[i][j] *= inverse_det;
    }
  }
  return result;
}

// operators:
// *--------------------------------------------------------------------------*
// *--------------------------------------------------------------------------*

/**
 * @brief Оператор сложения
 * @return Возврает новую матрицу, результат суммы
 * @note group: operator
 */
S21Matrix S21Matrix::operator+(const S21Matrix &other) const {
  S21ExceptionDoesNotExist(other);
  S21ExceptionDifferentMatrixDemension(other);
  S21Matrix result(*this);
  result.SumMatrix(other);
  return result;
}

/**
 * @brief Оператор разницы
 * @return Возврает новую матрицу, результат разницы
 * @note group: operator
 */
S21Matrix S21Matrix::operator-(const S21Matrix &other) const {
  S21ExceptionDoesNotExist(other);
  S21ExceptionDifferentMatrixDemension(other);
  S21Matrix result(*this);
  result.SubMatrix(other);
  return result;
}

/**
 * @brief Оператор перемножения двух матриц. Если умножается матрица на вектор,
 * то матрица - это this, вектор - other. Правило: this.cols == this.rows
 * @return Возврает новую матрицу, результат умножения, размерность:
 * [rows][other.cols]
 * @note group: operator
 */
S21Matrix S21Matrix::operator*(const S21Matrix &other) const {
  S21ExceptionDoesNotExist(other);
  S21ExceptionDifferentDemensionMul(other);
  S21Matrix result(*this);
  result.MulMatrix(other);
  return result;
}

/**
 * @brief Оператор умножения числа на матрицу
 * @return Возврает новую матрицу, результат умножения
 * @note group: operator
 */
S21Matrix operator*(const double &num, const S21Matrix &matrix) {
  matrix.S21ExceptionDoesNotExist();
  S21Matrix result(matrix);
  result.MulNumber(num);
  return result;
}

/**
 * @brief Оператор умножения матрицу на число
 * @return Возврает новую матрицу, результат умножения
 * @note group: operator
 */
S21Matrix operator*(const S21Matrix &matrix, const double &num) {
  matrix.S21ExceptionDoesNotExist();
  return num * matrix;
}

/**
 * @brief Оператор сравнения
 * @return Возврает результат сравнения, bool (true/false)
 * @note group: operator
 */
bool S21Matrix::operator==(const S21Matrix &other) {
  S21ExceptionDoesNotExist(other);
  bool result = EqMatrix(other);
  return result;
}

/**
 * @brief Оператор копирования
 * @return Возвращает матрицу, результат копирования
 * @note group: operator
 */
S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  if (this != &other) {
    S21RemoveMatrix();
    rows_ = other.rows_;
    cols_ = other.cols_;
    S21CreateMatrix();
    S21CopyMatrix(other);
  }
  return *this;
}

/**
 * @brief Оператор присвоения суммы
 * @return Возврает ссылку на результат суммы
 * @note group: operator
 */
S21Matrix &S21Matrix::operator+=(const S21Matrix &other) {
  S21ExceptionDoesNotExist(other);
  S21ExceptionDifferentMatrixDemension(other);
  SumMatrix(other);
  return *this;
}

/**
 * @brief Оператор присвоения разницы
 * @return Возврает ссылку на результат разницы
 * @note group: operator
 */
S21Matrix &S21Matrix::operator-=(const S21Matrix &other) {
  S21ExceptionDoesNotExist(other);
  S21ExceptionDifferentMatrixDemension(other);
  SubMatrix(other);
  return *this;
}

/**
 * @brief Оператор присвоения умножения матрицы на матрицу. Если умножается
 * матрица на вектор, то матрица - это this, вектор - other. Правило: this.cols
 * == this.rows
 * @return Возврает ссылку на результат перемножения двух матриц, размерность:
 * [rows][other.cols]
 * @note group: operator
 */
S21Matrix &S21Matrix::operator*=(const S21Matrix &other) {
  S21ExceptionDoesNotExist(other);
  S21ExceptionDifferentDemensionMul(other);
  MulMatrix(other);
  return *this;
}

/**
 * @brief Оператор присвоения умножения матрицы на число
 * @return Возврает ссылку на результат умножения
 * @note group: operator
 */
S21Matrix &S21Matrix::operator*=(const double &num) {
  S21ExceptionDoesNotExist();
  MulNumber(num);
  return *this;
}

/**
 * @brief Оператор индексации по элементам матрицы (row, column).
 * Example: matrix(i, j) = (double)num;
 * @return Возврает ссылку на значение матрицы по индексам
 * @note group: operator
 */
double &S21Matrix::operator()(int i, int j) {
  S21ExceptionOutOfRange(i, j);
  return matrix_[i][j];
}

/**
 * @brief Оператор индексации по элементам матрицы (row, column).
 * Expamle:(double)num = matrix(i, j);
 * @return Возврает ссылку на результат суммы
 * @note group: operator
 */
const double &S21Matrix::operator()(int i, int j) const {
  S21ExceptionOutOfRange(i, j);
  return matrix_[i][j];
}

/**
 * @brief Оператор перемещения
 * @return Возврает возвращает перемещенную матрицу
 * @note group: operator
 */
S21Matrix S21Matrix::operator=(S21Matrix &&other) noexcept {
  if (this != &other) {
    S21RemoveMatrix();
    rows_ = other.rows_;
    cols_ = other.cols_;
    matrix_ = other.matrix_;
    other.S21Nulling();
  }
  return *this;
}

// accessors:
// *--------------------------------------------------------------------------*
// *--------------------------------------------------------------------------*

/**
 * @brief Получает количество строк матрицы
 * @note group: accessors
 */
int S21Matrix::GetRows(void) const { return rows_; }

/**
 * @brief Получает количество столбцов матрицы
 * @note group: accessors
 */
int S21Matrix::GetCols(void) const { return cols_; }

// mutators:
// *--------------------------------------------------------------------------*
// *--------------------------------------------------------------------------*

/**
 * @brief Меняет количество строк матрицы;
 * Если их больше текущего количества, то копирует текущие, новые зануляет
 * @note group: mutators
 */
void S21Matrix::SetRows(int rows) {
  if (rows < 0) throw std::length_error("value rows is less 0");
  S21Matrix temp(rows, cols_);
  for (int i = 0; i != ((rows < rows_) ? rows : rows_); i++) {
    for (int j = 0; j != cols_; j++) {
      temp.matrix_[i][j] = matrix_[i][j];
    }
  }
  *this = std::move(temp);
}

/**
 * @brief Меняет количество столбцов матрицы;
 * Если их больше текущего количества, то копирует текущие, новые зануляет
 * @note group: mutators
 */
void S21Matrix::SetCols(int cols) {
  if (cols < 0) throw std::length_error("value cols is less 0");
  S21Matrix temp(rows_, cols);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < ((cols < cols_) ? cols : cols_); j++) {
      temp.matrix_[i][j] = matrix_[i][j];
    }
  }
  *this = std::move(temp);
}