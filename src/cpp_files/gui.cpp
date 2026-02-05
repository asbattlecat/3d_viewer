#include "../include/gui.h"

namespace s21 {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  connect_timers_();
  gl_area_ = new GLWidget(this);
  setup_ui_();
  connect_all_();
  Magick::InitializeMagick(nullptr);
}

MainWindow::~MainWindow() {}

void MainWindow::SetModelData(const Model3DDataGl &gl_data) {
  gl_area_->SetModelData(gl_data.vertices, gl_data.edges);
  gl_area_->update();
}

void MainWindow::SetMVPMatrix(const S21MatrixWrapper &mvp) {
  gl_area_->SetMVPMatrix(mvp);
  gl_area_->update();
}

void MainWindow::SetModelInfo(const std::string &file_name, size_t edges,
                              size_t vertices) {
  QString edges_count = QString::number(edges);
  QString vertices_count = QString::number(vertices);
  file_name_->setText("File name: " + QString::fromStdString(file_name));
  count_info_->setText("Vertices: " + vertices_count +
                       "; edges: " + edges_count);
}

void MainWindow::SetMemento(GLWidgetMemento &memento) {
  gl_area_->SetMemento(memento);
}

GLWidgetMemento MainWindow::CreateMemento() {
  return gl_area_->CreateMemento();
}

/**
 * @brief обработка нужных данных перед закрытием приложения
 */
void MainWindow::closeEvent(QCloseEvent *event) {
  qDebug() << "App is closing, saving state";
  emit AppAboutToQuit();
  event->accept();
  QMainWindow::closeEvent(event);
}

////////////////////////////////////////////////////////////////////////////////
// слоты

void MainWindow::StartTranslModel_fps(double tx, double ty, double tz) {
  target_translation_ = {tx, ty, tz};
  animation_progress_ = 0.0;
  transl_animation_timer_->start(frame_interval_);
}

void MainWindow::StartRotationModel_fps(double angle, Vector3D &axis) {
  target_angle_ = angle;
  // current_angle_ = 0.0;
  target__axis_ = axis;
  animation_progress_ = 0.0;
  rot_animation_timer_->start(frame_interval_);
}

void MainWindow::StartScaleModel_fps(double scale_value) {
  target_scale_ = scale_value;
  scale_iteration_ = 0;
  current_scale_ = 1.0;
  animation_progress_ = 0.0;
  scale_animation_timer_->start(frame_interval_);
}

void MainWindow::update_transl_animation_() {
  Vector3D step = target_translation_ * frame_duration_;
  animation_progress_ += frame_duration_;
  emit ProcessTranslation_fps(step.x, step.y, step.z);
  if (need_to_stop_()) {
    transl_animation_timer_->stop();
  }
}

void MainWindow::update_rotatin_animation_() {
  animation_progress_ += frame_duration_;
  double step = target_angle_ * frame_duration_;
  // current_angle_ += step;
  emit ProcessRotation_fps(step, target__axis_);
  if (need_to_stop_()) {
    rot_animation_timer_->stop();
  }
}

void MainWindow::update_scale_animation_() {
  ++scale_iteration_;
  animation_progress_ += frame_duration_;
  double t = scale_iteration_ * frame_duration_;
  current_scale_ = 1.0 + (target_scale_ - 1.0) * t;
  emit ProcessScaling_fps(current_scale_);
  if (need_to_stop_()) {
    stop_scale_animation_();
  }
}

void MainWindow::apply_trans_butt_clicked_() {
  check_timers_();
  emit ApplyTranslationClicked(x_coord_transl_->text(), y_coord_transl_->text(),
                               z_coord_transl_->text());
}

void MainWindow::apply_rotatation_butt_clicked_() {
  check_timers_();
  emit ApplyRotationClicked(rotation_angle_->text(), _axis_->text());
}

void MainWindow::apply_scale_butt_clicked_() {
  check_timers_();
  emit ApplyScaleClicked(scale_value_->text());
}

void MainWindow::clear_translation_inputs_() {
  QList<QLineEdit *> inputs = {x_coord_transl_, y_coord_transl_,
                               z_coord_transl_};
  for (auto *input : inputs) {
    input->clear();
  }
}

void MainWindow::clear_rotation_inputs_() {
  QList<QLineEdit *> inputs = {rotation_angle_, _axis_};
  for (auto *input : inputs) {
    input->clear();
  }
}

void MainWindow::change_projection_clicked_() {
  emit ProcessChangeProjection();
}

void MainWindow::clear_scale_inputs_() { scale_value_->clear(); }

void MainWindow::check_timers_() {
  if (transl_animation_timer_->isActive()) transl_animation_timer_->stop();
  if (rot_animation_timer_->isActive()) rot_animation_timer_->stop();
  if (scale_animation_timer_->isActive()) stop_scale_animation_();
}

bool MainWindow::need_to_stop_() {
  bool result = false;
  if (animation_progress_ > 1.0) {
    animation_progress_ = 1.0;
    result = true;
  }
  return result;
}

void MainWindow::stop_scale_animation_() {
  scale_animation_timer_->stop();
  QString scale = QString::number(target_scale_);
  emit ProcessScale_nofps(scale);
}

///////////////////////////////////////////////////
// смена цвета линий

void MainWindow::change_line_color_() {
  QObject *button = sender();
  QVector4D color;
  if (button == line_color_black_) {
    color = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);
  } else if (button == line_color_white_) {
    color = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
  } else if (button == line_color_red_) {
    color = QVector4D(1.0f, 0.0f, 0.0f, 1.0f);
  } else if (button == line_color_green_) {
    color = QVector4D(0.2f, 0.3f, 0.3f, 1.0f);
  } else {
    color = QVector4D(0.0f, 0.0f, 1.0f, 1.0f);
  }
  gl_area_->SetLineColor(color);
  gl_area_->update();
}

