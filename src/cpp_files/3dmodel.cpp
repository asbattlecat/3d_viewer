#include "./../include/3dmodel.h"

namespace s21 {

Model3DDataGl::Model3DDataGl(std::vector<float> verts,
                             std::vector<unsigned int> edgs)
    : vertices(verts), edges(edgs) {}

////////////////////////////////////////////////////////////////////////////////
// реализация ObjLoader

ObjLoader::ObjLoader() { set_start_capacity_(); }

ObjLoader::ObjLoader(const std::string &file, QObject *parent)
    : QObject(parent), obj_file_(file) {
  set_start_capacity_();
}

//________________________________________________________________________
// чисто для проверки вывода содержимого распаршеного obj файла

const std::vector<Vector3D> &ObjLoader::GetVertices() { return vertices_; }

const std::vector<std::vector<unsigned int>> &ObjLoader::GetFaces() {
  return faces_;
}

const std::unordered_set<std::pair<unsigned int, unsigned int>, PairHash> &
ObjLoader::GetEdges() {
  return edges_;
}
// ________________________________________________________________________

/**
 * @brief Метод для чтения OBJ файла. В процессе чтения записываются данные о
 * вершинах (vertices_), гранях (faces_), количестве ребер (edges_)
 */
void ObjLoader::Load() {
  std::ifstream in;
  in.open(obj_file_);
  if (in.is_open()) {
    std::string line;
    Vector3D vertix;
    line.reserve(256);
    while (std::getline(in, line)) {
      if (is_vertex_(line)) {
        add_vertex_(line);
      } else if (is_face_(line)) {
        add_face_(line);
      }
    }
  } else {
    emit LoadError("Model3D::ReadObjFile(): file does not exist!");
  }
  in.close();
  emit Loaded(vertices_, faces_, edges_);
}

/**
 * @brief Метод для определения, является ли строка данными о вершине
 * @return true - строка является вершиной, false - строка не явялется вершиной
 */

bool ObjLoader::is_vertex_(const std::string &line) const {
  return (line[0] == 'v' && line[1] == ' ');
}

/**
 * @brief Метод для определения, является ли строка данными о грани
 * @return true - строка является вершиной, false - не является
 */
bool ObjLoader::is_face_(const std::string &line) const {
  return (line[0] == 'f' && line[1] == ' ');
}

/**
 * @brief Метод для парсинга OBJ-файла, для записи данных о вершинах, здесь
 * включена обработка ошибок
 */
void ObjLoader::add_vertex_(const std::string &line) {
  std::istringstream str_stream(line);
  char v = 0;
  str_stream >> v;
  std::vector<double> coords;
  coords.reserve(3);
  double num = 0;
  while (str_stream >> num) {
    coords.push_back(num);
  }
  exception_invalid_data_stream_(line, "add_vertex_()", str_stream);
  if (coords.size() != 3) {
    emit LoadError(
        "Model3D::add_vertex_(): count of coords must be 3! Current count: " +
        coords.size());
  }

  vertices_.push_back(Vector3D(coords[0], coords[1], coords[2]));
}

/**
 * @brief Метод для парсинга OBJ-файла. Этот метод собирает данные о гранях
 * 3D-модели, также этот метод ведет подсчет ребер в процессе обработки строк
 */
void ObjLoader::add_face_(const std::string &line) {
  std::istringstream str_stream(line);
  std::string another_line;
  another_line.reserve(32);

  std::vector<unsigned int> indexes;
  indexes.reserve(6);

  char f = 0;
  int index = 0;
  str_stream >> f;

  while (str_stream >> another_line) {
    std::istringstream another_stream(another_line);
    another_stream >> index;
    if (index == 0) {
      emit LoadError("Model3D::add_face_(): index cannot be 0!");
    }
    exception_invalid_data_stream_(line, "add_face_()", another_stream);
    index_correction_(index);
    indexes.push_back(static_cast<unsigned int>(index));
  }
  exception_invalid_data_stream_(line, "add_face_()", str_stream);
  if (indexes.size() < 3) {
    emit LoadError("Model3D::add_face(): there must be 3 or more indexes!");
  }
  if (!is_there_same_face_indexes_(indexes)) {
    edges_count_(indexes);
    faces_.push_back(indexes);
  }
}

/**
 * @brief Метод для подсчета количества ребер
 */
void ObjLoader::edges_count_(const std::vector<unsigned int> indexes) {
  int first = 0, second = 0;
  for (size_t i = 0; i < indexes.size(); ++i) {
    first = indexes[i];
    second = indexes[(i + 1) % indexes.size()];
    edges_.insert({std::min(first, second), std::max(first, second)});
  }
}

// bool Model3D::is_line_correct_(const s td::string &line) const {}

void ObjLoader::exception_invalid_data_stream_(
    const std::string &line, const std::string &method_name,
    const std::istringstream &str_stream) {
  if (str_stream.fail() && !str_stream.eof()) {
    emit LoadError("Model3D::" + method_name +
                   ": incorrect data format in line: " + line);
  }
}

bool ObjLoader::is_there_same_face_indexes_(
    const std::vector<unsigned int> &indexes) const {
  std::set<int> unique_indexes(indexes.begin(), indexes.end());
  return unique_indexes.size() != indexes.size();
}

/**
 * @brief Метод для корректировки индексов вершин для faces.
 * @example Индексы могут поступать как положительными, так и отрицательными. В
 * obj файле начинается отсчет от 1, а в OpenGL от 0. Так же если число в obj
 * файле отрицательное - значит это вершина с конца списка. Значит надо
 * перевести в положительное
 */
void ObjLoader::index_correction_(int &index) const {
  if (index >= 1) {
    --index;
  } else if (index < 0) {
    index = vertices_.size() + index;
  }
  if (static_cast<size_t>(index) > vertices_.size()) {
    throw std::out_of_range(
        "ObjLoader::index_correction_(): index is out of range!");
  }
}

/**
 * @brief метод для задачи начального количества "вместимости" буферов для
 * вершин, граней и ребер.
 */
void ObjLoader::set_start_capacity_() {
  vertices_.reserve(1000000);
  faces_.reserve(500000);
  edges_.reserve(2000000);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// реализация Model3D

/**
 * @brief Метод для чтения OBJ файла. В процессе чтения записываются данные о
 * вершинах (vertices_), гранях (faces_), количестве ребер (edges_)
 */

void Model3D::SetData(
    const std::vector<Vector3D> &vertices,
    const std::vector<std::vector<unsigned int>> &faces,
    const std::unordered_set<std::pair<unsigned int, unsigned int>, PairHash>
        &edges) {
  if (vertices.size() == 0) {
    throw std::invalid_argument(
        "Model3D::SetData(): data is empty, there is no vertices!");
  }
  vertices_ = vertices;
  faces_ = faces;
  edges_ = edges;
  CalculateBoundingBox();
}

void Model3D::CalculateBoundingBox() {
  std::vector<Vector3D> vertices = vertices_;
  Vector3D max_b = vertices[0], min_b = vertices[0];

  for (const auto &vertex : vertices) {
    max_b.x = std::max(max_b.x, vertex.x);
    max_b.y = std::max(max_b.y, vertex.y);
    max_b.z = std::max(max_b.z, vertex.z);

    min_b.x = std::min(min_b.x, vertex.x);
    min_b.y = std::min(min_b.y, vertex.y);
    min_b.z = std::min(min_b.z, vertex.z);
  }

  box_.max = max_b;
  box_.min = min_b;
}

std::vector<float> Model3D::GetVerticesToGlFormat() {
  std::vector<float> gl_vertices;
  gl_vertices.reserve(vertices_.size() * 3);
  for (const auto &vertex : vertices_) {
    gl_vertices.push_back(static_cast<float>(vertex.x));
    gl_vertices.push_back(static_cast<float>(vertex.y));
    gl_vertices.push_back(static_cast<float>(vertex.z));
  }

  return gl_vertices;
}

std::vector<unsigned int> Model3D::GetEdgesToGlFormat() {
  std::vector<unsigned int> gl_edges;
  gl_edges.reserve(edges_.size() * 2);
  for (const auto &edge : edges_) {
    gl_edges.push_back((edge.first));
    gl_edges.push_back(edge.second);
  }
  return gl_edges;
}

const std::vector<Vector3D> &Model3D::GetVertices() { return vertices_; }

const std::vector<std::vector<unsigned int>> &Model3D::GetFaces() {
  return faces_;
}

const std::unordered_set<std::pair<unsigned int, unsigned int>, PairHash> &
Model3D::GetEdges() {
  return edges_;
}

const BoundingBox Model3D::GetBoundingBox() { return box_; }

size_t Model3D::GetEdgesCount() { return edges_.size(); }

size_t Model3D::GetVerticesCount() { return vertices_.size(); }

}  // namespace s21
