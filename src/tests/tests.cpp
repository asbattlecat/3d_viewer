#include "tests.h"

using faces_vector_type = std::vector<std::vector<unsigned int>>;

bool AreDoublesEq(const double first, const double second) {
  return (fabs(first - second) < EPSILON);
}

bool AreVectorsEqual(const s21::Vector3D &v1, const s21::Vector3D &v2) {
  return (AreDoublesEq(v1.x, v2.x) && AreDoublesEq(v1.y, v2.y) &&
          AreDoublesEq(v1.z, v2.z));
}

bool AreMatricesEqual(const S21Matrix &m1, const S21Matrix &m2) {
  bool res = true;
  for (int i = 0; i < m1.GetRows() && res; i++) {
    for (int j = 0; j < m1.GetCols() && res; j++) {
      if (!AreDoublesEq(m1(i, j), m2(i, j))) res = false;
    }
  }
  return res;
}

bool IsVerticesVectorsSizeEqual(const std::vector<s21::Vector3D> &v1,
                                const std::vector<s21::Vector3D> &v2) {
  return v1.size() == v2.size();
}

bool AreVerticesVectorsEqual(const std::vector<s21::Vector3D> &v1,
                             const std::vector<s21::Vector3D> &v2) {
  bool res = true;
  if (IsVerticesVectorsSizeEqual(v1, v1)) {
    for (size_t i = 0; i < v1.size() && res; i++) {
      if (!AreVectorsEqual(v1[i], v2[i])) res = false;
    }
  } else {
    res = false;
  }
  return res;
}

bool IsFacesVectorsSizeEqual(const faces_vector_type &f1,
                             const faces_vector_type &f2) {
  return f1.size() == f2.size();
}

bool AreFacesVectorsEqual(const faces_vector_type &f1,
                          const faces_vector_type &f2) {
  bool res = true;
  if (IsFacesVectorsSizeEqual(f1, f1)) {
    for (size_t i = 0; i < f1.size() && res; i++) {
      if (f1[i] != f2[i]) res = false;
    }
  } else
    res = false;
  return res;
}

bool IsEdgesSizeEqual(const unordered_set_pair_hash &e1,
                      const unordered_set_pair_hash &e2) {
  return e1.size() == e2.size();
}

bool AreEdgesSetEqual(const unordered_set_pair_hash &e1,
                      const unordered_set_pair_hash &e2) {
  bool res = true;
  if (IsEdgesSizeEqual(e1, e2)) {
    for (const auto &edge : e1) {
      if (e2.find(edge) == e2.end()) res = false;
    }
  } else {
    res = false;
  }
  return res;
}

void SignalTaker::TakeSignal(const std::string &error_message) {
  got_error_ = true;
  last_error_ = error_message;
}

bool SignalTaker::IsThereError() { return got_error_; }

////////////////////////////////////////////////////////////////////////////////
// Тесты для Vector3D

TEST(TestVector3d, Constructor1) {
  s21::Vector3D v(1.0, 2.0, 3.0);
  EXPECT_FALSE(AreVectorsEqual(v, s21::Vector3D(0.0, 0.0, 0.0)));
  EXPECT_TRUE(AreVectorsEqual(v, s21::Vector3D(1.0, 2.0, 3.0)));
}

TEST(TestVector3d, Constructor2) {
  s21::Vector3D v(1.0, 0.5, 0.5);
  s21::Vector3D oth_v(v);
  EXPECT_TRUE(AreVectorsEqual(v, s21::Vector3D(1.0, 0.5, 0.5)));
  EXPECT_TRUE(AreVectorsEqual(oth_v, s21::Vector3D(1.0, 0.5, 0.5)));
}

TEST(TestVector3d, Constructor3) {
  s21::Vector3D v(1.0, 0.5, 0.5);
  s21::Vector3D v_2(std::move(v));
  EXPECT_TRUE(AreVectorsEqual(v_2, s21::Vector3D(1.0, 0.5, 0.5)));
  EXPECT_TRUE(AreVectorsEqual(v, s21::Vector3D(0.0, 0.0, 0.0)));
}

