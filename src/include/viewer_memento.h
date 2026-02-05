#ifndef VIEWER_MEMENTO_H
#define VIEWER_MEMENTO_H

#include "affine.h"
#include "camera.h"
#include "include_common.h"
#include "include_gui_common.h"
#include "projection.h"

namespace s21 {

class GLWidgetMemento {
 private:
  QVector4D line_color_;
  QVector4D back_color_;
  GLfloat line_width_;
  bool is_line_solid_;
  GLushort stipple_pattern_;
  GLint stipple_factor_;
  GLfloat point_vertex_size_;
  bool show_point_vertex_;
  bool is_point_vertex_round_;
  QVector4D point_vertex_color_;

 public:
  GLWidgetMemento(QVector4D line_col, QVector4D back_col, GLfloat line_wid,
                  bool is_line_solid, GLushort stipple_patt, GLint stipple_fac,
                  GLfloat vertex_size, bool show_vertex, bool is_vertex_round,
                  QVector4D vertex_color);

  QJsonObject WriteToJson();
  static GLWidgetMemento ReadFromJson(const QJsonObject &json);

  QVector4D GetLineColor();
  QVector4D GetBackColor();
  GLfloat GetLineWidth();
  bool IsLineSolid();
  GLushort GetStipplePattern();
  GLint GetStippleFactor();
  GLfloat GetPointVertexSize();
  bool IsPointVertexDisplayed();
  bool IsPointVertexRound();
  QVector4D GetPointVertexColor();
};

class SceneMemento {
 private:
  std::string file_path_;
  S21Matrix transformation_;
  Projection projection_;

 public:
  SceneMemento(std::string &file_path, S21Matrix &transformation,
               Projection &projection);
  QJsonObject WriteToJson();
  static SceneMemento ReadFromJson(const QJsonObject &json);

  // геттеры
  std::string GetFilePath();
  S21Matrix GetTransformationMatrix();
  Projection GetProjection();

 private:
  void write_to_json_transformation_(QJsonObject &json);
  void write_to_json_projection_(QJsonObject &json);
  static S21Matrix read_from_json_transformation_(const QJsonObject &json);
  static Projection read_from_json_projection_(const QJsonObject &json);
};

}  // namespace s21

#endif