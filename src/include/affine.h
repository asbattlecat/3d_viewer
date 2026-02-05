#ifndef AFFINE_H
#define AFFINE_H

#include "./matrix/s21_matrix_oop.h"
#include "include_common.h"

namespace s21 {

/**
 * @brief Данный файл содержит в себе реализацию паттерна "Декоратор", который
 * помогает адаптировать S21Matrix под нашу задачу, а именно для аффинных
 * преобразований
 */

// ____________________________________________________________________________
// простенькая структура для вектора и операций, связанных с ним
struct Vector3D {
  double x, y, z;
  Vector3D(double n_x = 0.0, double n_y = 0.0, double n_z = 0.0);
  Vector3D(const Vector3D &other);
  Vector3D(Vector3D &&other) noexcept;
  Vector3D &operator=(const Vector3D &other);
  Vector3D &operator=(Vector3D &&other) noexcept;
  Vector3D operator-(const Vector3D &other) const;
  Vector3D operator+(const Vector3D &other) const;
  Vector3D operator*(double num) const;

  Vector3D cross(const Vector3D &other) const;
  double length() const;
  Vector3D normalize() const;
  double scalar(const Vector3D &other) const;
};

// ____________________________________________________________________________
// компонент (интерфейс):
class IMatrix {
 public:
  // basic methods
  virtual void MulMatrix(const S21Matrix &other) = 0;
  virtual ~IMatrix() = default;
  virtual void SetIdentity() = 0;
  virtual double &operator()(int i, int j) = 0;
  virtual const double &operator()(int i, int j) const = 0;
  virtual IMatrix &operator=(const S21Matrix &other) = 0;
  virtual S21Matrix &operator*=(const S21Matrix &other) = 0;
  virtual S21Matrix operator*(const S21Matrix &other) const = 0;

  // getter:
  virtual int GetRows() const = 0;
  virtual int GetCols() const = 0;
  virtual S21Matrix GetMatrix() const = 0;
  virtual std::vector<float> GetData()
      const = 0;  // getter для передачи данных в OpenGL
};

// ____________________________________________________________________________
// обертка для S21Matrix
class S21MatrixWrapper : public IMatrix {
 private:
  S21Matrix matrix_;

 public:
  // constructors:
  S21MatrixWrapper(const S21Matrix &other);
  S21MatrixWrapper(int rows, int cols);

  void SetIdentity() override;

  // basic methods:
  void MulMatrix(const S21Matrix &other) override;
  double &operator()(int i, int j) override;
  const double &operator()(int i, int j) const override;
  S21MatrixWrapper &operator=(const S21Matrix &other) override;
  S21Matrix &operator*=(const S21Matrix &other) override;
  S21Matrix operator*(const S21Matrix &other) const override;

  // getters:
  int GetRows() const override;
  int GetCols() const override;
  S21Matrix GetMatrix() const override;
  std::vector<float> GetData() const override;
};

// ____________________________________________________________________________
// базовый декоратор S21Matrix
class MatrixDecorator : public IMatrix {
 protected:
  std::unique_ptr<IMatrix> matrix_;

 public:
  MatrixDecorator(std::unique_ptr<IMatrix> matrix);
  ~MatrixDecorator() override;

  void SetIdentity() override;

  void MulMatrix(const S21Matrix &other) override;
  double &operator()(int i, int j) override;
  const double &operator()(int i, int j) const override;
  MatrixDecorator &operator=(const S21Matrix &other) override;
  S21Matrix &operator*=(const S21Matrix &other) override;
  S21Matrix operator*(const S21Matrix &other) const override;

  int GetRows() const override;
  int GetCols() const override;
  S21Matrix GetMatrix() const override;
  std::vector<float> GetData() const override;
};

// ____________________________________________________________________________
/**
 * @brief Класс под аффинные пробразования, созданный на основе структурного
 * паттерна "Декторатор". Этот класс поддерживает возможность создания трех
 * матриц, которые вместе преобразуются в матрицу MVP (матрица проекции),
 * которая, в свою очередь, будет передана OpenGL, в шейдер.
 * @details Методы для ModelMatrix: Translation, RotationLocal, Scale. Метод
 * CreateViewMatrix создает, как видно из названия, ViewMatrix.
 */
class AffineTransDecorator : public MatrixDecorator {
 public:
  AffineTransDecorator(std::unique_ptr<IMatrix> matrix);
  AffineTransDecorator &operator=(const S21Matrix &other) override;

  // modelMatrix - матрица в виде результата перемещения, вращения,
  // масштабирования

  // viewMatrix - матрица для камеры (поворочиваем мир таким образом, чтобы
  // камера оказалась в центре мировых координат)

  // ProjectionMatrix - мы определенным образом проецируем модель на плоскость
  // проецирования

  // в OpenGL мы передаем матрицу в виде результата перемножения Projection *
  // View
  // * Model

  void Translation(double tx, double ty, double tz);
  void Rotation(double angle, Vector3D &axis);
  void Scale(double sx, double sy, double sz);
  S21Matrix CreateViewMatrix(const Vector3D &eye, const Vector3D &target,
                             const Vector3D &up);
  S21Matrix CreateProjMatrixPersp(double fov, double aspect, double near,
                                  double far);
  S21Matrix CreateProjMatrixOrth(double left, double right, double bottom,
                                 double top, double near, double far);
  S21Matrix CreateMVPMatrix(const S21Matrix &projection, const S21Matrix &view,
                            const S21Matrix &model);

 private:
  void create_rotation_mat_(S21Matrix &rot_matrix, double t, double cosine,
                            double sine, Vector3D &n_a);
  double angle_to_radiane_(const double &angle) const;
  void copy_matrix_(const S21Matrix &other);
  void exception_check_matrix_size_(const std::string &method_name) const;
  void exception_check_vector_length_(const std::string &method_name,
                                      const Vector3D &vec) const;
  void exception_check_same_size_(const std::string &method_name,
                                  const S21Matrix &other) const;
  void orthogonalize_matrix_();
};

}  // namespace s21

#endif