TEST(TestVector3d, OperatorCopy) {
  s21::Vector3D v(1.0, 0.5, 0.5);
  s21::Vector3D v_2 = v;
  EXPECT_TRUE(AreVectorsEqual(v, s21::Vector3D(1.0, 0.5, 0.5)));
  EXPECT_TRUE(AreVectorsEqual(v_2, s21::Vector3D(1.0, 0.5, 0.5)));
}

TEST(TestVector3d, OperatorMode) {
  s21::Vector3D v(1.0, 0.5, 0.5);
  s21::Vector3D v_2 = std::move(v);
  EXPECT_TRUE(AreVectorsEqual(v_2, s21::Vector3D(1.0, 0.5, 0.5)));
  EXPECT_TRUE(AreVectorsEqual(v, s21::Vector3D(0.0, 0.0, 0.0)));
}

TEST(TestVector3d, OperatorMinus) {
  s21::Vector3D v1(1.0, 0.5, 0.5);
  s21::Vector3D v2(1.0, 0.5, 0.5);
  s21::Vector3D v3 = v1 - v2;
  EXPECT_TRUE(AreVectorsEqual(v3, s21::Vector3D(0.0, 0.0, 0.0)));
}

TEST(TestVector3d, OperatorMinus2) {
  s21::Vector3D v1(2.0, 1.0, 1.0);
  s21::Vector3D v2(1.0, 0.5, 0.5);
  s21::Vector3D v3 = v1 - v2;
  EXPECT_TRUE(AreVectorsEqual(v3, s21::Vector3D(1.0, 0.5, 0.5)));
}

TEST(TestVector3d, OperatorPlus) {
  s21::Vector3D v1(2.0, 1.0, 1.0);
  s21::Vector3D v2(1.0, 0.5, 0.5);
  s21::Vector3D v3 = v1 + v2;
  EXPECT_TRUE(AreVectorsEqual(v3, s21::Vector3D(3.0, 1.5, 1.5)));
}

TEST(TestVector3d, OperatorMulNum) {
  s21::Vector3D v1(2.0, 1.0, 1.0);
  double num = 2;
  s21::Vector3D v2 = v1 * num;
  EXPECT_TRUE(AreVectorsEqual(v2, s21::Vector3D(4.0, 2.0, 2.0)));
}

TEST(TestVector3d, Cross) {
  s21::Vector3D v1(1.0, 2.0, 3.0);
  s21::Vector3D v2(3.0, 4.0, 5.0);
  s21::Vector3D cross = v1.cross(v2);
  EXPECT_TRUE(AreVectorsEqual(cross, s21::Vector3D(-2.0, 4.0, -2.0)));
}

TEST(TestVector3d, Length) {
  s21::Vector3D v1(1.0, 1.0, 1.0);
  double length1 = v1.length();
  EXPECT_TRUE(AreDoublesEq(length1, std::sqrt(3.0)));

  s21::Vector3D v2(3, 3, 3);
  double length2 = v2.length();
  EXPECT_TRUE(AreDoublesEq(length2, std::sqrt(27.0)));
}

TEST(TestVector3d, Normalize) {
  s21::Vector3D v1(0.0, 0.0, 0.0);
  EXPECT_THROW(v1.normalize(), std::invalid_argument);

  s21::Vector3D v2(1.0, 1.0, 1.0);
  s21::Vector3D v3 = v2.normalize();
  EXPECT_TRUE(
      AreVectorsEqual(v3, s21::Vector3D(1.0 / std::sqrt(3), 1.0 / std::sqrt(3),
                                        1.0 / std::sqrt(3))));
}

TEST(TestVector3d, Scalar) {
  s21::Vector3D v1(1.0, 1.0, 1.0);
  s21::Vector3D v2(2.0, 2.0, 2.0);
  double result = v1.scalar(v2);
  EXPECT_TRUE(AreDoublesEq(result, 6.0));
}

