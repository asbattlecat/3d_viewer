#include "../include/scene.h"

namespace s21 {

/**
 * @brief Базовый конструктор, заполнение данных начальными занчениями
 */
Scene::Scene() {
  model_ = std::make_unique<Model3D>();
  camera_ = std::make_unique<Camera>();
  projection_ = std::make_unique<Projection>();

  std::unique_ptr matrix = std::make_unique<S21MatrixWrapper>(4, 4);
  matrix->SetIdentity();
  transformation_ = std::make_unique<AffineTransDecorator>(std::move(matrix));
  InitialMVPMatrix();
}

//________________________________________________________________________
// чисто для проверки вывода содержимого распаршеного obj файла

void Scene::LoadModelNoThread(const std::string &obj_file_name) {
  ObjLoader loader(obj_file_name);
  loader.Load();
  is_model_displayed_ = true;
  ModelLoaded(loader.GetVertices(), loader.GetFaces(), loader.GetEdges());
}

void Scene::LoadModelMemento() {
  ObjLoader loader(current_file_path_);
  loader.Load();
  is_model_displayed_ = true;
  ModelLoadedMemento(loader.GetVertices(), loader.GetFaces(),
                     loader.GetEdges());
}

// ________________________________________________________________________

void Scene::LoadModelThread(const std::string &obj_file_name) {
  QThread *thread = new QThread();
  ObjLoader *loader = new ObjLoader(obj_file_name);

  loader->moveToThread(thread);
  // подключение парсинга при запуске потока
  connect(thread, &QThread::started, loader, &ObjLoader::Load);

  // обработка, если загрузка была успешной (без ошибок тд)
  connect(loader, &ObjLoader::Loaded, this, &Scene::ModelLoaded);

  // обработка, если произошла ошибка (см ObjLoader, там есть куча исключений,
  // которые обрабатываются через emit)
  connect(loader, &ObjLoader::LoadError, this, &Scene::LoadError);

  // Завершаем поток после успешной загрузки или ошибки
  connect(loader, &ObjLoader::Loaded, thread, &QThread::quit);
  connect(loader, &ObjLoader::LoadError, thread, &QThread::quit);
  // Удаляем loader и thread после завершения потока
  connect(thread, &QThread::finished, loader, &QObject::deleteLater);
  connect(thread, &QThread::finished, thread, &QObject::deleteLater);

  need_to_calculate_far_ = true;
  is_model_displayed_ = true;
  thread->start();
}

void Scene::UpdateCamera(const Vector3D &target, const Vector3D &eye,
                         const Vector3D &up) {
  need_to_calculate_far_ = true;
  camera_->UpdateCamera(target, eye, up);
}

void Scene::UpdateProjectionPersp(double fov, double aspect) {
  UpdateFar();
  projection_->UpdatePerspective(fov, aspect, projection_->GetNear(),
                                 projection_->GetFar());
}

void Scene::UpdateProjectionOrth(double left, double right, double bottom,
                                 double top) {
  UpdateFar();
  projection_->UpdateOrthogonal(left, right, bottom, top,
                                projection_->GetNear(), projection_->GetFar());
}

void Scene::UpdateProjection() {
  if (!IsPerspective()) {
    UpdateProjectionOrth(-16, 16, -12, 12);
  } else {
    UpdateProjectionPersp(90, 4.0 / 3.0);
  }
}

void Scene::UpdateFar() {
  if (need_to_calculate_far_) {
    S21Matrix model_matrix = transformation_->GetMatrix();  // model matrix
    S21Matrix view_matrix = camera_->GetViewMatrix();       // view matrix
    S21Matrix model_view_matrix = view_matrix * model_matrix;
    projection_->SetFar(calculate_far_(model_view_matrix));
  }
}

void Scene::ApplyTranslation(double tx, double ty, double tz) {
  need_to_calculate_far_ = true;
  transformation_->Translation(tx, ty, tz);
}

void Scene::ApplyRotation(double angle, Vector3D &axis) {
  need_to_calculate_far_ = true;
  transformation_->Rotation(angle, axis);
}

void Scene::ApplyScale(double scale_val) {
  need_to_calculate_far_ = true;
  transformation_->Scale(scale_val, scale_val, scale_val);
}

S21Matrix Scene::CreateMVPMatrix() {
  return transformation_->CreateMVPMatrix(get_proj_matrix_(),
                                          camera_->GetViewMatrix(),
                                          transformation_->GetMatrix());
}

Model3DDataGl Scene::GetModelAsGLData() {
  return Model3DDataGl(model_->GetVerticesToGlFormat(),
                       model_->GetEdgesToGlFormat());
}

bool Scene::IsPerspective() { return projection_->IsPerspectiveProjection(); }

void Scene::InitialMVPMatrix() {
  UpdateCamera(Vector3D(0, 0, 0), Vector3D(5, 5, -8), Vector3D(0, -1, 0));
  transformation_->SetIdentity();
  UpdateProjection();
}

std::string Scene::GetFileName() {
  QFileInfo file_info(QString::fromStdString(current_file_path_));
  return file_info.fileName().toStdString();
}

size_t Scene::GetEdgesCount() { return model_->GetEdgesCount(); }

size_t Scene::GetVerticesCount() { return model_->GetVerticesCount(); }

void Scene::SetMemento(SceneMemento &memento) {
  current_file_path_ = memento.GetFilePath();

  Projection proj = memento.GetProjection();
  projection_ = std::make_unique<Projection>(proj);

  std::unique_ptr<S21MatrixWrapper> transform =
      std::make_unique<S21MatrixWrapper>(memento.GetTransformationMatrix());
  transformation_ =
      std::make_unique<AffineTransDecorator>(std::move(transform));
}

SceneMemento Scene::CreateMemento() {
  S21Matrix transf = transformation_->GetMatrix();
  return SceneMemento(current_file_path_, transf, *projection_);
}

bool Scene::IsModelDisplayed() { return is_model_displayed_; }

////////////////////////////////////////////////////////////////////////////////
// слоты для загрузки модели

void Scene::ModelLoaded(
    const std::vector<Vector3D> &vertices,
    const std::vector<std::vector<unsigned int>> &faces,
    const std::unordered_set<std::pair<unsigned int, unsigned int>, PairHash>
        &edges) {
  model_->SetData(vertices, faces, edges);
  model_->CalculateBoundingBox();
  emit Loaded(Model3DDataGl(model_->GetVerticesToGlFormat(),
                            model_->GetEdgesToGlFormat()));
}

void Scene::ModelLoadedMemento(
    const std::vector<Vector3D> &vertices,
    const std::vector<std::vector<unsigned int>> &faces,
    const std::unordered_set<std::pair<unsigned int, unsigned int>, PairHash>
        &edges) {
  model_->SetData(vertices, faces, edges);
  model_->CalculateBoundingBox();
  emit LoadedMemento(Model3DDataGl(model_->GetVerticesToGlFormat(),
                                   model_->GetEdgesToGlFormat()));
}

void Scene::LoadError(const std::string &error_message) {
  emit ErrorLoad(error_message);
}

void Scene::ProcessObjLoad(std::string &file_path) {
  current_file_path_ = file_path;
  LoadModelThread(file_path);
}

////////////////////////////////////////////////////////////////////////////////
// слоты для обработки Translation сигналов от gui

void Scene::ProcessTranslation_nofps(const QString &tx_s, const QString &ty_s,
                                     const QString &tz_s) {
  if (is_model_displayed_) {
    double tx = tx_s.isEmpty() ? 0.0 : tx_s.toDouble();
    double ty = ty_s.isEmpty() ? 0.0 : ty_s.toDouble();
    double tz = tz_s.isEmpty() ? 0.0 : tz_s.toDouble();

    if (tx != 0.0 || ty != 0.0 || tz != 0.0) {
      ApplyTranslation(tx, ty, tz);
      emit UpdateMVP();
    }
  }
}

void Scene::ProcessTranslation_fps(double tx, double ty, double tz) {
  if (is_model_displayed_) {
    ApplyTranslation(tx, ty, tz);
    emit UpdateMVP();
  }
}

void Scene::CheckTranslationData_fps(const QString &tx_s, const QString &ty_s,
                                     const QString &tz_s) {
  if (is_model_displayed_) {
    double tx = tx_s.isEmpty() ? 0.0 : tx_s.toDouble();
    double ty = ty_s.isEmpty() ? 0.0 : ty_s.toDouble();
    double tz = tz_s.isEmpty() ? 0.0 : tz_s.toDouble();

    if (tx != 0.0 || ty != 0.0 || tz != 0.0) {
      emit OkTranslationData(tx, ty, tz);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// слоты для обработки Rotation сигналов от gui

void Scene::ProcessRotation_nofps(const QString &angle_s,
                                  const QString &axis_s) {
  if (is_model_displayed_) {
    double angle = angle_s.isEmpty() ? 0.0 : angle_s.toDouble();
    double ax = 0.0, ay = 0.0, az = 0.0;
    double is_empty = axis_s.isEmpty() ? 0.0 : 0.0;
    ax = axis_s.toLower() == 'x' ? 1.0 : is_empty;
    ay = axis_s.toLower() == 'y' ? 1.0 : is_empty;
    az = axis_s.toLower() == 'z' ? 1.0 : is_empty;
    if (angle != 0.0 && (ax != 0.0 || ay != 0.0 || az != 0.0)) {
      Vector3D axis(ax, ay, az);
      ApplyRotation(angle, axis);
      emit UpdateMVP();
    }
  }
}

void Scene::ProcessRotation_fps(double angle, Vector3D &axis) {
  if (is_model_displayed_) {
    ApplyRotation(angle, axis);
    emit UpdateMVP();
  }
}

void Scene::CheckRotationData_fps(const QString &angle_s,
                                  const QString &_axis_s) {
  if (is_model_displayed_) {
    double angle = angle_s.isEmpty() ? 0.0 : angle_s.toDouble();
    double ax = 0.0, ay = 0.0, az = 0.0;
    double is_empty = _axis_s.isEmpty() ? 0.0 : 0.0;
    ax = _axis_s.toLower() == 'x' ? 1.0 : is_empty;
    ay = _axis_s.toLower() == 'y' ? 1.0 : is_empty;
    az = _axis_s.toLower() == 'z' ? 1.0 : is_empty;
    if (angle != 0.0 && (ax != 0.0 || ay != 0.0 || az != 0.0)) {
      Vector3D axis(ax, ay, az);
      emit OkRotationData(angle, axis);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// слоты для обработки Scale сигналов от gui

void Scene::ProcessScaling_nofps(const QString &scale_value_s) {
  if (is_model_displayed_) {
    double scale = scale_value_s.isEmpty() ? 0.0 : scale_value_s.toDouble();
    if (scale != 0.0 && scale != 1.0) {
      ApplyScale(scale);
      emit UpdateMVP();
    }
  }
}

void Scene::ProcessScaling_fps(double scale_value) {
  if (is_model_displayed_) {
    S21MatrixWrapper result = apply_scale_fps_(scale_value);
    emit UpdateTempMVP(result);
  }
}

void Scene::CheckScalingData_fps(const QString &scale_value_s) {
  if (is_model_displayed_) {
    double scale = scale_value_s.isEmpty() ? 0.0 : scale_value_s.toDouble();
    if (scale != 0.0 && scale != 1.0) emit OkScalingData(scale);
  }
}

S21MatrixWrapper Scene::apply_scale_fps_(double scale_val) {
  std::unique_ptr matrix = std::make_unique<S21MatrixWrapper>(4, 4);
  matrix->SetIdentity();
  auto transf = std::make_unique<AffineTransDecorator>(std::move(matrix));

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      (*transf)(i, j) = (*transformation_)(i, j);
    }
  }

  transf->Scale(scale_val, scale_val, scale_val);

  S21Matrix view = camera_->GetViewMatrix();
  S21Matrix model = transf->GetMatrix();
  S21Matrix model_view = view * model;

  double far = calculate_far_(model_view);

  S21Matrix result =
      transf->CreateMVPMatrix(get_proj_matrix_temp_(far),
                              camera_->GetViewMatrix(), transf->GetMatrix());
  return S21MatrixWrapper(result);
}

S21Matrix Scene::update_proj_orth_temp_(double far) {
  AffineTransDecorator projection_matrix(
      std::make_unique<S21MatrixWrapper>(4, 4));
  return projection_matrix.CreateProjMatrixOrth(-16, 16, -12, 12,
                                                projection_->GetNear(), far);
}

S21Matrix Scene::update_proj_persp_temp_(double far) {
  AffineTransDecorator projection_matrix(
      std::make_unique<S21MatrixWrapper>(4, 4));
  return projection_matrix.CreateProjMatrixPersp(90, 4.0 / 3.0,
                                                 projection_->GetNear(), far);
}

S21Matrix Scene::get_proj_matrix_temp_(double far) {
  S21Matrix proj_matr(4, 4);

  if (projection_->IsPerspectiveProjection()) {
    proj_matr = update_proj_persp_temp_(far);
  } else if (!projection_->IsPerspectiveProjection()) {
    proj_matr = update_proj_orth_temp_(far);
  }
  return proj_matr;
}

////////////////////////////////////////////////////////////////////////////////
// слоты для обработки смены типа проекции от gui
void Scene::ProcessChangeProjection() {
  if (is_model_displayed_) {
    switch_projection_();
    emit UpdateMVP();
  }
}

void Scene::CreatingGifFromJpegs(int target_frames, QString &gif_fr_path,
                                 QString gif_target_path, double anim_delay) {
  QThread *thread = new QThread();
  GifCreator *creator =
      new GifCreator(target_frames, gif_fr_path, gif_target_path, anim_delay);

  creator->moveToThread(thread);

  connect(thread, &QThread::started, creator, &GifCreator::CreateGif);

  connect(creator, &GifCreator::GifError, this, &Scene::LoadError);

  connect(creator, &GifCreator::Ok, thread, &QThread::quit);
  connect(creator, &GifCreator::GifError, thread, &QThread::quit);

  connect(thread, &QThread::finished, creator, &QObject::deleteLater);
  connect(thread, &QThread::finished, thread, &QObject::deleteLater);

  thread->start();
}

/**
 * @brief Метод для расчета far для ProjectMatrix. Нужно для того, чтобы фигура
 * полностью помещалась по глубине (не было обрезания слишком дальних точек)
 * @return Возвращает расстояние от камеры до самой дальней вершины фигуры (уже
 * после умножения ModelMatrix * ViewMatrix)
 */
double Scene::calculate_far_(S21Matrix &model_view) {
  BoundingBox box = model_->GetBoundingBox();

  std::vector<Vector3D> corners = {
      {box.min.x, box.min.y, box.min.z},   // угол ниж. грани 1
      {box.min.x, box.min.y, box.max.z},   // угол ниж. грани 2
      {box.max.x, box.min.y, box.max.z},   // угол ниж. грани 3
      {box.max.x, box.min.y, box.min.z},   // угол ниж. грани 4
      {box.min.x, box.max.y, box.min.z},   // угол верх. грани 1
      {box.min.x, box.max.y, box.max.z},   // угол верх. грани 2
      {box.max.x, box.max.y, box.max.z},   // угол верх. грани 3
      {box.max.x, box.max.y, box.min.z}};  // угол верх. грани 4

  double max_z = 0.0;

  for (const auto &corner : corners) {
    S21Matrix corner_vector(4, 1);
    corner_vector(0, 0) = corner.x;
    corner_vector(1, 0) = corner.y;
    corner_vector(2, 0) = corner.z;
    corner_vector(3, 0) = 1.0;
    S21Matrix transformed_ver_vec = model_view * corner_vector;

    if (std::isnan(transformed_ver_vec(2, 0)) ||
        std::isinf(transformed_ver_vec(2, 0))) {
      continue;
    }

    max_z = std::max(max_z, std::abs(transformed_ver_vec(2, 0)));
  }
  need_to_calculate_far_ = false;

  double near = projection_->GetNear();
  double far = std::max(max_z * 1.1, near + 1.0);  // немного увеличенный запас

  return far;
}

void Scene::print_figues_data() {
  std::vector<Vector3D> vertices = model_->GetVertices();

  std::cout << "\nVetrices count: " << vertices.size() << "\n\n";

  std::vector<std::vector<unsigned int>> faces = model_->GetFaces();

  std::cout << "\n\n faces count: " << faces.size() << "\n\n";

  auto edges = model_->GetEdges();
  std::cout << "\n\nEdges count: " << edges.size() << "\n\n";
}

void Scene::switch_projection_() {
  if (IsPerspective()) {
    projection_->SetOrthogonal();
  } else {
    projection_->SetPerspective();
  }
}

Vector3D Scene::get_current_translation_() {
  return {(*transformation_)(0, 3), (*transformation_)(1, 3),
          (*transformation_)(2, 3)};
}

S21Matrix Scene::get_proj_matrix_() {
  S21Matrix proj_matr(4, 4);

  UpdateProjection();
  if (projection_->IsPerspectiveProjection()) {
    proj_matr = projection_->GetPerspProjMatrix();
  } else if (!projection_->IsPerspectiveProjection()) {
    proj_matr = projection_->GetOrthProjMatrix();
  }
  return proj_matr;
}

}  // namespace s21