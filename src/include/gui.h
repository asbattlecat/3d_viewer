#ifndef GUI_H
#define GUI_H

#include "3dmodel.h"
#include "affine.h"
#include "include_common.h"
#include "include_gui_common.h"
#include "viewer_memento.h"

namespace s21 {

class GLWidget;

class MainWindow : public QMainWindow {
  Q_OBJECT
 private:
  GLWidget *gl_area_;  // поле для отрисовки модели

  /*
    поля для ввода данных для преобразований модели
  */
  QLineEdit *x_coord_transl_;  // поле для ввода перемещения по оси X
  QLineEdit *y_coord_transl_;  // для перемещения по оси Y
  QLineEdit *z_coord_transl_;  // перемещение по оси Z

  QLineEdit *rotation_angle_;  // поле для угла поворота
  QLineEdit *_axis_;           // поле для выбора оси

  QLineEdit *scale_value_;  // поле для scale преоразования

  /*
    кнопки для изменения состояний
  */
  QPushButton *load_model_button_;  // кнопка для загрузки модели

  QPushButton *apply_trans_button_;  // кнопка для принятия преобразований по
                                     // перемещениям в работу
  QPushButton *apply_rotation_button_;  // кнопка для принятия преобразований по
                                        // повороту в работу
  QPushButton *apply_scale_button_;  // кнопка для принятия преобразований по
                                     // масштабированию в работу

  QPushButton *cl_trans_input_button_;  // очистка поля ввода для перемещений
  QPushButton *cl_rotation_input_button_;  // очистка поля ввода для поворота
  QPushButton
      *cl_scale_input_button_;  // очистка поля ввода для масштабирования

  QPushButton *change_projection_button_;  // изменение типа проекции
                                           // (параллельная или центральная)

  /*
    здесь данные для анимации перемещений
  */
  const double frame_duration_ = 20.0 / 1000.0;
  const double frame_interval_ = 20;  // 50 fps
  double animation_progress_;  // что-то вроде обозначения текущего прогресса по
                               // изменению модели

  /*
    тут таймеры для перемещения, вращения, масштабирования
  */
  QTimer *transl_animation_timer_;  // таймер для анимации преобразований модели
  QTimer *rot_animation_timer_;
  QTimer *scale_animation_timer_;

  /*
    данные о конечных значениях преобразований (нужно для анимации
    преобразований)
  */
  Vector3D target_translation_;  // то, куда надо переместить

  // double current_angle_; // текущий угол, на который повернули модель
  double target_angle_;  // конечный угол поворота модели
  Vector3D target__axis_;  // ось, вокруг которой вращаем

  double current_scale_;
  double target_scale_;  // конечный коэф. масштабирования
  int scale_iteration_;

  /*
    Данные о модели: кол-во вершин, ребер, название файла
  */
  QLabel *file_name_;
  QLabel *count_info_;

  /*
    кнопки  для изменения цвета линий
  */
  QPushButton *line_color_black_;
  QPushButton *line_color_white_;
  QPushButton *line_color_red_;
  QPushButton *line_color_green_;
  QPushButton *line_color_blue_;

  /*
    кнопки для изменения цвета фона
  */
  QPushButton *back_color_black_;
  QPushButton *back_color_white_;
  QPushButton *back_color_red_;
  QPushButton *back_color_green_;
  QPushButton *back_color_blue_;

  /*
    изменение толщины линий
  */
  QPushButton *width_line_one_;
  QPushButton *width_line_two_;
  QPushButton *width_line_three_;
  QPushButton *width_line_four_;

  /*
    изменение типа отображени линий (пунктир или солид)
  */
  QPushButton *solid_line_;
  QPushButton *stipple_line_;

  /*
    изменение толщины отобажаемой вершины
  */
  QPushButton *vertex_size_one_;
  QPushButton *vertex_size_two_;
  QPushButton *vertex_size_three_;
  QPushButton *vertex_size_four_;

  /*
    изменение вида отображения вершин
  */
  QPushButton *round_vertex_;
  QPushButton *square_vertex_;
  QPushButton *no_vertex_;

  /*
    кнопки для выбора цвета отображения вершин
  */
  QPushButton *vertex_color_black_;
  QPushButton *vertex_color_white_;
  QPushButton *vertex_color_red_;
  QPushButton *vertex_color_green_;
  QPushButton *vertex_color_blue_;

  /*
    кнопки для создания медиа (gif, скрины)
  */
  QPushButton *take_screenshot_button_;
  QPushButton *record_gif_button_;
  QTimer *gif_shot_timer_;
  QString gif_frames_path_ =
      QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) +
      "/3DViewer_gif_frames";
  QString gif_result_path_;

  const double shot_duration_ = 1.0 / 10.0;
  double target_frames_ = 50.0;
  double current_frames_ = 0.0;
  QString gif_path_;
  const double frame_gif_interval_ = 100.0;  // 10 fps
  const double anim_delay_ = 10.0;  // Magick++ использует сотые доли секунды
  double gif_progress_;  // обозначение прогресса по созданию gif (сбор кадров
                         // (50))

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void SetModelData(const Model3DDataGl &gl_data);
  void SetMVPMatrix(const S21MatrixWrapper &mvp);
  void SetModelInfo(const std::string &file_name, size_t edges,
                    size_t vertices);

  void SetMemento(GLWidgetMemento &memento);
  GLWidgetMemento CreateMemento();

 signals:
  void ApplyTranslationClicked(QString tx_s, QString ty_s, QString tz_s);
  void ProcessTranslation_fps(double tx, double ty, double tz);