////////////////////////////////////////////////////////////////////////////////
// Тесты для AffineTransDecorator

TEST(TestAffine, Constructor) {
  s21::S21MatrixWrapper mat(4, 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      mat(i, j) = i + j;
    }
  }
  s21::S21MatrixWrapper mat2 = mat;
  s21::AffineTransDecorator aff(
      std::move(std::make_unique<s21::S21MatrixWrapper>(mat)));
  S21Matrix res = aff.GetMatrix();
  S21Matrix res_2 = mat2.GetMatrix();
  EXPECT_TRUE(AreMatricesEqual(res, res_2));
}

TEST(TestAffine, EqMat) {
  S21Matrix mat(4, 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      mat(i, j) = i + j;
    }
  }
  std::unique_ptr mat_2 = std::make_unique<s21::S21MatrixWrapper>(4, 4);
  s21::AffineTransDecorator aff(std::move(mat_2));
  aff = mat;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_TRUE(AreDoublesEq(mat(i, j), aff(i, j)));
    }
  }
}

TEST(TestAffine, EqMat2) {
  S21Matrix mat(3, 3);
  std::unique_ptr mat_2 = std::make_unique<s21::S21MatrixWrapper>(4, 4);
  s21::AffineTransDecorator aff(std::move(mat_2));
  EXPECT_THROW(aff = mat, std::invalid_argument);
}

TEST(TestAffine, Translation) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(4, 4);
  s21::AffineTransDecorator aff(std::move(mat));
  aff.SetIdentity();
  aff.Translation(1, 1, 1);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i == j) {
        EXPECT_TRUE(AreDoublesEq(aff(i, j), 1.0));
      }
      if (i >= 0 && i <= 2 && j == 3) {
        EXPECT_TRUE(AreDoublesEq(aff(i, j), 1.0));
      }
    }
  }
}

TEST(TestAffine, Translation2) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(3, 3);
  s21::AffineTransDecorator aff(std::move(mat));
  EXPECT_THROW(aff.Translation(1, 1, 1), std::invalid_argument);
}

TEST(TestAffine, Rotation) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(3, 3);
  s21::AffineTransDecorator aff(std::move(mat));
  s21::Vector3D vec(0.0, 1.0, 0.0);
  EXPECT_THROW(aff.Rotation(90, vec), std::invalid_argument);
}

TEST(TestAffine, Rotation2) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(4, 4);
  s21::AffineTransDecorator aff(std::move(mat));
  aff.SetIdentity();
  s21::Vector3D vec(0.0, 1.0, 0.0);
  double angle = 80.0;
  double radian = angle * M_PI / 180.0;
  aff.Rotation(80, vec);
  S21Matrix exp_res(4, 4);
  exp_res.SetIdentity();
  exp_res(0, 0) = std::cos(radian);
  exp_res(0, 2) = std::sin(radian);
  exp_res(2, 0) = -std::sin(radian);
  exp_res(2, 2) = std::cos(radian);
  S21Matrix res = aff.GetMatrix();
  EXPECT_TRUE(AreMatricesEqual(exp_res, res));
}

TEST(TestAffine, Scale) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(3, 3);
  s21::AffineTransDecorator aff(std::move(mat));
  EXPECT_THROW(aff.Scale(2.0, 2.0, 2.0), std::invalid_argument);
}

TEST(TestAffine, Scale2) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(4, 4);
  s21::AffineTransDecorator aff(std::move(mat));
  aff.SetIdentity();
  aff.Scale(2.0, 2.0, 2.0);
  S21Matrix exp_res(4, 4);
  exp_res.SetIdentity();
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (i == j) {
        exp_res(i, j) = 2.0;
      }
    }
  }
  S21Matrix res = aff.GetMatrix();
  EXPECT_TRUE(AreMatricesEqual(exp_res, res));
}

