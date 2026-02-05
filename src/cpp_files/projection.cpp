#include "./../include/projection.h"

namespace s21 {

Projection::Projection() {
  persp_projection_matrix_.SetIdentity();
  orth_projection_matrix_.SetIdentity();
}

Projection::Projection(const Projection &other) {
  SetFar(other.far_);
  persp_projection_matrix_ = other.persp_projection_matrix_;
  orth_projection_matrix_ = other.orth_projection_matrix_;
  is_perspective_ = other.is_perspective_;
}

Projection &Projection::operator=(const Projection &other) {
  if (this != &other) {
    persp_pr_ = other.persp_pr_;
    orth_pr_ = other.orth_pr_;
    near_ = other.near_;
    far_ = other.far_;
    persp_projection_matrix_ = other.persp_projection_matrix_;
    orth_projection_matrix_ = other.orth_projection_matrix_;
    is_perspective_ = other.is_perspective_;
  }
  return *this;
}

void Projection::UpdatePerspective(double fov, double aspect, double near,
                                   double far) {
  persp_pr_.fov_ = fov;
  persp_pr_.aspect_ = aspect;
  near_ = near;
  far_ = far;
  AffineTransDecorator projection_matrix(
      std::make_unique<S21MatrixWrapper>(4, 4));
  persp_projection_matrix_ =
      projection_matrix.CreateProjMatrixPersp(fov, aspect, near, far);
}

void Projection::UpdateOrthogonal(double left, double right, double bottom,
                                  double top, double near, double far) {
  orth_pr_.left_ = left;
  orth_pr_.right_ = right;
  orth_pr_.bottom_ = bottom;
  orth_pr_.top_ = top;
  near_ = near;
  far_ = far;
  AffineTransDecorator projection_matrix(
      std::make_unique<S21MatrixWrapper>(4, 4));
  orth_projection_matrix_ = projection_matrix.CreateProjMatrixOrth(
      left, right, bottom, top, near, far);
}

void Projection::SetOrthogonal() { is_perspective_ = false; }

void Projection::SetPerspective() { is_perspective_ = true; }

void Projection::SetFar(const double far) {
  far_ = far;
  UpdatePerspective(persp_pr_.fov_, persp_pr_.aspect_, near_, far_);
  UpdateOrthogonal(orth_pr_.left_, orth_pr_.right_, orth_pr_.bottom_,
                   orth_pr_.top_, near_, far_);
}
void Projection::SetNear(const double near) {
  near_ = near;
  UpdatePerspective(persp_pr_.fov_, persp_pr_.aspect_, near_, far_);
  UpdateOrthogonal(orth_pr_.left_, orth_pr_.right_, orth_pr_.bottom_,
                   orth_pr_.top_, near_, far_);
}

void Projection::SetPerspProjectionMatrix(const S21Matrix &other) {
  persp_projection_matrix_ = other;
}

void Projection::SetOrthProjectionMatrix(const S21Matrix &other) {
  orth_projection_matrix_ = other;
}

S21Matrix Projection::GetPerspProjMatrix() { return persp_projection_matrix_; }

S21Matrix Projection::GetOrthProjMatrix() { return orth_projection_matrix_; }

bool Projection::IsPerspectiveProjection() { return is_perspective_; }

double Projection::GetFar() const { return far_; }

double Projection::GetNear() const { return near_; }

}  // namespace s21