  void ApplyRotationClicked(QString angle_s, QString _axis_s);
  void ProcessRotation_nofps(QString angle_s, QString _axis_s);
  void ProcessRotation_fps(double angle, Vector3D &_axis);

  void ApplyScaleClicked(QString scale_value_s);
  void ProcessScale_nofps(QString scale_val_s);
  void ProcessScaling_fps(double scale_value);

  void ProcessObjLoad(std::string &file_path);

  void ProcessChangeProjection();

  void AppAboutToQuit();

  void CreateGifFromJpegs(int target_frames, QString &gif_fr_path,
                          QString gif_target_path, double anim_delay);

 public slots:
  void StartTranslModel_fps(double tx, double ty, double tz);
  void StartRotationModel_fps(double angle, Vector3D &axis);
  void StartScaleModel_fps(double scale_value);

 protected:
  void closeEvent(QCloseEvent *event) override;

 private slots:

  void apply_trans_butt_clicked_();
  void apply_rotatation_butt_clicked_();
  void apply_scale_butt_clicked_();

  void clear_translation_inputs_();
  void clear_rotation_inputs_();
  void clear_scale_inputs_();

  void change_projection_clicked_();

  void update_transl_animation_();
  void update_rotatin_animation_();
  void update_scale_animation_();

  void change_line_color_();
  void change_line_width_();
  void change_line_type_();
  void change_back_color_();

  void change_vertex_color_();
  void change_vertex_size_();
  void change_vertex_visual_();

  void take_screenshot_();
  void record_gif_anim_();
  void take_picture_gif_();
  void obj_load_clicked_();

 private:
  void connect_all_();

  void connect_timers_();
  void connect_apply_transform_();
  void connect_clear_line_edits_();
  void connect_line_color_buttons_();
  void connect_line_width_buttons_();
  void connect_line_type_buttons_();
  void connect_back_color_buttons_();
  void connect_vertex_color_buttons_();
  void connect_vertex_size_button_();
  void connect_vertex_visual_();
  void connect_media_buttons_();

  void setup_ui_();
  void setup_load_model_button_(QVBoxLayout *layout);
  void setup_translation_(QVBoxLayout *layout);
  void setup_rotation_(QVBoxLayout *layout);
  void setup_scale_(QVBoxLayout *layout);
  void setup_line_color_buttons_(QVBoxLayout *layout);
  void setup_background_color_buttons_(QVBoxLayout *layout);
  void setup_line_width_buttons_(QVBoxLayout *layout);
  void setup_line_type_buttons_(QVBoxLayout *layout);
  void setup_vertex_color_buttons_(QVBoxLayout *layout);
  void setup_vertex_size_buttons_(QVBoxLayout *layout);
  void setup_vertex_visual_(QVBoxLayout *layout);
  void setup_projection_button_(QVBoxLayout *layout);

  void setup_model_info_(QVBoxLayout *layout);

  void setup_media_buttons_(QVBoxLayout *layout);

  void set_input_validation_();

  void check_timers_();
  bool need_to_stop_();

  void stop_scale_animation_();

  void clear_inputs_();  // очистка всех полей ввода

  void check_gif_pics_dir_();

  void create_gif_from_jpegs_();
  void add_jpegs_to_group_(std::vector<Magick::Image> &images);
  void clear_jpegs_for_gif_();

  void stop_gif_creating_();
};

/**
 * @brief класс для работы с OpenGL. Здесь представлено переопределение методов
 * initializeGL(), paintGL(), resizeGL() из QOpenGLWidget. Эти методы
 * автомотически вызываются Qt в нужные моменты (инициализация, перерисовка,
 * изменение размера окна)
 */
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_0 {
  Q_OBJECT
 private:
  GLuint vbo_;  // буффер Vertex Buffer Object
  GLuint ebo_;  // буффер Element Buffer Object для ребер
  std::vector<float> vertices_;
  std::vector<unsigned int> edges_;

  GLuint shader_program_;  // программа шейдера
  GLint mvp_location_;     // локация uniform-переменной MVP

  S21MatrixWrapper mvp_matrix_;
  /*
    данные для изменения отображения линий, вершин (цвет, толщина, форма)
  */
  GLint line_color_location_;
  QVector4D line_color_;
  QVector4D back_color_;
  GLfloat line_width_;

  bool is_line_solid_;
  GLushort stipple_pattern_;
  GLint stipple_factor_;

  GLfloat point_vertex_size_;
  bool show_point_vertex_;
  bool is_point_vertex_round_;

  GLint point_vertex_color_location_;
  GLint show_point_vertex_location_;
  GLint is_point_vertex_round_location_;

  QVector4D point_vertex_color_;

 public:
  GLWidget(QWidget *parent);
  void SetModelData(const std::vector<float> &vertices,
                    const std::vector<unsigned int> &edges);
  void SetMVPMatrix(const S21MatrixWrapper &mvp);
  void SetLineColor(QVector4D &line_color);
  void SetBackgroundColor(QVector4D &back_color);
  void SetLineWidth(GLfloat &width);
  void SetLineSolid();
  void SetLineStipple(GLushort pattern, GLint factor);
  void SetRoundVertex();
  void SetSquareVertex();
  void SetNoVertexDisplay();
  void SetVertexSize(GLfloat &size);
  bool IsVertexRound();
  void SetVertexColor(QVector4D &color);

  void SetMemento(GLWidgetMemento &memento);
  GLWidgetMemento CreateMemento();

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;
  void resizeEvent(QResizeEvent *event) override;

 private:
  void setup_shaders_();
};

}  // namespace s21

#endif