TEST(TestAffine, ViewMatrix) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(4, 4);
  s21::AffineTransDecorator aff(std::move(mat));
  aff.SetIdentity();
  s21::Vector3D target(0.0, 0.0, 0.0);
  s21::Vector3D eye(1.0, 1.0, 1.0);
  s21::Vector3D up(0.0, 1.0, 0.0);
  S21Matrix res = aff.CreateViewMatrix(eye, target, up);

  S21Matrix exp_res(4, 4);
  exp_res.SetIdentity();
  exp_res(0, 0) = 1.0 / std::sqrt(2);
  exp_res(0, 2) = -1.0 / std::sqrt(2);

  exp_res(1, 0) = 1.0 / std::sqrt(6);
  exp_res(1, 1) = -2.0 / std::sqrt(6);
  exp_res(1, 2) = 1.0 / std::sqrt(6);

  exp_res(2, 0) = 1.0 / std::sqrt(3);
  exp_res(2, 1) = 1.0 / std::sqrt(3);
  exp_res(2, 2) = 1.0 / std::sqrt(3);
  exp_res(2, 3) = -std::sqrt(3);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_TRUE(AreDoublesEq(res(i, j), exp_res(i, j)));
    }
  }
}

TEST(TestAffine, ViewMatrix2) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(3, 3);
  s21::AffineTransDecorator aff(std::move(mat));
  s21::Vector3D target(0.0, 0.0, 0.0);
  s21::Vector3D eye(1.0, 1.0, 1.0);
  s21::Vector3D up(0.0, 1.0, 0.0);
  EXPECT_THROW(aff.CreateViewMatrix(eye, target, up), std::invalid_argument);
}

TEST(TestAffine, PerspProj) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(4, 4);
  s21::AffineTransDecorator aff(std::move(mat));
  aff.SetIdentity();
  S21Matrix res = aff.CreateProjMatrixPersp(90, 1, 1, 10);

  S21Matrix exp_res(4, 4);
  exp_res.SetIdentity();
  exp_res(0, 0) = 1.0;
  exp_res(1, 1) = 1.0;
  exp_res(2, 2) = -11.0 / 9.0;
  exp_res(2, 3) = -20.0 / 9.0;
  exp_res(3, 2) = -1.0;
  exp_res(3, 3) = 0.0;
  EXPECT_TRUE(AreMatricesEqual(res, exp_res));
}

TEST(TestAffine, PerspProj2) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(4, 4);
  s21::AffineTransDecorator aff(std::move(mat));
  EXPECT_THROW(aff.CreateProjMatrixPersp(-1, 1, 1, 10), std::invalid_argument);
  EXPECT_THROW(aff.CreateProjMatrixPersp(90, -1, 1, 10), std::invalid_argument);
  EXPECT_THROW(aff.CreateProjMatrixPersp(90, 1, -1, 10), std::invalid_argument);
  EXPECT_THROW(aff.CreateProjMatrixPersp(90, 1, 1, 0.1), std::invalid_argument);
}

TEST(TestAffine, OrthProj) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(4, 4);
  s21::AffineTransDecorator aff(std::move(mat));
  S21Matrix res = aff.CreateProjMatrixOrth(-10, 10, -10, 10, 1, 10);
  S21Matrix exp_res(4, 4);
  exp_res.SetIdentity();

  exp_res(0, 0) = 1.0 / 10.0;
  exp_res(1, 1) = 1.0 / 10.0;
  exp_res(2, 2) = -2.0 / 9.0;
  exp_res(2, 3) = -11.0 / 9.0;
  exp_res(3, 3) = 1.0;
  EXPECT_TRUE(AreMatricesEqual(res, exp_res));
}

