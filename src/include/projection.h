#ifndef PROJECTION_H
#define PROJECTION_H

#include "affine.h"
#include "include_common.h"

namespace s21 {

struct ProjPerspectiveData {
  double fov_ = 90.0;
  double aspect_ = 4.0 / 3.0;
};

struct ProjOrthogonalData {
  double left_ = -16.0;
  double right_ = 16.0;
  double bottom_ = -12.0;
  double top_ = 12.0;
};

class Projection {
 private:
  ProjPerspectiveData persp_pr_;
  ProjOrthogonalData orth_pr_;
  double near_ = 0.01, far_ = 15.0;

  S21Matrix persp_projection_matrix_;
  S21Matrix orth_projection_matrix_;
  bool is_perspective_ = false;

 public:
  // конструктор базовый
  Projection();
  Projection(const Projection &other);

  Projection &operator=(const Projection &other);

  // мутаторы
  void UpdatePerspective(double fov, double aspect, double near, double far);
  void UpdateOrthogonal(double left, double right, double bottom, double top,
                        double near, double far);
  void SetPerspective();
  void SetOrthogonal();
  void SetFar(const double far);
  void SetNear(const double near);
  void SetPerspProjectionMatrix(const S21Matrix &other);
  void SetOrthProjectionMatrix(const S21Matrix &other);

  // геттеры
  S21Matrix GetPerspProjMatrix();
  S21Matrix GetOrthProjMatrix();
  bool IsPerspectiveProjection();
  double GetNear() const;
  double GetFar() const;
};

}  // namespace s21

#endif