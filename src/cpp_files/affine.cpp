#include "./../include/affine.h"

namespace s21 {
// ____________________________________________________________________________
// реализация Vector3D:

Vector3D::Vector3D(double n_x, double n_y, double n_z)
    : x(n_x), y(n_y), z(n_z) {}

Vector3D::Vector3D(const Vector3D &other)
    : x(other.x), y(other.y), z(other.z) {}

Vector3D::Vector3D(Vector3D &&other) noexcept
    : x(other.x), y(other.y), z(other.z) {
  other.x = 0.0;
  other.y = 0.0;
  other.z = 0.0;
}

Vector3D &Vector3D::operator=(const Vector3D &other) {
  if (this != &other) {
    x = other.x;
    y = other.y;
    z = other.z;
  }
  return *this;
}

Vector3D &Vector3D::operator=(Vector3D &&other) noexcept {
  if (this != &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    other.x = 0.0;
    other.y = 0.0;
    other.z = 0.0;
  }
  return *this;
}

Vector3D Vector3D::operator-(const Vector3D &other) const {
  return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator+(const Vector3D &other) const {
  return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator*(double num) const {
  return Vector3D(x * num, y * num, z * num);
}

/**
 * @brief Метод для векторного умножения двух векторов
 * @return Возвращается результат векторного умножения в виде нового вектора
 */
Vector3D Vector3D::cross(const Vector3D &other) const {
  return Vector3D(y * other.z - z * other.y, z * other.x - x * other.z,
                  x * other.y - y * other.x);
}

/**
 * @brief метод для определения длины вектора
 */
double Vector3D::length() const { return std::sqrt(x * x + y * y + z * z); }

Vector3D Vector3D::normalize() const {
  double len = length();
  if (len == 0.0) {
    throw std::invalid_argument(
        "Vector3D::normalize(): length of the vector is 0.0!");
  }
  return Vector3D(x / len, y / len, z / len);
}

double Vector3D::scalar(const Vector3D &other) const {
  return (x * other.x + y * other.y + z * other.z);
}

// ____________________________________________________________________________
// реализация S21MatrixWrapper:

S21MatrixWrapper::S21MatrixWrapper(const S21Matrix &other) : matrix_(other) {}

S21MatrixWrapper::S21MatrixWrapper(int rows, int cols) : matrix_(rows, cols) {}

void S21MatrixWrapper::SetIdentity() { matrix_.SetIdentity(); }

void S21MatrixWrapper::MulMatrix(const S21Matrix &other) {
  matrix_.MulMatrix(other);
}

double &S21MatrixWrapper::operator()(int i, int j) { return matrix_(i, j); }

const double &S21MatrixWrapper::operator()(int i, int j) const {
  return matrix_(i, j);
}

S21MatrixWrapper &S21MatrixWrapper::operator=(const S21Matrix &other) {
  matrix_ = other;
  return *this;
}

S21Matrix &S21MatrixWrapper::operator*=(const S21Matrix &other) {
  return matrix_.operator*=(other);
}

S21Matrix S21MatrixWrapper::operator*(const S21Matrix &other) const {
  return matrix_.operator*(other);
}

int S21MatrixWrapper::GetRows() const { return matrix_.GetRows(); }

int S21MatrixWrapper::GetCols() const { return matrix_.GetCols(); }

S21Matrix S21MatrixWrapper::GetMatrix() const { return matrix_; }

std::vector<float> S21MatrixWrapper::GetData() const {
  std::vector<float> data;
  data.reserve(16);
  for (int col = 0; col < matrix_.GetCols(); ++col) {
    for (int row = 0; row < matrix_.GetRows(); ++row) {
      data[col * 4 + row] = static_cast<float>(matrix_(row, col));
    }
  }
  return data;
}

// ____________________________________________________________________________
// реализация MatrixDecorator:

MatrixDecorator::MatrixDecorator(std::unique_ptr<IMatrix> matrix)
    : matrix_(std::move(matrix)) {
  if (!matrix_) {
    throw std::invalid_argument("MatrixDecorator constr: matrix_ is nullptr!");
  }
}

MatrixDecorator::~MatrixDecorator() = default;

void MatrixDecorator::MulMatrix(const S21Matrix &other) {
  if (matrix_->GetCols() != 4 || matrix_->GetRows() != 4 ||
      other.GetCols() != 4 || other.GetRows() != 4) {
    throw std::invalid_argument(
        "MatrixDecorator::MulMatrix: Matrix size must be 4 x 4");
  }
  return matrix_->MulMatrix(other);
}

void MatrixDecorator::SetIdentity() { matrix_->SetIdentity(); }

double &MatrixDecorator::operator()(int i, int j) { return (*matrix_)(i, j); }

const double &MatrixDecorator::operator()(int i, int j) const {
  return (*matrix_)(i, j);
}

MatrixDecorator &MatrixDecorator::operator=(const S21Matrix &other) {
  matrix_->operator=(other);
  return *this;
}

S21Matrix &MatrixDecorator::operator*=(const S21Matrix &other) {
  return matrix_->operator*=(other);
}

S21Matrix MatrixDecorator::operator*(const S21Matrix &other) const {
  return matrix_->operator*(other);
}

int MatrixDecorator::GetRows() const { return matrix_->GetRows(); }

int MatrixDecorator::GetCols() const { return matrix_->GetCols(); }

S21Matrix MatrixDecorator::GetMatrix() const { return matrix_->GetMatrix(); }

std::vector<float> MatrixDecorator::GetData() const {
  return matrix_->GetData();
}

// ____________________________________________________________________________
// реализация AffineTransDecorator:

AffineTransDecorator::AffineTransDecorator(std::unique_ptr<IMatrix> matrix)
    : MatrixDecorator(std::move(matrix)) {}

AffineTransDecorator &AffineTransDecorator::operator=(const S21Matrix &other) {
  exception_check_same_size_("operator=()", other);
  MatrixDecorator::operator=(other);
  return *this;
}

/**
 * @brief операция перемещения
 * @param tx, ty, tz это значение, на которое перемещается фигура
 */
void AffineTransDecorator::Translation(double tx, double ty, double tz) {
  exception_check_matrix_size_("Translation()");
  // заполнение матрицы перемещения
  S21Matrix t_matrix(4, 4);
  t_matrix.SetIdentity();
  t_matrix(0, 3) = tx, t_matrix(1, 3) = ty, t_matrix(2, 3) = tz;

  // создание ModelMatrix
  matrix_->MulMatrix(t_matrix);
}

/**
 * @brief операция вращения фигуры относительно локальной оси
 * @param angle - угол, на который вращается фигура;
 * @param axis - ось, вокруг которой
 * вращается фигура (глобальная)
 */
void AffineTransDecorator::Rotation(double angle, Vector3D &axis) {
  exception_check_matrix_size_("Rotation()");
  exception_check_vector_length_("Rotation", axis);
  Vector3D n_a = axis.normalize();  // normalized axis
  float radian = angle_to_radiane_(angle);
  float cosine = std::cos(radian), sine = std::sin(radian);
  float t = 1 - cosine;
  S21Matrix rot_matrix(4, 4);
  rot_matrix.SetIdentity();

  // создание матрицы поворота по формуле Родригеса
  create_rotation_mat_(rot_matrix, t, cosine, sine, n_a);

  // создание ModelMatrix
  matrix_->MulMatrix(rot_matrix);
}

void AffineTransDecorator::Scale(double sx, double sy, double sz) {
  exception_check_matrix_size_("Scale");
  S21Matrix scale_mat(4, 4);
  scale_mat.SetIdentity();

  // заполенение матрицы масштабирования
  scale_mat(0, 0) = sx, scale_mat(1, 1) = sy;
  scale_mat(2, 2) = sz;

  // создание ModelMatrix
  matrix_->MulMatrix(scale_mat);
}

/**
 * @brief Метод для создания VeiwMatrix. Это одна из трех матриц,
 * которая составляет MVP (для передачи в OpenGL). Матрица записывается в this
 * @param eye это положение точки "взгляда";
 * @param target куда смотрим;
 * @param up направление "вверх" камеры
 * @name f - вектор forward; r - вектор right; u - ветор real up
 * @return матрица ViewMatrix размером 4x4
 */
S21Matrix AffineTransDecorator::CreateViewMatrix(const Vector3D &eye,
                                                 const Vector3D &target,
                                                 const Vector3D &up) {
  exception_check_matrix_size_("CreateViewMatrix");
  Vector3D f = (target - eye).normalize();
  Vector3D r = f.cross(up).normalize();
  Vector3D u = f.cross(r);

  S21Matrix view(4, 4);
  view.SetIdentity();
  // заполнение матрицы ViewMatrix
  view(0, 0) = r.x, view(0, 1) = r.y, view(0, 2) = r.z;
  view(1, 0) = u.x, view(1, 1) = u.y, view(1, 2) = u.z;
  view(2, 0) = -f.x, view(2, 1) = -f.y, view(2, 2) = -f.z;

  // это результат перемножения матрицы на вектор (3х3 часть view и eye)
  view(0, 3) = -(eye.scalar(r));
  view(1, 3) = -(eye.scalar(u));
  view(2, 3) = eye.scalar(f);

  return view;
}

/**
 * @brief Метод для создания матрицы проекцирование. Проецирование -
 * перспективное
 * @return Матрица ProjectionMatrix, размер 4х4
 */
S21Matrix AffineTransDecorator::CreateProjMatrixPersp(double fov, double aspect,
                                                      double near, double far) {
  if (fov <= 0 || fov >= 180) {
    throw std::invalid_argument(
        "CreateProjMatrixPersp: FOV must be in (0, 180)");
  }
  if (aspect <= 0) {
    throw std::invalid_argument(
        "CreateProjMatrixPersp: aspect must be more than 0");
  }
  if (near <= 0 || far <= near) {
    throw std::invalid_argument("CreateProjMatrixPersp: invalid near or far");
  }
  exception_check_matrix_size_("CreateProjMatrixPersp");

  S21Matrix proj(4, 4);
  proj.SetIdentity();

  double half_tan_fov = std::tan(angle_to_radiane_(fov) / 2);

  proj(0, 0) = 1.0 / (aspect * half_tan_fov);
  proj(1, 1) = 1.0 / half_tan_fov;
  proj(2, 2) = -(far + near) / (far - near);
  proj(2, 3) = -2.0 * far * near / (far - near);
  proj(3, 2) = -1.0;
  proj(3, 3) = 0.0;

  return proj;
}

/**
 * @brief Метод для создания матрицы проецирования. Проецирование - параллельное
 * @return матрица ProjectionMatrix, размер 4х4
 */
S21Matrix AffineTransDecorator::CreateProjMatrixOrth(double left, double right,
                                                     double bottom, double top,
                                                     double near, double far) {
  if (right <= left || top <= bottom || far <= near) {
    throw std::invalid_argument(
        "CreateProjMatrixOrth: invalid projection parameters!");
  }

  S21Matrix proj(4, 4);
  proj.SetIdentity();

  proj(0, 0) = 2.0 / (right - left);
  proj(1, 1) = 2.0 / (top - bottom);
  proj(2, 2) = -2.0 / (far - near);
  proj(0, 3) = -(right + left) / (right - left);
  proj(1, 3) = -(top + bottom) / (top - bottom);
  proj(2, 3) = -(far + near) / (far - near);

  return proj;
}

/**
 * @brief создает MVP матрицу, способ получения: projection * view * model
 * @return результат перемножения матриц, размер 4х4
 */
S21Matrix AffineTransDecorator::CreateMVPMatrix(const S21Matrix &projection,
                                                const S21Matrix &view,
                                                const S21Matrix &model) {
  exception_check_matrix_size_("CreateMVPMatrix");
  exception_check_same_size_("CreateMVPMatrix", projection);
  exception_check_same_size_("CreateMVPMatrix", view);
  exception_check_same_size_("CreateMVPMatrix", model);
  S21Matrix mv = view * model;
  S21Matrix result = projection * mv;
  return result;
}

// создание матрицы поворота по формуле Родригеса
void AffineTransDecorator::create_rotation_mat_(S21Matrix &rot_matrix, double t,
                                                double cosine, double sine,
                                                Vector3D &n_a) {
  rot_matrix(0, 0) = t * n_a.x * n_a.x + cosine;
  rot_matrix(0, 1) = t * n_a.x * n_a.y - sine * n_a.z;
  rot_matrix(0, 2) = t * n_a.x * n_a.z + sine * n_a.y;
  rot_matrix(1, 0) = t * n_a.x * n_a.y + sine * n_a.z;
  rot_matrix(1, 1) = t * n_a.y * n_a.y + cosine;
  rot_matrix(1, 2) = t * n_a.y * n_a.z - sine * n_a.x;
  rot_matrix(2, 0) = t * n_a.x * n_a.z - sine * n_a.y;
  rot_matrix(2, 1) = t * n_a.y * n_a.z + sine * n_a.x;
  rot_matrix(2, 2) = t * n_a.z * n_a.z + cosine;
}

double AffineTransDecorator::angle_to_radiane_(const double &angle) const {
  return angle * M_PI / 180;
}

void AffineTransDecorator::copy_matrix_(const S21Matrix &other) {
  exception_check_same_size_("copy_matrix_", other);
  for (int i = 0; i < GetRows(); ++i) {
    for (int j = 0; j < GetCols(); ++j) {
      (*matrix_)(i, j) = other(i, j);
    }
  }
}

/**
 * @brief Проверка матрицы на то, что ее размер соответствует 4х4
 */
void AffineTransDecorator::exception_check_matrix_size_(
    const std::string &method_name) const {
  if (matrix_->GetCols() != 4 || matrix_->GetRows() != 4) {
    throw std::invalid_argument(method_name + ": Matrix size must be 4 x 4!");
  }
}

/**
 * @brief Проверка на то, что длина вектора не равна 0.0
 */
void AffineTransDecorator::exception_check_vector_length_(
    const std::string &method_name, const Vector3D &vec) const {
  if (vec.length() == 0.0) {
    throw std::invalid_argument(method_name +
                                ": Vector length must be more than 0.0!");
  }
}

/**
 * @brief Проверка на то, что у матриц одинаковые размеры
 */
void AffineTransDecorator::exception_check_same_size_(
    const std::string &method_name, const S21Matrix &other) const {
  if (GetCols() != other.GetCols() || GetRows() != other.GetRows()) {
    throw std::invalid_argument(method_name +
                                ": Matrix sizes must be the same");
  }
}

}  // namespace s21