TEST(TestAffine, OrthProj2) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(3, 3);
  s21::AffineTransDecorator aff(std::move(mat));
  EXPECT_THROW(aff.CreateProjMatrixOrth(20, 10, -10, 10, 1, 10),
               std::invalid_argument);
  EXPECT_THROW(aff.CreateProjMatrixOrth(-10, -20, -10, 10, 1, 10),
               std::invalid_argument);
  EXPECT_THROW(aff.CreateProjMatrixOrth(-10, 10, 20, 10, 1, 10),
               std::invalid_argument);
  EXPECT_THROW(aff.CreateProjMatrixOrth(-10, 10, -10, -20, 1, 10),
               std::invalid_argument);
  EXPECT_THROW(aff.CreateProjMatrixOrth(-10, 10, -10, 10, 20, 10),
               std::invalid_argument);
  EXPECT_THROW(aff.CreateProjMatrixOrth(-10, 10, -10, 10, 1, -1),
               std::invalid_argument);
}

TEST(TestAffine, Mvp) {
  std::unique_ptr mat = std::make_unique<s21::S21MatrixWrapper>(4, 4);
  s21::AffineTransDecorator aff(std::move(mat));
  aff.SetIdentity();
  S21Matrix proj = aff.CreateProjMatrixOrth(-10, 10, -10, 10, 1, 10);

  s21::Vector3D target(0.0, 0.0, 0.0);
  s21::Vector3D eye(1.0, 1.0, 1.0);
  s21::Vector3D up(0.0, 1.0, 0.0);
  S21Matrix view = aff.CreateViewMatrix(eye, target, up);

  s21::Vector3D axis(0.0, 1.0, 0.0);
  aff.Rotation(80, axis);
  S21Matrix model = aff.GetMatrix();

  S21Matrix mvp_matrix = aff.CreateMVPMatrix(proj, view, model);

  S21Matrix exp_res(4, 4);
  exp_res.SetIdentity();

  // Row 0
  // Символьное заполнение матрицы MVP
  exp_res(0, 2) = -(std::cos(80.0 * M_PI / 180.0) / std::sqrt(2.0) -
                    std::sin(80.0 * M_PI / 180.0) / std::sqrt(2.0)) /
                  10.0;
  exp_res(0, 1) = 0.0;
  exp_res(0, 0) = (std::sin(80.0 * M_PI / 180.0) / std::sqrt(2.0) +
                   std::cos(80.0 * M_PI / 180.0) / std::sqrt(2.0)) /
                  10.0;
  exp_res(0, 3) = 0.0;

  exp_res(1, 0) = -((-std::cos(80.0 * M_PI / 180.0) / std::sqrt(6.0) +
                     std::sin(80.0 * M_PI / 180.0) / std::sqrt(6.0)) /
                    10.0);
  exp_res(1, 1) = -1.0 / (5.0 * std::sqrt(6.0));
  exp_res(1, 2) = -(-std::sin(80.0 * M_PI / 180.0) / std::sqrt(6.0) -
                    std::cos(80.0 * M_PI / 180.0) / std::sqrt(6.0)) /
                  10.0;
  exp_res(1, 3) = 0.0;

  exp_res(2, 0) = -(std::cos(80.0 * M_PI / 180.0) / std::sqrt(3.0) -
                    std::sin(80.0 * M_PI / 180.0) / std::sqrt(3.0)) *
                  2.0 / 9.0;
  exp_res(2, 1) = -2.0 / (9.0 * std::sqrt(3.0));
  exp_res(2, 2) = -(std::sin(80.0 * M_PI / 180.0) / std::sqrt(3.0) +
                    std::cos(80.0 * M_PI / 180.0) / std::sqrt(3.0)) *
                  2.0 / 9.0;
  exp_res(2, 3) = -0.837322;

  exp_res(3, 0) = 0.0;
  exp_res(3, 1) = 0.0;
  exp_res(3, 2) = 0.0;
  exp_res(3, 3) = 1.0;

  EXPECT_TRUE(AreMatricesEqual(mvp_matrix, exp_res));
}

////////////////////////////////////////////////////////////////////////////////

