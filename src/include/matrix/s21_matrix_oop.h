#ifndef S21_MATRIX_H
#define S21_MATRIX_H

#include <cmath>
#include <iostream>
#include <stdexcept>
#define EPSILON 9e-8

class S21Matrix {
 private:
  // Atributes:
  int rows_, cols_;
  double **matrix_;

  // methods:
  void S21CreateMatrix();
  void S21RemoveMatrix();
  void S21Nulling();
  void S21CopyMatrix(const S21Matrix &other);
  bool S21CompareDoubles(const double &first, const double &second) const;
  void S21RowCopy(const S21Matrix &matrix, double *temp_row, const int &row);
  void S21ColCopy(const S21Matrix &matrix, double *temp_col, const int &col);
  double S21RowColMul(const double *temp_row, const double *temp_col,
                      const int col_size);
  void S21TransposeAdditional(const S21Matrix &other);
  void S21GetCofactor(S21Matrix &temp, int row_income, int column_income,
                      int size) const;
  double S21CalcDeterminant(int size) const;
  void S21CalcComplAdditional(S21Matrix &result);

  //    exceptions:

  void S21ExceptionDifferentMatrixDemension(const S21Matrix &other) const;
  void S21ExceptionDoesNotExist(const S21Matrix &other) const;
  void S21ExceptionDoesNotExist(void) const;
  void S21ExceptionDifferentDemensionMul(const S21Matrix &other) const;
  void S21ExceptionIsNotSquare(void) const;
  void S21ExceptionDetIsZero(void) const;
  void S21ExceptionOutOfRange(const int &a, const int &b) const;

 public:
  // constructors

  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix &other);  // copying
  S21Matrix(S21Matrix &&other);

  // destructor

  ~S21Matrix();

  // methods

  void SetIdentity();
  bool EqMatrix(const S21Matrix &other) const;  // compare matrices
  void SumMatrix(const S21Matrix &other);       // sum matrices
  void SubMatrix(const S21Matrix &other);       // sub matrices
  void MulNumber(const double num);             // mul current matrix to number
  void MulMatrix(const S21Matrix &other);       // mul corrent matrix to other
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  double Determinant();
  S21Matrix InverseMatrix();

  // operators:

  S21Matrix operator+(const S21Matrix &other) const;
  S21Matrix operator-(const S21Matrix &other) const;
  S21Matrix operator*(const S21Matrix &other) const;
  friend S21Matrix operator*(const double &num, const S21Matrix &matrix);
  friend S21Matrix operator*(const S21Matrix &matrix, const double &num);
  bool operator==(const S21Matrix &other);
  S21Matrix &operator=(const S21Matrix &other);  // оператор копирования
  S21Matrix &operator+=(const S21Matrix &other);
  S21Matrix &operator-=(const S21Matrix &other);
  S21Matrix &operator*=(const S21Matrix &other);
  S21Matrix &operator*=(const double &num);
  S21Matrix operator=(S21Matrix &&other) noexcept;  // оператор переноса
  double &operator()(int i, int j);
  const double &operator()(int i, int j) const;

  // accessors:

  int GetRows() const;
  int GetCols() const;

  // mutators:

  void SetRows(int rows);
  void SetCols(int cols);
};

#endif