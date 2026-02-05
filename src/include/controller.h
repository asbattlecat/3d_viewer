#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gui.h"
#include "include_common.h"
#include "include_gui_common.h"
#include "scene.h"

namespace s21 {

class Controller : public QObject {
  Q_OBJECT
 private:
  std::unique_ptr<Scene> model_;
  std::unique_ptr<MainWindow> view_;
  std::string json_dir_ = "logs";
  std::string json_path_ = "logs/state.json";

 public:
  Controller(std::unique_ptr<Scene> model, std::unique_ptr<MainWindow> view);
  ~Controller();
  void Start();

  void Show();

  void LoadState();

 private slots:
  void save_state_();
  void handle_model_loaded_(const Model3DDataGl &gl_data);
  void handle_model_loaded_memento_(const Model3DDataGl &gl_data);
  void handle_load_error_(const std::string &error_message);

  void handle_update_mvp_();
  void handle_update_temp_mvp_(S21MatrixWrapper &mvp);

 private:
  void connect_transl_sig_slots_();
  void connect_rotation_sig_slots_();
  void connect_scale_sig_slots_();

  void check_json_dir_();
};

}  // namespace s21

#endif