TEST(ObjTest, LoadCube) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  std::vector<s21::Vector3D> exp_ver = {
      {1.0, 1.0, 1.0},    // Вершина 0
      {1.0, 1.0, -1.0},   // Вершина 1
      {1.0, -1.0, 1.0},   // Вершина 2
      {1.0, -1.0, -1.0},  // Вершина 3
      {-1.0, 1.0, 1.0},   // Вершина 4
      {-1.0, 1.0, -1.0},  // Вершина 5
      {-1.0, -1.0, 1.0},  // Вершина 6
      {-1.0, -1.0, -1.0}  // Вершина 7
  };
  auto res_ver = loader.GetVertices();
  EXPECT_TRUE(IsVerticesVectorsSizeEqual(res_ver, exp_ver));
}

TEST(ObjTest, LoadCube2) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  std::vector<s21::Vector3D> exp_ver = {
      {1.0, 1.0, 1.0},    // Вершина 0
      {1.0, 1.0, -1.0},   // Вершина 1
      {1.0, -1.0, 1.0},   // Вершина 2
      {1.0, -1.0, -1.0},  // Вершина 3
      {-1.0, 1.0, 1.0},   // Вершина 4
      {-1.0, 1.0, -1.0},  // Вершина 5
      {-1.0, -1.0, 1.0},  // Вершина 6
      {-1.0, -1.0, -1.0}  // Вершина 7
  };
  auto res_ver = loader.GetVertices();
  EXPECT_TRUE(AreVerticesVectorsEqual(exp_ver, res_ver));
}

TEST(ObjTest, LoadCube3) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  faces_vector_type exp_fac = {{0, 1, 3}, {0, 3, 2}, {4, 5, 7}, {4, 7, 6},
                               {0, 4, 6}, {0, 6, 2}, {1, 5, 7}, {1, 7, 3},
                               {2, 3, 7}, {2, 7, 6}, {0, 1, 5}, {0, 5, 4}};
  auto res_fac = loader.GetFaces();
  EXPECT_TRUE(IsFacesVectorsSizeEqual(res_fac, exp_fac));
}

TEST(ObjTest, LoadCube4) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  faces_vector_type exp_fac = {{0, 1, 3}, {0, 3, 2}, {4, 5, 7}, {4, 7, 6},
                               {0, 4, 6}, {0, 6, 2}, {1, 5, 7}, {1, 7, 3},
                               {2, 3, 7}, {2, 7, 6}, {0, 1, 5}, {0, 5, 4}};
  auto res_fac = loader.GetFaces();
  EXPECT_TRUE(AreFacesVectorsEqual(res_fac, exp_fac));
}

TEST(ObjTest, LoadCube5) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  unordered_set_pair_hash exp_edg = {
      {3, 7}, {1, 7}, {2, 7}, {0, 6}, {0, 4}, {4, 6}, {0, 5}, {6, 7}, {4, 7},
      {1, 5}, {5, 7}, {2, 6}, {4, 5}, {0, 2}, {2, 3}, {0, 3}, {1, 3}, {0, 1}};
  auto res_edg = loader.GetEdges();
  EXPECT_TRUE(IsEdgesSizeEqual(exp_edg, res_edg));
}

TEST(ObjTest, LoadCube6) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  unordered_set_pair_hash exp_edg = {
      {3, 7}, {1, 7}, {2, 7}, {0, 6}, {0, 4}, {4, 6}, {0, 5}, {6, 7}, {4, 7},
      {1, 5}, {5, 7}, {2, 6}, {4, 5}, {0, 2}, {2, 3}, {0, 3}, {1, 3}, {0, 1}};
  auto res_edg = loader.GetEdges();
  EXPECT_TRUE(AreEdgesSetEqual(exp_edg, res_edg));
}

TEST(ObjTest, LoadCube7) {
  s21::ObjLoader loader("tests/cube_errors.obj");
  SignalTaker taker;
  QObject::connect(&loader, &s21::ObjLoader::LoadError, &taker,
                   &SignalTaker::TakeSignal);
  loader.Load();
  EXPECT_TRUE(taker.IsThereError());
}

