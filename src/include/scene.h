#ifndef SCENE_H
#define SCENE_H

#include "3dmodel.h"
#include "camera.h"
#include "gif_creator.h"
#include "include_common.h"
#include "projection.h"
#include "viewer_memento.h"

namespace s21 {

/**
 * @brief Это реализация паттерна фасад, т.к. класс Scene является объединяющим
 * классом для четырех других классов: Model3D (хранение данных о модели),
 * AffineTransDecorator (здесь есть экземпляр transformation_, он предоставляет
 * возможность осуществлять перемещение, вращение, масштабирование,
 * предоставляет матрицу ModelMatrix), Camera (предоставляет матрицу
 * ViewMatrix), Projection (предоставляет ProjectionMatrix)
 */
class Scene : public QObject {
  Q_OBJECT
 private:
  std::unique_ptr<Model3D> model_;
  std::unique_ptr<AffineTransDecorator> transformation_;  // для ModelMatrix
  std::unique_ptr<Camera> camera_;
  std::unique_ptr<Projection> projection_;
  bool need_to_calculate_far_ = true;

  std::string current_file_path_;
  bool is_model_displayed_ = false;

 public:
  Scene();
  void LoadModelNoThread(const std::string &obj_file_name);
  void LoadModelMemento();
  void LoadModelThread(const std::string &obj_file_name);
  void UpdateCamera(const Vector3D &target, const Vector3D &eye,
                    const Vector3D &up);
  void UpdateProjectionPersp(double fov, double aspect);
  void UpdateProjectionOrth(double left, double right, double bottom,
                            double top);
  void UpdateProjection();
  void UpdateFar();
  void ApplyTranslation(double tx, double ty, double tz);
  void ApplyRotation(double angle, Vector3D &axis);
  void ApplyScale(double scale_val);
  S21Matrix CreateMVPMatrix();
  Model3DDataGl GetModelAsGLData();

  bool IsPerspective();

  void InitialMVPMatrix();
  std::string GetFileName();
  size_t GetVerticesCount();
  size_t GetEdgesCount();

  void print_figues_data();

  void SetMemento(SceneMemento &memento);
  SceneMemento CreateMemento();

  bool IsModelDisplayed();

  // сигналы для контроллера
 signals:
  void Loaded(const Model3DDataGl &gl_data);
  void LoadedMemento(const Model3DDataGl &gl_data);
  void ErrorLoad(const std::string &error_message);

  void UpdateMVP();
  void UpdateTempMVP(S21MatrixWrapper &mvp);

  void OkTranslationData(double tx, double ty, double tz);
  void OkRotationData(double angle, Vector3D &axis);
  void OkScalingData(double scale_value);

 public slots:
  void ProcessObjLoad(std::string &file_path);

  void ProcessTranslation_nofps(const QString &tx_s, const QString &ty_s,
                                const QString &tz_s);
  void ProcessTranslation_fps(double tx, double ty, double tz);
  void CheckTranslationData_fps(const QString &tx_s, const QString &ty_s,
                                const QString &tz_s);

  void ProcessRotation_nofps(const QString &angle_s, const QString &axis_s);
  void ProcessRotation_fps(double angle, Vector3D &axis);
  void CheckRotationData_fps(const QString &angle_s, const QString &axis_s);

  void ProcessScaling_nofps(const QString &scale_value_s);
  void ProcessScaling_fps(double scale_value);
  void CheckScalingData_fps(const QString &scale_value_s);

  void ProcessChangeProjection();

  void CreatingGifFromJpegs(int target_frames, QString &gif_fr_path,
                            QString gif_target_path, double anim_delay);

 private slots:
  void ModelLoaded(
      const std::vector<Vector3D> &vertices,
      const std::vector<std::vector<unsigned int>> &faces,
      const std::unordered_set<std::pair<unsigned int, unsigned int>, PairHash>
          &edges);
  void ModelLoadedMemento(
      const std::vector<Vector3D> &vertices,
      const std::vector<std::vector<unsigned int>> &faces,
      const std::unordered_set<std::pair<unsigned int, unsigned int>, PairHash>
          &edges);
  void LoadError(const std::string &error_message);

 private:
  double calculate_far_(S21Matrix &model_view);
  void switch_projection_();
  Vector3D get_current_translation_();
  S21MatrixWrapper apply_scale_fps_(double scale_val);
  S21Matrix get_proj_matrix_();
  S21Matrix get_proj_matrix_temp_(double far);
  S21Matrix update_proj_orth_temp_(double far);
  S21Matrix update_proj_persp_temp_(double far);
};

}  // namespace s21

#endif