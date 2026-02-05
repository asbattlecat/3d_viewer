#ifndef MODEL_H
#define MODEL_H

#include "affine.h"
#include "include_common.h"

namespace s21 {

/**
 * @brief В общем-то простая структура расчета хэша для unordered_set
 */
struct PairHash {
  template <typename T1, typename T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>{}(pair.first) ^ (std::hash<T2>{}(pair.second) << 1);
  }
};

struct Model3DDataGl {
  std::vector<float> vertices;
  std::vector<unsigned int> edges;

  Model3DDataGl(std::vector<float> verts, std::vector<unsigned int> edgs);
};

struct BoundingBox {
  Vector3D max;
  Vector3D min;
};

}  // namespace s21

// без использования псевдонима ниже не получается зарегистрировать в Qt сложный
// тип данных, поэтому он и используется
using unordered_set_pair_hash =
    std::unordered_set<std::pair<unsigned int, unsigned int>, s21::PairHash>;

Q_DECLARE_METATYPE(std::vector<s21::Vector3D>);
Q_DECLARE_METATYPE(std::vector<std::vector<unsigned int>>);
Q_DECLARE_METATYPE(unordered_set_pair_hash);

namespace s21 {

/**
 * @brief Класс для загрузки .obj файла не в основном потоке.
 */
class ObjLoader : public QObject {
  Q_OBJECT
 private:
  std::vector<Vector3D> vertices_;
  std::vector<std::vector<unsigned int>> faces_;
  std::unordered_set<std::pair<unsigned int, unsigned int>, PairHash> edges_;
  std::string obj_file_;

 public:
  ObjLoader();
  ObjLoader(const std::string &file, QObject *parent = nullptr);

  const std::vector<Vector3D> &GetVertices();
  const std::vector<std::vector<unsigned int>> &GetFaces();
  const std::unordered_set<std::pair<unsigned int, unsigned int>, PairHash> &
  GetEdges();

 public slots:
  void Load();

 signals:
  void Loaded(const std::vector<Vector3D> &vertices,
              const std::vector<std::vector<unsigned int>> &faces,
              const std::unordered_set<std::pair<unsigned int, unsigned int>,
                                       PairHash> &edges);
  void LoadError(const std::string &error_message);

 private:
  bool is_vertex_(const std::string &line) const;
  bool is_face_(const std::string &line) const;
  void add_vertex_(const std::string &line);
  void add_face_(const std::string &line);
  void edges_count_(const std::vector<unsigned int> indexes);

  // bool is_line_correct_(const std::string &line) const;
  void exception_invalid_data_stream_(const std::string &line,
                                      const std::string &method_name,
                                      const std::istringstream &str_stream);
  void index_correction_(int &index) const;
  bool is_there_same_face_indexes_(
      const std::vector<unsigned int> &indexes) const;

  void set_start_capacity_();
};

/**
 * @brief Класс для хранения данных о фигуре.
 */
class Model3D {
 private:
  std::vector<Vector3D> vertices_;
  std::vector<std::vector<unsigned int>> faces_;
  std::unordered_set<std::pair<unsigned int, unsigned int>, PairHash> edges_;
  BoundingBox box_;

 public:
  Model3D() = default;
  void SetData(const std::vector<Vector3D> &vertices,
               const std::vector<std::vector<unsigned int>> &faces,
               const std::unordered_set<std::pair<unsigned int, unsigned int>,
                                        PairHash> &edges);
  void CalculateBoundingBox();

  // getters
  std::vector<float> GetVerticesToGlFormat();
  std::vector<unsigned int> GetEdgesToGlFormat();

  const std::vector<Vector3D> &GetVertices();
  const std::vector<std::vector<unsigned int>> &GetFaces();
  const std::unordered_set<std::pair<unsigned int, unsigned int>, PairHash> &
  GetEdges();
  const BoundingBox GetBoundingBox();
  size_t GetEdgesCount();
  size_t GetVerticesCount();
};

}  // namespace s21

#endif