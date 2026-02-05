#include "include/controller.h"

namespace s21 {

Controller::Controller(std::unique_ptr<Scene> model,
                       std::unique_ptr<MainWindow> view)
    : model_(std::move(model)), view_(std::move(view)) {
  // подключение сигнала-слота для загрузки модели при нажатии кнопки
  connect(view_.get(), &MainWindow::ProcessObjLoad, model_.get(),
          &Scene::ProcessObjLoad);

  // подключение обработки для успешной загрузки модели в фоновом потоке
  connect(model_.get(), &Scene::Loaded, this,
          &Controller::handle_model_loaded_);
  connect(model_.get(), &Scene::LoadedMemento, this,
          &Controller::handle_model_loaded_memento_);

  // подключение обработки загрузки модели с ошибками
  connect(model_.get(), &Scene::ErrorLoad, this,
          &Controller::handle_load_error_);

  // обработка сигналов от нажатия кнопок Apply, fps
  connect_transl_sig_slots_();
  connect_rotation_sig_slots_();
  connect_scale_sig_slots_();

  connect(model_.get(), &Scene::UpdateMVP, this,
          &Controller::handle_update_mvp_);
  connect(model_.get(), &Scene::UpdateTempMVP, this,
          &Controller::handle_update_temp_mvp_);

  connect(view_.get(), &MainWindow::ProcessChangeProjection, model_.get(),
          &Scene::ProcessChangeProjection);
  connect(view_.get(), &MainWindow::AppAboutToQuit, this,
          &Controller::save_state_);

  connect(view_.get(), &MainWindow::CreateGifFromJpegs, model_.get(),
          &Scene::CreatingGifFromJpegs);
}

void Controller::Show() { view_->show(); }

void Controller::Start() {
  Show();
  LoadState();
}

Controller::~Controller() = default;

void Controller::LoadState() {
  QString q_dir = QString::fromStdString(json_dir_);
  QDir dir(q_dir);
  if (dir.exists()) {
    QString json_path = QString::fromStdString(json_path_);
    QFile file(json_path);
    if (file.open(QIODevice::ReadOnly)) {
      QByteArray data = file.readAll();
      file.close();
      QJsonDocument json_doc = QJsonDocument::fromJson(data);
      QJsonObject json_object = json_doc.object();

      QJsonObject gl_json = json_object["gl_widget"].toObject();
      QJsonObject scene_json = json_object["scene"].toObject();
      SceneMemento sc_memento = SceneMemento::ReadFromJson(scene_json);
      GLWidgetMemento gl_memento = GLWidgetMemento::ReadFromJson(gl_json);

      model_->SetMemento(sc_memento);
      view_->SetMemento(gl_memento);
      model_->LoadModelMemento();
    }
  }
}

void Controller::handle_model_loaded_(const Model3DDataGl &gl_data) {
  view_->SetModelData(gl_data);
  model_->InitialMVPMatrix();
  S21MatrixWrapper mvp = S21MatrixWrapper(model_->CreateMVPMatrix());
  view_->SetMVPMatrix(mvp);
  view_->SetModelInfo(model_->GetFileName(), model_->GetEdgesCount(),
                      model_->GetVerticesCount());
}

void Controller::handle_model_loaded_memento_(const Model3DDataGl &gl_data) {
  view_->SetModelData(gl_data);
  S21MatrixWrapper mvp = S21MatrixWrapper(model_->CreateMVPMatrix());
  view_->SetMVPMatrix(mvp);
  view_->SetModelInfo(model_->GetFileName(), model_->GetEdgesCount(),
                      model_->GetVerticesCount());
  view_->update();
}

void Controller::save_state_() {
  if (model_->IsModelDisplayed()) {
    SceneMemento sc_memento = model_->CreateMemento();
    GLWidgetMemento gl_memento = view_->CreateMemento();

    QJsonObject json;
    json["scene"] = sc_memento.WriteToJson();
    json["gl_widget"] = gl_memento.WriteToJson();
    check_json_dir_();
    QJsonDocument json_doc(json);

    QString json_path = QString::fromStdString(json_path_);
    QFile file(json_path);
    if (file.open(QIODevice::WriteOnly)) {
      file.write(json_doc.toJson(QJsonDocument::Indented));
      file.close();
    }
  }
}

void Controller::handle_load_error_(const std::string &error_message) {
  QMessageBox::critical(nullptr, "Error",
                        QString::fromStdString(error_message));
}

void Controller::handle_update_mvp_() {
  S21MatrixWrapper mvp = S21MatrixWrapper(model_->CreateMVPMatrix());
  view_->SetMVPMatrix(mvp);
}

void Controller::handle_update_temp_mvp_(S21MatrixWrapper &mvp) {
  view_->SetMVPMatrix(mvp);
}

void Controller::connect_transl_sig_slots_() {
  connect(view_.get(), &MainWindow::ApplyTranslationClicked, model_.get(),
          &Scene::CheckTranslationData_fps);
  connect(model_.get(), &Scene::OkTranslationData, view_.get(),
          &MainWindow::StartTranslModel_fps);
  connect(view_.get(), &MainWindow::ProcessTranslation_fps, model_.get(),
          &Scene::ProcessTranslation_fps);
}

void Controller::connect_rotation_sig_slots_() {
  connect(view_.get(), &MainWindow::ApplyRotationClicked, model_.get(),
          &Scene::CheckRotationData_fps);
  connect(model_.get(), &Scene::OkRotationData, view_.get(),
          &MainWindow::StartRotationModel_fps);
  connect(view_.get(), &MainWindow::ProcessRotation_fps, model_.get(),
          &Scene::ProcessRotation_fps);
}

void Controller::connect_scale_sig_slots_() {
  connect(view_.get(), &MainWindow::ApplyScaleClicked, model_.get(),
          &Scene::CheckScalingData_fps);
  connect(model_.get(), &Scene::OkScalingData, view_.get(),
          &MainWindow::StartScaleModel_fps);
  connect(view_.get(), &MainWindow::ProcessScaling_fps, model_.get(),
          &Scene::ProcessScaling_fps);
  connect(view_.get(), &MainWindow::ProcessScale_nofps, model_.get(),
          &Scene::ProcessScaling_nofps);
}

void Controller::check_json_dir_() {
  QString q_dir = QString::fromStdString(json_dir_);
  QDir dir(q_dir);

  if (!dir.exists()) {
    dir.mkpath(q_dir);
    if (!dir.exists()) {
      qDebug()
          << "ERROR: Controller::check_json_dir_(): json_dir was not created!";
    }
  }
}

}  // namespace s21