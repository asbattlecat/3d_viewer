#ifndef CAMERA_H
#define CAMERA_H

#include "affine.h"
#include "include_common.h"

namespace s21 {

class Camera {
 private:
  Vector3D target_ = {0, 0, 0};
  Vector3D eye_ = {5, 5, -8};
  Vector3D up_ = {0, 1, 0};
  S21Matrix view_matrix_{4, 4};

 public:
  Camera();

  // мутатор
  void UpdateCamera(const Vector3D &target, const Vector3D &eye,
                    const Vector3D &up);

  // геттер
  S21Matrix GetViewMatrix() const;

 private:
};

}  // namespace s21

#endif