#ifndef GIF_CREATOR_H
#define GIF_CREATOR_H

#include "include_common.h"

namespace s21 {

class GifCreator : public QObject {
  Q_OBJECT
 private:
  int target_frames_;
  QString gif_frames_path_;
  QString gif_target_path_;
  double animation_delay_;

 public:
  GifCreator(int target_frames, QString &gif_fr_path, QString gif_target_path,
             double anim_delay);
  void CreateGif();

 signals:
  void GifError(const std::string &error_message);
  void Ok();

 private:
  void clear_jpegs_for_gif_();
};

}  // namespace s21

#endif