TEST(ObjTest, LoadCube8) {
  s21::ObjLoader loader("tests/blabla.obj");
  SignalTaker taker;
  QObject::connect(&loader, &s21::ObjLoader::LoadError, &taker,
                   &SignalTaker::TakeSignal);
  loader.Load();
  EXPECT_TRUE(taker.IsThereError());
}

TEST(ObjTest, LoadCube9) {
  s21::ObjLoader loader("tests/cube_errors2.obj");
  SignalTaker taker;
  QObject::connect(&loader, &s21::ObjLoader::LoadError, &taker,
                   &SignalTaker::TakeSignal);
  loader.Load();
  EXPECT_TRUE(taker.IsThereError());
}

TEST(ObjTest, LoadCube10) {
  s21::ObjLoader loader("tests/cube_errors3.obj");
  SignalTaker taker;
  QObject::connect(&loader, &s21::ObjLoader::LoadError, &taker,
                   &SignalTaker::TakeSignal);
  loader.Load();
  EXPECT_TRUE(taker.IsThereError());
}

TEST(ObjTest, LoadCube11) {
  s21::ObjLoader loader("tests/cube_errors4.obj");
  EXPECT_THROW(loader.Load(), std::out_of_range);
}

TEST(ModelTest, LoadCube12) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  s21::Model3D model;
  model.SetData(loader.GetVertices(), loader.GetFaces(), loader.GetEdges());
  std::vector<s21::Vector3D> exp_ver = {
      {1.0, 1.0, 1.0},    // Вершина 0
      {1.0, 1.0, -1.0},   // Вершина 1
      {1.0, -1.0, 1.0},   // Вершина 2
      {1.0, -1.0, -1.0},  // Вершина 3
      {-1.0, 1.0, 1.0},   // Вершина 4
      {-1.0, 1.0, -1.0},  // Вершина 5
      {-1.0, -1.0, 1.0},  // Вершина 6
      {-1.0, -1.0, -1.0}  // Вершина 7
  };
  auto res_ver = model.GetVertices();
  EXPECT_TRUE(IsVerticesVectorsSizeEqual(res_ver, exp_ver));
}

TEST(ModelTest, LoadCube13) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  s21::Model3D model;
  model.SetData(loader.GetVertices(), loader.GetFaces(), loader.GetEdges());
  std::vector<s21::Vector3D> exp_ver = {
      {1.0, 1.0, 1.0},    // Вершина 0
      {1.0, 1.0, -1.0},   // Вершина 1
      {1.0, -1.0, 1.0},   // Вершина 2
      {1.0, -1.0, -1.0},  // Вершина 3
      {-1.0, 1.0, 1.0},   // Вершина 4
      {-1.0, 1.0, -1.0},  // Вершина 5
      {-1.0, -1.0, 1.0},  // Вершина 6
      {-1.0, -1.0, -1.0}  // Вершина 7
  };
  auto res_ver = model.GetVertices();
  EXPECT_TRUE(AreVerticesVectorsEqual(res_ver, exp_ver));
}

TEST(ModelTest, LoadCube14) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  s21::Model3D model;
  model.SetData(loader.GetVertices(), loader.GetFaces(), loader.GetEdges());
  faces_vector_type exp_fac = {{0, 1, 3}, {0, 3, 2}, {4, 5, 7}, {4, 7, 6},
                               {0, 4, 6}, {0, 6, 2}, {1, 5, 7}, {1, 7, 3},
                               {2, 3, 7}, {2, 7, 6}, {0, 1, 5}, {0, 5, 4}};
  auto res_fac = model.GetFaces();
  EXPECT_TRUE(AreFacesVectorsEqual(res_fac, exp_fac));
}

