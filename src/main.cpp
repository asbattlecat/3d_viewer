#include "./include/gui.h"
#include "./include/include_common.h"
#include "./include/include_gui_common.h"
#include "include/affine.h"
#include "include/controller.h"
#include "include/scene.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  auto model = std::make_unique<s21::Scene>();
  auto view = std::make_unique<s21::MainWindow>();
  auto controller =
      std::make_unique<s21::Controller>(std::move(model), std::move(view));
  controller->Start();
  return app.exec();
}