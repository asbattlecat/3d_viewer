#include "../include/viewer_memento.h"

namespace s21 {

//______________________________________________________________________________
// Реализация GLWidgetMemento

GLWidgetMemento::GLWidgetMemento(QVector4D line_col, QVector4D back_col,
                                 GLfloat line_wid, bool is_line_solid,
                                 GLushort stipple_patt, GLint stipple_fac,
                                 GLfloat vertex_size, bool show_vertex,
                                 bool is_vertex_round, QVector4D vertex_color)
    : line_color_(line_col),
      back_color_(back_col),
      line_width_(line_wid),
      is_line_solid_(is_line_solid),
      stipple_pattern_(stipple_patt),
      stipple_factor_(stipple_fac),
      point_vertex_size_(vertex_size),
      show_point_vertex_(show_vertex),
      is_point_vertex_round_(is_vertex_round),
      point_vertex_color_(vertex_color) {}

QVector4D GLWidgetMemento::GetLineColor() { return line_color_; }

QVector4D GLWidgetMemento::GetBackColor() { return back_color_; }

GLfloat GLWidgetMemento::GetLineWidth() { return line_width_; }

bool GLWidgetMemento::IsLineSolid() { return is_line_solid_; }

GLushort GLWidgetMemento::GetStipplePattern() { return stipple_pattern_; }

GLint GLWidgetMemento::GetStippleFactor() { return stipple_factor_; }

GLfloat GLWidgetMemento::GetPointVertexSize() { return point_vertex_size_; }

bool GLWidgetMemento::IsPointVertexDisplayed() { return show_point_vertex_; }

bool GLWidgetMemento::IsPointVertexRound() {
  return is_point_vertex_round_;
  ;
}

QVector4D GLWidgetMemento::GetPointVertexColor() { return point_vertex_color_; }

/**
 * @brief метод для сериализации данных в json файл
 */
QJsonObject GLWidgetMemento::WriteToJson() {
  QJsonObject json;
  json["line_color"] = QJsonArray{line_color_.x(), line_color_.y(),
                                  line_color_.z(), line_color_.w()};
  json["back_color"] = QJsonArray{back_color_.x(), back_color_.y(),
                                  back_color_.z(), back_color_.w()};
  json["line_width"] = line_width_;
  json["is_line_solid"] = is_line_solid_;
  json["stipple_pattern"] = stipple_pattern_;
  json["stipple_factor"] = stipple_factor_;
  json["point_vertex_size"] = point_vertex_size_;
  json["show_point_vertex"] = show_point_vertex_;
  json["is_point_vertex_round"] = is_point_vertex_round_;
  json["point_vertex_color"] = QJsonArray{
      point_vertex_color_.x(), point_vertex_color_.y(), point_vertex_color_.z(),
      point_vertex_color_.w()};  // Исправлено: .w() вместо .z()

  return json;
}

/**
 * @brief метод для десереализации данных из json файла
 */
GLWidgetMemento GLWidgetMemento::ReadFromJson(const QJsonObject &json) {
  QJsonArray line_col_arr = json["line_color"].toArray();
  QVector4D line_color(line_col_arr[0].toDouble(), line_col_arr[1].toDouble(),
                       line_col_arr[2].toDouble(), line_col_arr[3].toDouble());

  QJsonArray back_col_arr = json["back_color"].toArray();
  QVector4D back_color(back_col_arr[0].toDouble(), back_col_arr[1].toDouble(),
                       back_col_arr[2].toDouble(), back_col_arr[3].toDouble());
  GLfloat line_width = json["line_width"].toDouble();
  bool is_line_solid = json["is_line_solid"].toBool();
  GLushort stipple_pattern =
      static_cast<GLushort>(json["stipple_pattern"].toInt());
  GLint stipple_factor = json["stipple_factor"].toInt();
  GLfloat point_vertex_size = json["point_vertex_size"].toDouble();
  bool show_point_vertex = json["show_point_vertex"].toBool();
  bool is_point_vertex_round = json["is_point_vertex_round"].toBool();

  QJsonArray point_color_arr = json["point_vertex_color"].toArray();
  QVector4D point_vertex_color(
      point_color_arr[0].toDouble(), point_color_arr[1].toDouble(),
      point_color_arr[2].toDouble(), point_color_arr[3].toDouble());

  return GLWidgetMemento(line_color, back_color, line_width, is_line_solid,
                         stipple_pattern, stipple_factor, point_vertex_size,
                         show_point_vertex, is_point_vertex_round,
                         point_vertex_color);
}

//______________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// реализация SceneMemento

SceneMemento::SceneMemento(std::string &file_path, S21Matrix &transformation,
                           Projection &projection)
    : file_path_(file_path),
      transformation_(transformation),
      projection_(projection) {}

QJsonObject SceneMemento::WriteToJson() {
  QJsonObject json;
  json["file_path"] = QString::fromStdString(file_path_);
  write_to_json_transformation_(json);
  write_to_json_projection_(json);
  return json;
}

SceneMemento SceneMemento::ReadFromJson(const QJsonObject &json) {
  std::string file_path = json["file_path"].toString().toStdString();
  S21Matrix transformation = read_from_json_transformation_(json);
  Projection proj = read_from_json_projection_(json);

  return SceneMemento(file_path, transformation, proj);
}

std::string SceneMemento::GetFilePath() { return file_path_; }

S21Matrix SceneMemento::GetTransformationMatrix() { return transformation_; }

Projection SceneMemento::GetProjection() { return projection_; }

void SceneMemento::write_to_json_transformation_(QJsonObject &json) {
  QJsonArray transform_array;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      transform_array.append(transformation_(i, j));
    }
  }
  json["transformation_array"] = transform_array;
}

void SceneMemento::write_to_json_projection_(QJsonObject &json) {
  S21Matrix orth_proj = projection_.GetOrthProjMatrix();
  QJsonArray orth_pr_array;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      orth_pr_array.append(orth_proj(i, j));
    }
  }
  json["orth_projection_matrix"] = orth_pr_array;

  S21Matrix persp_proj = projection_.GetPerspProjMatrix();
  QJsonArray persp_pr_array;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      persp_pr_array.append(persp_proj(i, j));
    }
  }
  json["persp_projection_matrix"] = persp_pr_array;

  json["is_perspective"] = projection_.IsPerspectiveProjection();

  json["far"] = projection_.GetFar();
}

S21Matrix SceneMemento::read_from_json_transformation_(
    const QJsonObject &json) {
  QJsonArray transf_array = json["transformation_array"].toArray();
  S21Matrix transf_matrix(4, 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      transf_matrix(i, j) = transf_array[i * 4 + j].toDouble();
    }
  }
  return transf_matrix;
}

Projection SceneMemento::read_from_json_projection_(const QJsonObject &json) {
  QJsonArray orth_pr_array = json["orth_projection_matrix"].toArray();
  S21Matrix orth_proj(4, 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      orth_proj(i, j) = orth_pr_array[i * 4 + j].toDouble();
    }
  }

  QJsonArray persp_pr_array = json["persp_projection_matrix"].toArray();
  S21Matrix persp_proj(4, 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      persp_proj(i, j) = persp_pr_array[i * 4 + j].toDouble();
    }
  }

  bool is_perspective = json["is_perspective"].toBool();

  double far = json["far"].toDouble();

  Projection projection;
  projection.SetOrthProjectionMatrix(orth_proj);
  projection.SetPerspProjectionMatrix(persp_proj);
  projection.SetFar(far);
  if (is_perspective) projection.SetPerspective();
  return projection;
}

}  // namespace s21