///////////////////////////////////////////////////
// смена толщины линий

void MainWindow::change_line_width_() {
  QObject *button = sender();
  GLfloat num;
  if (button == width_line_one_) {
    num = GLfloat(1.0f);
  } else if (button == width_line_two_) {
    num = GLfloat(3.0f);
  } else if (button == width_line_three_) {
    num = GLfloat(5.0f);
  } else if (button == width_line_four_) {
    num = GLfloat(7.0f);
  }
  gl_area_->SetLineWidth(num);
  gl_area_->update();
}

///////////////////////////////////////////////////
// смена типа линий (сплошная, пунктирная)

void MainWindow::change_line_type_() {
  QObject *button = sender();
  if (button == solid_line_) {
    gl_area_->SetLineSolid();
  } else {
    gl_area_->SetLineStipple(0xAAAA, 8);
  }
  gl_area_->update();
}

///////////////////////////////////////////////////
// смена цвета заднего плана

void MainWindow::change_back_color_() {
  QObject *button = sender();
  QVector4D color;
  if (button == back_color_black_) {
    color = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);
  } else if (button == back_color_white_) {
    color = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
  } else if (button == back_color_red_) {
    color = QVector4D(1.0f, 0.0f, 0.0f, 1.0f);
  } else if (button == back_color_green_) {
    color = QVector4D(0.2f, 0.3f, 0.3f, 1.0f);
  } else {
    color = QVector4D(0.0f, 0.0f, 1.0f, 1.0f);
  }
  gl_area_->SetBackgroundColor(color);
  gl_area_->update();
}

//////////////////////////////////////////////////
// изменение размера, цвета и вида вершин

void MainWindow::change_vertex_color_() {
  QObject *button = sender();
  QVector4D color;
  if (button == vertex_color_black_) {
    color = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);
  } else if (button == vertex_color_white_) {
    color = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
  } else if (button == vertex_color_red_) {
    color = QVector4D(1.0f, 0.0f, 0.0f, 1.0f);
  } else if (button == vertex_color_green_) {
    color = QVector4D(0.2f, 0.3f, 0.3f, 1.0f);
  } else {
    color = QVector4D(0.0f, 0.0f, 1.0f, 1.0f);
  }
  gl_area_->SetVertexColor(color);
  gl_area_->update();
}

void MainWindow::change_vertex_size_() {
  QObject *button = sender();
  GLfloat size;
  if (button == vertex_size_one_) {
    size = GLfloat(3.0f);
  } else if (button == vertex_size_two_) {
    size = GLfloat(6.0f);
  } else if (button == vertex_size_three_) {
    size = GLfloat(9.0f);
  } else if (button == vertex_size_four_) {
    size = GLfloat(12.0f);
  }
  gl_area_->SetVertexSize(size);
  gl_area_->update();
}

void MainWindow::change_vertex_visual_() {
  QObject *button = sender();
  if (button == round_vertex_) {
    gl_area_->SetRoundVertex();
  } else if (button == square_vertex_) {
    gl_area_->SetSquareVertex();
  } else {
    gl_area_->SetNoVertexDisplay();
  }
  gl_area_->update();
}

void MainWindow::take_screenshot_() {
  // для забора расширения файла
  QString selected_filt;
  // запись пользователем куда сохранять, задача имени фала и расширения
  QString file_path = QFileDialog::getSaveFileName(
      this, "Select file save location", "", "JPEG (*.jpeg);;  BMP (*.bmp)",
      &selected_filt);
  if (!file_path.isEmpty()) {
    // изображение захватили с окна GLWidget
    QPixmap screen_image = gl_area_->grab();
    // разбиваем на части
    QFileInfo file_info(file_path);
    QString path = file_info.path();
    QString file_name = file_info.completeBaseName();
    QString extension;

    // записали расширение в стринг
    if (selected_filt.contains(".jpeg")) {
      extension = "jpeg";
    } else if (selected_filt.contains(".bmp")) {
      extension = "bmp";
    }

    QString file =
        QString("%1/Screenshot_%2.%3").arg(path, file_name, extension);

    screen_image.save(file, extension.toUpper().toStdString().c_str());
    qDebug() << "Screenshot saved to:" << file;
  }
}

void MainWindow::record_gif_anim_() {
  if (gif_shot_timer_->isActive()) {
    stop_gif_creating_();
  }
  check_gif_pics_dir_();
  current_frames_ = 0;
  target_frames_ = 50;
  QString selected_filt;
  gif_path_.clear();
  QString file_path = QFileDialog::getSaveFileName(
      this, "Select file save location", "", "GIF (*.gif)", &selected_filt);
  if (!file_path.isEmpty()) {
    QFileInfo file_info(file_path);
    QString path = file_info.path();
    QString file_name = file_info.completeBaseName();
    gif_path_ = QString("%1/%2.gif").arg(path, file_name);
    record_gif_button_->setStyleSheet("background-color: red");
    gif_shot_timer_->start(frame_gif_interval_);
  }
}

void MainWindow::take_picture_gif_() {
  ++current_frames_;
  QString fr_count = QString::number(static_cast<int>(current_frames_));
  // формирования названия файла (окончание на номер фрейма)
  QString file = QString("%1/Screenshot_for_gif_%2%3")
                     .arg(gif_frames_path_, fr_count, ".jpeg");

  QPixmap screen_image = gl_area_->grab();
  screen_image.save(file, "JPEG");
  if (current_frames_ >= target_frames_) {
    stop_gif_creating_();
  }
}

void MainWindow::obj_load_clicked_() {
  // диалоговое окно для выбора файла
  QString file_path = QFileDialog::getOpenFileName(this, "Select .obj file", "",
                                                   ".obj files (*.obj)");

  if (!file_path.isEmpty()) {  // был ли выбран файл
    qDebug() << "Selected file:" << file_path;
    std::string path = file_path.toStdString();
    emit ProcessObjLoad(path);
  } else {  // отмена выбора
    qDebug() << "No file selected";
  }
}

