#ifndef TESTS_H
#define TESTS_H

#include <gtest/gtest.h>

#include "../include/3dmodel.h"
#include "../include/affine.h"

using faces_vector_type = std::vector<std::vector<unsigned int>>;
bool AreDoublesEq(const double first, const double second);
bool AreVectorsEqual(const s21::Vector3D &v1, const s21::Vector3D &v2);
bool AreMatricesEqual(const S21Matrix &m1, const S21Matrix &m2);
bool IsVerticesVectorsSizeEqual(const std::vector<s21::Vector3D> &v1,
                                const std::vector<s21::Vector3D> &v2);

bool AreVerticesVectorsEqual(const std::vector<s21::Vector3D> &v1,
                             const std::vector<s21::Vector3D> &v2);
bool IsFacesVectorsSizeEqual(const faces_vector_type &f1,
                             const faces_vector_type &f2);
bool AreFacesVectorsEqual(const faces_vector_type &f1,
                          const faces_vector_type &f2);
bool IsEdgesSizeEqual(const unordered_set_pair_hash &e1,
                      const unordered_set_pair_hash &e2);
bool AreEdgesSetEqual(const unordered_set_pair_hash &e1,
                      const unordered_set_pair_hash &e2);

class SignalTaker : public QObject {
  Q_OBJECT
 public slots:
  void TakeSignal(const std::string &error_message);

 private:
  std::string last_error_;
  bool got_error_ = false;

 public:
  bool IsThereError();
};

#endif