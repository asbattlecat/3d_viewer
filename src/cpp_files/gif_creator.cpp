#include "../include/gif_creator.h"

namespace s21 {

GifCreator::GifCreator(int target_frames, QString &gif_fr_path,
                       QString gif_target_path, double anim_delay)
    : target_frames_(target_frames),
      gif_frames_path_(gif_fr_path),
      gif_target_path_(gif_target_path),
      animation_delay_(anim_delay) {}

void GifCreator::CreateGif() {
  std::vector<Magick::Image> images;
  try {
    for (int i = 1; i <= target_frames_; i++) {
      QString frames_count = QString::number(i);
      QString q_file_name = QString("%1/Screenshot_for_gif_%2%3")
                                .arg(gif_frames_path_, frames_count, ".jpeg");
      QFileInfo file_info(q_file_name);
      if (file_info.exists()) {
        Magick::Image image;
        image.read(q_file_name.toStdString());
        image.resize(Magick::Geometry(640, 480));
        image.quantizeColors(256);
        image.animationDelay(animation_delay_);
        images.push_back(image);
      } else {
        qDebug() << "File does not exists: " << q_file_name << ", skipped";
      }
    }
    Magick::writeImages(images.begin(), images.end(),
                        gif_target_path_.toStdString());
    qDebug() << "GIF successfully created, path:" << gif_target_path_;
    clear_jpegs_for_gif_();
  } catch (Magick::Exception &error) {
    emit GifError(error.what());
  }
  emit Ok();
}

void GifCreator::clear_jpegs_for_gif_() {
  QDir dir(gif_frames_path_);
  QStringList filters;
  filters << "Screenshot_for_gif_*.jpeg";
  dir.setNameFilters(filters);
  for (const auto &file : dir.entryList()) {
    dir.remove(file);
  }
}

}  // namespace s21