////////////////////////////////////////////////////////////////////////////////
// соединение сигналов от кнопок и слотов для их обработки, а также сигналов от
// таймера и обработки истечения таймера

void MainWindow::connect_all_() {
  // для загрузки модели соединение кнопки и нажатия
  connect(load_model_button_, &QPushButton::clicked, this,
          &MainWindow::obj_load_clicked_);

  // кнопки для внесения изменений в модель
  connect_apply_transform_();
  connect_clear_line_edits_();

  // кнопки изменения визуальной составляющей рендеринга
  connect_line_color_buttons_();
  connect_line_width_buttons_();
  connect_line_type_buttons_();
  connect_back_color_buttons_();
  connect_vertex_color_buttons_();
  connect_vertex_size_button_();
  connect_vertex_visual_();

  connect_media_buttons_();
}

void MainWindow::connect_timers_() {
  transl_animation_timer_ = new QTimer(this);
  connect(transl_animation_timer_, &QTimer::timeout, this,
          &MainWindow::update_transl_animation_);

  rot_animation_timer_ = new QTimer(this);
  connect(rot_animation_timer_, &QTimer::timeout, this,
          &MainWindow::update_rotatin_animation_);

  scale_animation_timer_ = new QTimer(this);
  connect(scale_animation_timer_, &QTimer::timeout, this,
          &MainWindow::update_scale_animation_);

  gif_shot_timer_ = new QTimer(this);
  connect(gif_shot_timer_, &QTimer::timeout, this,
          &MainWindow::take_picture_gif_);
}

void MainWindow::connect_apply_transform_() {
  connect(apply_trans_button_, &QPushButton::clicked, this,
          &MainWindow::apply_trans_butt_clicked_);

  connect(apply_rotation_button_, &QPushButton::clicked, this,
          &MainWindow::apply_rotatation_butt_clicked_);

  connect(apply_scale_button_, &QPushButton::clicked, this,
          &MainWindow::apply_scale_butt_clicked_);

  connect(change_projection_button_, &QPushButton::clicked, this,
          &MainWindow::change_projection_clicked_);
}

void MainWindow::connect_clear_line_edits_() {
  connect(cl_trans_input_button_, &QPushButton::clicked, this,
          &MainWindow::clear_translation_inputs_);
  connect(cl_rotation_input_button_, &QPushButton::clicked, this,
          &MainWindow::clear_rotation_inputs_);
  connect(cl_scale_input_button_, &QPushButton::clicked, this,
          &MainWindow::clear_scale_inputs_);
}

void MainWindow::connect_line_color_buttons_() {
  connect(line_color_black_, &QPushButton::clicked, this,
          &MainWindow::change_line_color_);
  connect(line_color_white_, &QPushButton::clicked, this,
          &MainWindow::change_line_color_);
  connect(line_color_red_, &QPushButton::clicked, this,
          &MainWindow::change_line_color_);
  connect(line_color_green_, &QPushButton::clicked, this,
          &MainWindow::change_line_color_);
  connect(line_color_blue_, &QPushButton::clicked, this,
          &MainWindow::change_line_color_);
}

void MainWindow::connect_line_width_buttons_() {
  connect(width_line_one_, &QPushButton::clicked, this,
          &MainWindow::change_line_width_);
  connect(width_line_two_, &QPushButton::clicked, this,
          &MainWindow::change_line_width_);
  connect(width_line_three_, &QPushButton::clicked, this,
          &MainWindow::change_line_width_);
  connect(width_line_four_, &QPushButton::clicked, this,
          &MainWindow::change_line_width_);
}

void MainWindow::connect_line_type_buttons_() {
  connect(solid_line_, &QPushButton::clicked, this,
          &MainWindow::change_line_type_);
  connect(stipple_line_, &QPushButton::clicked, this,
          &MainWindow::change_line_type_);
}

void MainWindow::connect_back_color_buttons_() {
  connect(back_color_black_, &QPushButton::clicked, this,
          &MainWindow::change_back_color_);
  connect(back_color_white_, &QPushButton::clicked, this,
          &MainWindow::change_back_color_);
  connect(back_color_red_, &QPushButton::clicked, this,
          &MainWindow::change_back_color_);
  connect(back_color_green_, &QPushButton::clicked, this,
          &MainWindow::change_back_color_);
  connect(back_color_blue_, &QPushButton::clicked, this,
          &MainWindow::change_back_color_);
}

void MainWindow::connect_vertex_color_buttons_() {
  connect(vertex_color_black_, &QPushButton::clicked, this,
          &MainWindow::change_vertex_color_);
  connect(vertex_color_white_, &QPushButton::clicked, this,
          &MainWindow::change_vertex_color_);
  connect(vertex_color_red_, &QPushButton::clicked, this,
          &MainWindow::change_vertex_color_);
  connect(vertex_color_green_, &QPushButton::clicked, this,
          &MainWindow::change_vertex_color_);
  connect(vertex_color_blue_, &QPushButton::clicked, this,
          &MainWindow::change_vertex_color_);
}

void MainWindow::connect_vertex_size_button_() {
  connect(vertex_size_one_, &QPushButton::clicked, this,
          &MainWindow::change_vertex_size_);
  connect(vertex_size_two_, &QPushButton::clicked, this,
          &MainWindow::change_vertex_size_);
  connect(vertex_size_three_, &QPushButton::clicked, this,
          &MainWindow::change_vertex_size_);
  connect(vertex_size_four_, &QPushButton::clicked, this,
          &MainWindow::change_vertex_size_);
}

void MainWindow::connect_vertex_visual_() {
  connect(round_vertex_, &QPushButton::clicked, this,
          &MainWindow::change_vertex_visual_);
  connect(square_vertex_, &QPushButton::clicked, this,
          &MainWindow::change_vertex_visual_);
  connect(no_vertex_, &QPushButton::clicked, this,
          &MainWindow::change_vertex_visual_);
}