TEST(ModelTest, LoadCube15) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  s21::Model3D model;
  model.SetData(loader.GetVertices(), loader.GetFaces(), loader.GetEdges());
  unordered_set_pair_hash exp_edg = {
      {3, 7}, {1, 7}, {2, 7}, {0, 6}, {0, 4}, {4, 6}, {0, 5}, {6, 7}, {4, 7},
      {1, 5}, {5, 7}, {2, 6}, {4, 5}, {0, 2}, {2, 3}, {0, 3}, {1, 3}, {0, 1}};
  auto res_edg = model.GetEdges();
  EXPECT_TRUE(IsEdgesSizeEqual(exp_edg, res_edg));
}

TEST(ModelTest, LoadCube16) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  s21::Model3D model;
  model.SetData(loader.GetVertices(), loader.GetFaces(), loader.GetEdges());
  unordered_set_pair_hash exp_edg = {
      {3, 7}, {1, 7}, {2, 7}, {0, 6}, {0, 4}, {4, 6}, {0, 5}, {6, 7}, {4, 7},
      {1, 5}, {5, 7}, {2, 6}, {4, 5}, {0, 2}, {2, 3}, {0, 3}, {1, 3}, {0, 1}};
  auto res_edg = model.GetEdges();
  EXPECT_TRUE(AreEdgesSetEqual(exp_edg, res_edg));
}

TEST(ModelTest, LoadCube17) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  s21::Model3D model;
  model.SetData(loader.GetVertices(), loader.GetFaces(), loader.GetEdges());
  s21::BoundingBox box = model.GetBoundingBox();
  EXPECT_TRUE(AreVectorsEqual(box.max, s21::Vector3D(1.0, 1.0, 1.0)));
  EXPECT_TRUE(AreVectorsEqual(box.min, s21::Vector3D(-1.0, -1.0, -1.0)));
}

TEST(ModelTest, LoadCube18) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  s21::Model3D model;
  model.SetData(loader.GetVertices(), loader.GetFaces(), loader.GetEdges());
  size_t edges = model.GetEdgesCount();
  EXPECT_EQ(edges, 18);
}

TEST(ModelTest, LoadCube19) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  s21::Model3D model;
  model.SetData(loader.GetVertices(), loader.GetFaces(), loader.GetEdges());
  size_t vertices = model.GetVerticesCount();
  EXPECT_EQ(vertices, 8);
}

TEST(ModelTest, LoadCube20) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  s21::Model3D model;
  model.SetData(loader.GetVertices(), loader.GetFaces(), loader.GetEdges());
  std::vector<float> exp_res = {
      1.0f,  1.0f,  1.0f,   // Вершина 0: x, y, z
      1.0f,  1.0f,  -1.0f,  // Вершина 1: x, y, z
      1.0f,  -1.0f, 1.0f,   // Вершина 2: x, y, z
      1.0f,  -1.0f, -1.0f,  // Вершина 3: x, y, z
      -1.0f, 1.0f,  1.0f,   // Вершина 4: x, y, z
      -1.0f, 1.0f,  -1.0f,  // Вершина 5: x, y, z
      -1.0f, -1.0f, 1.0f,   // Вершина 6: x, y, z
      -1.0f, -1.0f, -1.0f   // Вершина 7: x, y, z
  };
  auto res = model.GetVerticesToGlFormat();
  EXPECT_TRUE(res == exp_res);
}

TEST(ModelTest, LoadCube21) {
  s21::ObjLoader loader("tests/cube.obj");
  loader.Load();
  s21::Model3D model;
  model.SetData(loader.GetVertices(), loader.GetFaces(), loader.GetEdges());
  std::vector<unsigned int> exp_res = {3, 7, 1, 7, 2, 7, 0, 6, 0, 4, 4, 6,
                                       6, 7, 0, 5, 4, 7, 1, 5, 5, 7, 2, 6,
                                       4, 5, 0, 2, 2, 3, 0, 3, 1, 3, 0, 1};
  auto res = model.GetEdgesToGlFormat();

  EXPECT_TRUE(res == exp_res);
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}