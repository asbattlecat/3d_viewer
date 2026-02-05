#include "./../include/camera.h"

namespace s21 {

Camera::Camera() { view_matrix_.SetIdentity(); }

/**
 * @brief Метод для задачи новых параметров
 */
void Camera::UpdateCamera(const Vector3D &target, const Vector3D &eye,
                          const Vector3D &up) {
  target_ = target;
  eye_ = eye;
  up_ = up;
  AffineTransDecorator camera(std::make_unique<S21MatrixWrapper>(4, 4));
  view_matrix_ = camera.CreateViewMatrix(eye_, target_, up_);
}

S21Matrix Camera::GetViewMatrix() const { return view_matrix_; }

}  // namespace s21