void MainWindow::connect_media_buttons_() {
  connect(take_screenshot_button_, &QPushButton::clicked, this,
          &MainWindow::take_screenshot_);
  connect(record_gif_button_, &QPushButton::clicked, this,
          &MainWindow::record_gif_anim_);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// ui составляющие

void MainWindow::setup_ui_() {
  QWidget *central_widget = new QWidget(this);
  QHBoxLayout *main_layout = new QHBoxLayout(central_widget);

  gl_area_->setMinimumSize(1000, 750);
  main_layout->addWidget(gl_area_, 1);

  QWidget *control_widget = new QWidget(this);
  QVBoxLayout *control_layout = new QVBoxLayout(control_widget);

  setup_load_model_button_(control_layout);
  setup_translation_(control_layout);
  setup_rotation_(control_layout);
  setup_scale_(control_layout);
  control_layout->addStretch();

  setup_line_color_buttons_(control_layout);
  setup_line_width_buttons_(control_layout);
  setup_line_type_buttons_(control_layout);
  setup_background_color_buttons_(control_layout);
  setup_vertex_color_buttons_(control_layout);
  setup_vertex_size_buttons_(control_layout);
  setup_vertex_visual_(control_layout);

  setup_model_info_(control_layout);
  setup_media_buttons_(control_layout);
  setup_projection_button_(control_layout);

  set_input_validation_();

  main_layout->addWidget(control_widget);

  setCentralWidget(central_widget);
  setWindowTitle("3D Viewer");
}

void MainWindow::setup_load_model_button_(QVBoxLayout *layout) {
  load_model_button_ = new QPushButton("Load model", this);
  layout->addWidget(load_model_button_);
}

/**
 * @brief метод для расстановки лэйблов и строк для вноса данных для переноса
 * фигуры
 */
void MainWindow::setup_translation_(QVBoxLayout *layout) {
  layout->addWidget(new QLabel("Translate model", this));
  QHBoxLayout *trans_layout = new QHBoxLayout();
  trans_layout->addWidget(new QLabel("X:", this));
  x_coord_transl_ = new QLineEdit(this);
  x_coord_transl_->setPlaceholderText("Enter X");
  trans_layout->addWidget(x_coord_transl_);

  trans_layout->addWidget(new QLabel("Y:", this));
  y_coord_transl_ = new QLineEdit(this);
  y_coord_transl_->setPlaceholderText("Enter Y");
  trans_layout->addWidget(y_coord_transl_);

  trans_layout->addWidget(new QLabel("Z:", this));
  z_coord_transl_ = new QLineEdit(this);
  z_coord_transl_->setPlaceholderText("Enter Z");
  trans_layout->addWidget(z_coord_transl_);

  layout->addLayout(trans_layout);

  QHBoxLayout *button_layout = new QHBoxLayout();
  cl_trans_input_button_ = new QPushButton("Clear", this);
  button_layout->addWidget(cl_trans_input_button_);
  button_layout->addStretch();  // Растягиваем, чтобы кнопка Apply была справа
  apply_trans_button_ = new QPushButton("Apply translation", this);
  button_layout->addWidget(apply_trans_button_);

  layout->addLayout(button_layout);
}

/**
 * @brief метод для расстановки лэйблов и строк для вноса данных для вращения
 * фигуры
 */
void MainWindow::setup_rotation_(QVBoxLayout *layout) {
  layout->addWidget(new QLabel("Rotate model", this));
  QHBoxLayout *rot_layout = new QHBoxLayout();
  rot_layout->addWidget(new QLabel("Angle:", this));
  rotation_angle_ = new QLineEdit(this);
  rotation_angle_->setPlaceholderText("Enter angle");
  rot_layout->addWidget(rotation_angle_);

  rot_layout->addWidget(new QLabel("Axis", this));
  _axis_ = new QLineEdit(this);
  _axis_->setPlaceholderText("Enter X/Y/Z");
  rot_layout->addWidget(_axis_);

  layout->addLayout(rot_layout);

  QHBoxLayout *button_layout = new QHBoxLayout();
  cl_rotation_input_button_ = new QPushButton("Clear", this);
  button_layout->addWidget(cl_rotation_input_button_);
  button_layout->addStretch();
  apply_rotation_button_ = new QPushButton("Apply rotation", this);
  button_layout->addWidget(apply_rotation_button_);

  layout->addLayout(button_layout);
}

/**
 * метод для настройки кнопок по масштабированию
 */
void MainWindow::setup_scale_(QVBoxLayout *layout) {
  layout->addWidget(new QLabel("Scale model", this));
  QHBoxLayout *scale_layout = new QHBoxLayout();
  scale_layout->addWidget(new QLabel("Scale", this));
  scale_value_ = new QLineEdit(this);
  scale_value_->setPlaceholderText("Scale value");
  scale_layout->addWidget(scale_value_);

  layout->addLayout(scale_layout);

  QHBoxLayout *button_layout = new QHBoxLayout();
  cl_scale_input_button_ = new QPushButton("Clear", this);
  button_layout->addWidget(cl_scale_input_button_);
  button_layout->addStretch();
  apply_scale_button_ = new QPushButton("Apply scale", this);
  button_layout->addWidget(apply_scale_button_);

  layout->addLayout(button_layout);
}

/**
 * @brief метод для настройки кнопок по изменению цвета линий
 */
void MainWindow::setup_line_color_buttons_(QVBoxLayout *layout) {
  QLabel *line_color = new QLabel("Line color:", this);
  layout->addWidget(line_color);

  QHBoxLayout *color_buttons = new QHBoxLayout();

  // для черной кнопки
  line_color_black_ = new QPushButton(this);
  QPalette palette_bl = line_color_black_->palette();
  palette_bl.setColor(QPalette::Button, Qt::black);  // черный цвет кнопки
  line_color_black_->setPalette(palette_bl);
  line_color_black_->setFixedSize(40, 40);

  // для белой кнопки
  line_color_white_ = new QPushButton(this);
  QPalette palette_wh = line_color_white_->palette();
  palette_wh.setColor(QPalette::Button, Qt::white);
  line_color_white_->setPalette(palette_wh);
  line_color_white_->setFixedSize(40, 40);

  // для красной кнопки
  line_color_red_ = new QPushButton(this);
  QPalette palette_red = line_color_red_->palette();
  palette_red.setColor(QPalette::Button, Qt::red);
  line_color_red_->setPalette(palette_red);
  line_color_red_->setFixedSize(40, 40);

  // зеленый цвет
  line_color_green_ = new QPushButton(this);
  QPalette palette_gr = line_color_green_->palette();
  palette_gr.setColor(QPalette::Button, QColor(51, 77, 77));
  line_color_green_->setPalette(palette_gr);
  line_color_green_->setFixedSize(40, 40);

  // для синей кнопки
  line_color_blue_ = new QPushButton(this);
  QPalette palette_blue = line_color_blue_->palette();
  palette_blue.setColor(QPalette::Button, Qt::blue);
  line_color_blue_->setPalette(palette_blue);
  line_color_blue_->setFixedSize(40, 40);

  color_buttons->addWidget(line_color_black_);
  color_buttons->addWidget(line_color_white_);
  color_buttons->addWidget(line_color_red_);
  color_buttons->addWidget(line_color_green_);
  color_buttons->addWidget(line_color_blue_);
  color_buttons->setAlignment(Qt::AlignLeft);

  layout->addLayout(color_buttons);
}

/**
 * @brief метод для настройки кнопок по изменению цвета заднего фона
 */
void MainWindow::setup_background_color_buttons_(QVBoxLayout *layout) {
  QLabel *back_color = new QLabel("Background color:", this);
  layout->addWidget(back_color);

  QHBoxLayout *color_buttons = new QHBoxLayout();

  // для черной кнопки
  back_color_black_ = new QPushButton(this);
  QPalette palette_bl = back_color_black_->palette();
  palette_bl.setColor(QPalette::Button, Qt::black);  // черный цвет кнопки
  back_color_black_->setPalette(palette_bl);
  back_color_black_->setFixedSize(40, 40);

  // для белой кнопки
  back_color_white_ = new QPushButton(this);
  QPalette palette_wh = back_color_white_->palette();
  palette_wh.setColor(QPalette::Button, Qt::white);
  back_color_white_->setPalette(palette_wh);
  back_color_white_->setFixedSize(40, 40);

  // для красной кнопки
  back_color_red_ = new QPushButton(this);
  QPalette palette_red = back_color_red_->palette();
  palette_red.setColor(QPalette::Button, Qt::red);
  back_color_red_->setPalette(palette_red);
  back_color_red_->setFixedSize(40, 40);

  // зелени кнёпкя
  back_color_green_ = new QPushButton(this);
  QPalette palette_gr = back_color_green_->palette();
  palette_gr.setColor(QPalette::Button, QColor(51, 77, 77));
  back_color_green_->setPalette(palette_gr);
  back_color_green_->setFixedSize(40, 40);

  // для синей кнопки
  back_color_blue_ = new QPushButton(this);
  QPalette palette_blue = back_color_blue_->palette();
  palette_blue.setColor(QPalette::Button, Qt::blue);
  back_color_blue_->setPalette(palette_blue);
  back_color_blue_->setFixedSize(40, 40);

  color_buttons->addWidget(back_color_black_);
  color_buttons->addWidget(back_color_white_);
  color_buttons->addWidget(back_color_red_);
  color_buttons->addWidget(back_color_green_);
  color_buttons->addWidget(back_color_blue_);
  color_buttons->setAlignment(Qt::AlignLeft);

  layout->addLayout(color_buttons);
}

/**
 * @brief метод для настройки кнопок по изменению толщины линий
 */
void MainWindow::setup_line_width_buttons_(QVBoxLayout *layout) {
  QLabel *width_text = new QLabel("Line width:", this);
  layout->addWidget(width_text);

  width_line_one_ = new QPushButton("1", this);
  width_line_one_->setFixedSize(30, 30);

  width_line_two_ = new QPushButton("2", this);
  width_line_two_->setFixedSize(30, 30);

  width_line_three_ = new QPushButton("3", this);
  width_line_three_->setFixedSize(30, 30);

  width_line_four_ = new QPushButton("4", this);
  width_line_four_->setFixedSize(30, 30);

  QHBoxLayout *h_layout = new QHBoxLayout();
  h_layout->addWidget(width_line_one_);
  h_layout->addWidget(width_line_two_);
  h_layout->addWidget(width_line_three_);
  h_layout->addWidget(width_line_four_);
  h_layout->setAlignment(Qt::AlignLeft);

  layout->addLayout(h_layout);
}

void MainWindow::setup_line_type_buttons_(QVBoxLayout *layout) {
  QLabel *type_text = new QLabel("Line type:", this);
  layout->addWidget(type_text);

  solid_line_ = new QPushButton("Solid", this);

  stipple_line_ = new QPushButton("Stipple", this);

  QHBoxLayout *h_layout = new QHBoxLayout();

  h_layout->addWidget(solid_line_);
  h_layout->addWidget(stipple_line_);
  h_layout->setAlignment(Qt::AlignLeft);

  layout->addLayout(h_layout);
}

void MainWindow::setup_vertex_color_buttons_(QVBoxLayout *layout) {
  QLabel *vertex_color = new QLabel("Vertex color:", this);
  layout->addWidget(vertex_color);

  QHBoxLayout *color_buttons = new QHBoxLayout();

  // для черной кнопки
  vertex_color_black_ = new QPushButton(this);
  QPalette palette_bl = vertex_color_black_->palette();
  palette_bl.setColor(QPalette::Button, Qt::black);  // черный цвет кнопки
  vertex_color_black_->setPalette(palette_bl);
  vertex_color_black_->setFixedSize(40, 40);

  // для белой кнопки
  vertex_color_white_ = new QPushButton(this);
  QPalette palette_wh = vertex_color_white_->palette();
  palette_wh.setColor(QPalette::Button, Qt::white);
  vertex_color_white_->setPalette(palette_wh);
  vertex_color_white_->setFixedSize(40, 40);

  // для красной кнопки
  vertex_color_red_ = new QPushButton(this);
  QPalette palette_red = vertex_color_red_->palette();
  palette_red.setColor(QPalette::Button, Qt::red);
  vertex_color_red_->setPalette(palette_red);
  vertex_color_red_->setFixedSize(40, 40);

  // зелени кнёпкя
  vertex_color_green_ = new QPushButton(this);
  QPalette palette_gr = vertex_color_green_->palette();
  palette_gr.setColor(QPalette::Button, QColor(51, 77, 77));
  vertex_color_green_->setPalette(palette_gr);
  vertex_color_green_->setFixedSize(40, 40);

  // для синей кнопки
  vertex_color_blue_ = new QPushButton(this);
  QPalette palette_blue = vertex_color_blue_->palette();
  palette_blue.setColor(QPalette::Button, Qt::blue);
  vertex_color_blue_->setPalette(palette_blue);
  vertex_color_blue_->setFixedSize(40, 40);

  color_buttons->addWidget(vertex_color_black_);
  color_buttons->addWidget(vertex_color_white_);
  color_buttons->addWidget(vertex_color_red_);
  color_buttons->addWidget(vertex_color_green_);
  color_buttons->addWidget(vertex_color_blue_);
  color_buttons->setAlignment(Qt::AlignLeft);

  layout->addLayout(color_buttons);
}

void MainWindow::setup_vertex_size_buttons_(QVBoxLayout *layout) {
  QLabel *vertex_size_text = new QLabel("Vertex size:", this);
  layout->addWidget(vertex_size_text);

  vertex_size_one_ = new QPushButton("1", this);
  vertex_size_one_->setFixedSize(30, 30);

  vertex_size_two_ = new QPushButton("2", this);
  vertex_size_two_->setFixedSize(30, 30);

  vertex_size_three_ = new QPushButton("3", this);
  vertex_size_three_->setFixedSize(30, 30);

  vertex_size_four_ = new QPushButton("4", this);
  vertex_size_four_->setFixedSize(30, 30);

  QHBoxLayout *h_layout = new QHBoxLayout();
  h_layout->addWidget(vertex_size_one_);
  h_layout->addWidget(vertex_size_two_);
  h_layout->addWidget(vertex_size_three_);
  h_layout->addWidget(vertex_size_four_);
  h_layout->setAlignment(Qt::AlignLeft);

  layout->addLayout(h_layout);
}

void MainWindow::setup_vertex_visual_(QVBoxLayout *layout) {
  QLabel *vertex_visual_text = new QLabel("Vertex display type:");
  layout->addWidget(vertex_visual_text);

  round_vertex_ = new QPushButton("Round", this);

  square_vertex_ = new QPushButton("Square", this);

  no_vertex_ = new QPushButton("No display", this);

  QHBoxLayout *h_layout = new QHBoxLayout();
  h_layout->addWidget(round_vertex_);
  h_layout->addWidget(square_vertex_);
  h_layout->addWidget(no_vertex_);

  h_layout->setAlignment(Qt::AlignLeft);

  layout->addLayout(h_layout);
}

/**
 * @brief метод для настройки отображения данных о модели (вершины, ребра,
 * название)
 */
void MainWindow::setup_model_info_(QVBoxLayout *layout) {
  QLabel *model_info = new QLabel("Model info:", this);
  layout->addWidget(model_info);
  file_name_ = new QLabel(this);
  layout->addWidget(file_name_);
  count_info_ = new QLabel(this);
  layout->addWidget(count_info_);
}

void MainWindow::setup_media_buttons_(QVBoxLayout *layout) {
  QHBoxLayout *h_layout = new QHBoxLayout();

  take_screenshot_button_ = new QPushButton("Take screenshot", this);
  record_gif_button_ = new QPushButton("Record gif", this);
  record_gif_button_->setStyleSheet("background-color: green");

  h_layout->addWidget(take_screenshot_button_);
  h_layout->addWidget(record_gif_button_);

  layout->addLayout(h_layout);
}

void MainWindow::setup_projection_button_(QVBoxLayout *layout) {
  change_projection_button_ = new QPushButton("Change projection type", this);
  layout->addWidget(change_projection_button_);
}

/**
 * @brief метод для задачи вида вводимых данных в поля
 */
void MainWindow::set_input_validation_() {
  x_coord_transl_->setValidator(new QDoubleValidator(-100.0, 100.0, 2, this));
  y_coord_transl_->setValidator(new QDoubleValidator(-100.0, 100.0, 2, this));
  z_coord_transl_->setValidator(new QDoubleValidator(-100.0, 100.0, 2, this));

  rotation_angle_->setValidator(new QDoubleValidator(-360.0, 360.0, 2, this));
  QRegularExpression axis_regular("^[xXyYzZ]$");
  _axis_->setValidator(new QRegularExpressionValidator(axis_regular, this));

  scale_value_->setValidator(new QDoubleValidator(0.01, 10.0, 3, this));
}

/**
 * @brief метод для обработки ввода пользователя в поля для ввода.
 */

void MainWindow::clear_inputs_() {
  QList<QLineEdit *> inputs = {x_coord_transl_, y_coord_transl_,
                               z_coord_transl_, rotation_angle_,
                               _axis_,          scale_value_};

  for (auto *input : inputs) {
    input->clear();
  }
}

void MainWindow::check_gif_pics_dir_() {
  QString scrshot_path = QDir::cleanPath(gif_frames_path_);
  QDir dir(scrshot_path);
  if (!dir.exists()) {
    dir.mkpath(scrshot_path);
    if (!dir.exists()) {
      qDebug() << "ERROR: MainWindow::check_gif_pics_dir_(): " + scrshot_path +
                      " dir was not created, error appeared";
    } else {
      qDebug() << "Directory created:" << scrshot_path;
    }
  } else {
    clear_jpegs_for_gif_();
  }
}

void MainWindow::create_gif_from_jpegs_() {
  if (!gif_path_.isEmpty()) {
    emit CreateGifFromJpegs(target_frames_, gif_frames_path_, gif_path_,
                            anim_delay_);
  }
}

void MainWindow::add_jpegs_to_group_(std::vector<Magick::Image> &images) {
  try {
    for (int i = 1; i <= target_frames_; ++i) {
      QString frames_count = QString::number(i);
      QString q_file_name = QString("%1/Screenshot_for_gif_%2%3")
                                .arg(gif_frames_path_, frames_count, ".jpeg");
      QFileInfo file_info(q_file_name);
      if (file_info.exists()) {
        Magick::Image image;
        image.read(q_file_name.toStdString());
        image.animationDelay(anim_delay_);
        images.push_back(image);
      } else {
        qDebug() << "File does not exists: " << q_file_name << ", skipped";
      }
    }
    for (auto it = images.begin(); it != images.end(); ++it) {
      it->resize(Magick::Geometry(640, 480));
      it->quantizeColors(256);
    }
    Magick::writeImages(images.begin(), images.end(), gif_path_.toStdString());
    qDebug() << "GIF successfully created, path:" << gif_path_;
    clear_jpegs_for_gif_();
  } catch (Magick::Exception &error) {
    qDebug() << "Error creating GIF:" << error.what();
  }
}

void MainWindow::clear_jpegs_for_gif_() {
  QDir dir(gif_frames_path_);
  QStringList filters;
  filters << "Screenshot_for_gif_*.jpeg";
  dir.setNameFilters(filters);
  for (const auto &file : dir.entryList()) {
    dir.remove(file);
  }
}

void MainWindow::stop_gif_creating_() {
  record_gif_button_->setStyleSheet("background-color: green");
  check_timers_();
  if (gif_shot_timer_->isActive()) gif_shot_timer_->stop();
  create_gif_from_jpegs_();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// реализация GLWidget
GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      vbo_(0),
      ebo_(0),
      shader_program_(0),
      mvp_matrix_(S21MatrixWrapper(4, 4)),
      line_color_(0.0f, 0.0f, 0.0f, 1.0f),
      back_color_(1.0f, 1.0f, 1.0f, 1.0f),
      line_width_(1.0f),
      is_line_solid_(true),
      stipple_pattern_(0xAAAA),
      stipple_factor_(1),
      point_vertex_size_(4.0f),
      show_point_vertex_(false),
      is_point_vertex_round_(true),
      point_vertex_color_(0.2f, 0.3f, 0.3f, 1.0f) {
  mvp_matrix_.SetIdentity();
}

void GLWidget::SetMVPMatrix(const S21MatrixWrapper &mvp) {
  mvp_matrix_ = mvp;
  update();
}

void GLWidget::SetLineColor(QVector4D &line_color) { line_color_ = line_color; }

void GLWidget::SetBackgroundColor(QVector4D &back_color) {
  back_color_ = back_color;
}

void GLWidget::SetLineWidth(GLfloat &width) { line_width_ = width; }

void GLWidget::SetLineSolid() { is_line_solid_ = true; }

void GLWidget::SetLineStipple(GLushort pattern, GLint factor) {
  is_line_solid_ = false;
  stipple_pattern_ = pattern;
  stipple_factor_ = factor;
}

void GLWidget::SetRoundVertex() {
  show_point_vertex_ = true;
  is_point_vertex_round_ = true;
}

void GLWidget::SetSquareVertex() {
  show_point_vertex_ = true;
  is_point_vertex_round_ = false;
}

void GLWidget::SetVertexSize(GLfloat &size) { point_vertex_size_ = size; }

bool GLWidget::IsVertexRound() { return is_point_vertex_round_; }

void GLWidget::SetNoVertexDisplay() { show_point_vertex_ = false; }

void GLWidget::SetVertexColor(QVector4D &color) { point_vertex_color_ = color; }

/**
 * @brief Метод для записи данных о фигуре и передачи этих данных OpenGL для
 * дальнейшей работы
 */
void GLWidget::SetModelData(const std::vector<float> &vertices,
                            const std::vector<unsigned int> &edges) {
  vertices_ = vertices;
  edges_ = edges;
  if (context()) {
    initializeOpenGLFunctions();

    // связываем данные о модели с буферами
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float),
                 vertices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, edges_.size() * sizeof(unsigned int),
                 edges_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}

void GLWidget::SetMemento(GLWidgetMemento &memento) {
  line_color_ = memento.GetLineColor();
  back_color_ = memento.GetBackColor();
  line_width_ = memento.GetLineWidth();
  is_line_solid_ = memento.IsLineSolid();
  stipple_pattern_ = memento.GetStipplePattern();
  stipple_factor_ = memento.GetStippleFactor();
  point_vertex_size_ = memento.GetPointVertexSize();
  show_point_vertex_ = memento.IsPointVertexDisplayed();
  is_point_vertex_round_ = memento.IsPointVertexRound();
  point_vertex_color_ = memento.GetPointVertexColor();
}

GLWidgetMemento GLWidget::CreateMemento() {
  return GLWidgetMemento(line_color_, back_color_, line_width_, is_line_solid_,
                         stipple_pattern_, stipple_factor_, point_vertex_size_,
                         show_point_vertex_, is_point_vertex_round_,
                         point_vertex_color_);
}

/**
 * @brief переопределение виртуальной функции из QOpenGLWidget. Вызывается один
 * раз при создании виджета. Инициализирует все, что нужно для рендеринга
 * (буфер, шейдеры, цвета) до начала отрисовки.
 */
void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(back_color_.x(), back_color_.y(), back_color_.z(),
               back_color_.w());
  glEnable(GL_DEPTH_TEST);

  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  setup_shaders_();
}

void GLWidget::paintGL() {
  glClearColor(back_color_.x(), back_color_.y(), back_color_.z(),
               back_color_.w());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shader_program_);
  std::vector<float> mvp_data = mvp_matrix_.GetData();

  glUniformMatrix4fv(mvp_location_, 1, GL_FALSE, mvp_data.data());

  if (line_color_location_ != -1) {
    GLfloat color_data[] = {line_color_.x(), line_color_.y(), line_color_.z(),
                            line_color_.w()};
    glUniform4fv(line_color_location_, 1, color_data);
  } else {
    qDebug() << "GLWidget::paintGL() ERROR: failed to find line_color uniform "
                "lication!";
  }

  if (point_vertex_color_location_ != -1) {
    GLfloat color_data[] = {point_vertex_color_.x(), point_vertex_color_.y(),
                            point_vertex_color_.z(), point_vertex_color_.w()};
    glUniform4fv(point_vertex_color_location_, 1, color_data);
  }

  glLineWidth(line_width_);
  if (show_point_vertex_location_ != -1) {
    glUniform1i(show_point_vertex_location_, 0);
  }

  if (is_line_solid_) {
    glDisable(GL_LINE_STIPPLE);
  } else {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(stipple_factor_, stipple_pattern_);
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glDrawElements(GL_LINES, edges_.size(), GL_UNSIGNED_INT, nullptr);

  if (show_point_vertex_) {
    glPointSize(point_vertex_size_);
    if (is_point_vertex_round_) {
      glEnable(GL_POINT_SMOOTH);  // типа круглые точки
    } else {
      glDisable(GL_POINT_SMOOTH);  // квадратные точки
    }
    if (show_point_vertex_location_ != -1) {
      glUniform1i(
          show_point_vertex_location_,
          1);  // это типа передается в шейдер, и даем знать, что рисуем вершину
    }
    glDrawArrays(GL_POINTS, 0, vertices_.size() / 3);
  }

  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glUseProgram(0);
}

void GLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void GLWidget::setup_shaders_() {
  const char *vertex_shader_src = R"(
    #version 130
    in vec3 position;
    uniform mat4 mvp;
    void main() {
      gl_Position = mvp * vec4(position, 1.0);
    }
  )";
  const char *fragment_shader_src = R"(
    #version 130
    uniform vec4 line_color; // переменная для цветов линий
    uniform vec4 point_vertex_color; // для цвета вершин
    uniform bool show_point_vertex; // отображена ли точка
    out vec4 color;
    void main() {
      if (show_point_vertex) {
        color = point_vertex_color;
      } else {
        color = line_color;
      }
    }
  )";

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_src, nullptr);
  glCompileShader(vertex_shader);
  GLint success;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar info_log[512];
    glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
    qDebug() << "Vertex Shader Error:" << info_log;
  }

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_src, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar info_log[512];
    glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
    qDebug() << "Fragment Shader Error:" << info_log;
  }

  shader_program_ = glCreateProgram();
  glAttachShader(shader_program_, vertex_shader);
  glAttachShader(shader_program_, fragment_shader);
  glLinkProgram(shader_program_);
  glGetProgramiv(shader_program_, GL_LINK_STATUS, &success);
  if (!success) {
    GLchar info_log[512];
    glGetProgramInfoLog(shader_program_, 512, nullptr, info_log);
    qDebug() << "Shader Program Link Error:" << info_log;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  mvp_location_ = glGetUniformLocation(shader_program_, "mvp");
  line_color_location_ = glGetUniformLocation(shader_program_, "line_color");
  point_vertex_color_location_ =
      glGetUniformLocation(shader_program_, "point_vertex_color");
  show_point_vertex_location_ =
      glGetUniformLocation(shader_program_, "show_point_vertex");

  if (mvp_location_ == -1) {
    qDebug() << "GLWidget::setup_shaders_() ERROR: failed to find mvp uniform "
                "location!";
  }
  if (line_color_location_ == -1) {
    qDebug() << "GLWidget::setup_shaders_() ERROR: failed to find line_color "
                "uniform location!";
  }
  if (point_vertex_color_location_ == -1) {
    qDebug() << "GLWidget::setup_shaders_() ERROR: failed to find "
                "point_vertex_color "
                "uniform location!";
  }
  if (show_point_vertex_location_ == -1) {
    qDebug() << "GLWidget::setup_shaders_() ERROR: failed to find "
                "show_point_vertex_ "
                "uniform location!";
  }
}

void GLWidget::resizeEvent(QResizeEvent *event) {
  QOpenGLWidget::resizeEvent(event);
  // соотношение сторон 16:10
  const double targetRatio = 16.0 / 12.0;
  int currentWidth = width();
  int currentHeight = height();
  double currentRatio = static_cast<double>(currentWidth) / currentHeight;

  // чтобы сохранить соотношение 16:12
  if (currentRatio > targetRatio) {
    // если чересчур широкий виджет, уменьшаем ширину
    currentWidth = static_cast<int>(currentHeight * targetRatio);
  } else if (currentRatio < targetRatio) {
    // если слишком высокий виджет, уменьшаем высоту
    currentHeight = static_cast<int>(currentWidth / targetRatio);
  }

  resize(currentWidth, currentHeight);
  update();
}

}  // namespace s21