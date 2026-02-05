# -----------------------------
#  Основные настройки проекта
# -----------------------------
TEMPLATE = app
CONFIG += c++20 console
CONFIG += qt warn_on thread
QT += core gui widgets opengl openglwidgets

TARGET = 3d_viewer

# -----------------------------
#  Пути к исходникам
# -----------------------------
INCLUDEPATH += \
    . \
    include \
    tests

# -----------------------------
#  Источники и заголовки
# -----------------------------
SOURCES += \
    cpp_files/gui.cpp \
    cpp_files/affine.cpp \
    include/matrix/s21_matrix_oop.cpp \
    include/matrix/s21_matrix_oop_private.cpp \
    cpp_files/3dmodel.cpp \
    cpp_files/camera.cpp \
    cpp_files/controller.cpp \
    cpp_files/projection.cpp \
    cpp_files/scene.cpp \
    cpp_files/viewer_memento.cpp \
    cpp_files/gif_creator.cpp \
    main.cpp

HEADERS += \
    include/3dmodel.h \
    include/affine.h \
    include/camera.h \
    include/controller.h \
    include/gif_creator.h \
    include/gui.h \
    include/include_common.h \
    include/include_gui_common.h \
    include/projection.h \
    include/scene.h \
    include/viewer_memento.h

# -----------------------------
#  ImageMagick
# -----------------------------
IMAGEMAGICK_CFLAGS = $$system(pkg-config --cflags Magick++-6.Q16)
IMAGEMAGICK_LIBS   = $$system(pkg-config --libs Magick++-6.Q16 MagickWand-6.Q16 MagickCore-6.Q16)

QMAKE_CXXFLAGS += $$IMAGEMAGICK_CFLAGS
LIBS += $$IMAGEMAGICK_LIBS

# -----------------------------
#  OpenMP
# -----------------------------
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

# -----------------------------
#  доп
# -----------------------------
DEFINES += MAGICKCORE_HDRI_ENABLE=0
DEFINES += MAGICKCORE_QUANTUM_DEPTH=16
DEFINES += QT_NO_DEBUG

QMAKE_CXXFLAGS += -O2 -Wall -Wextra -Werror -fPIC

# -----------------------------
#  DVI (генерация документации)
# -----------------------------
dvi.target = dvi
dvi.commands = doxygen ./manual/Doxyfile && xdg-open ./doc/html/index.html
QMAKE_EXTRA_TARGETS += dvi

# -----------------------------
#  DIST (архивация проекта)
# -----------------------------
dist.depends = clean
dist.target = dist
dist.commands = tar -cvzf 3d_viewer.tar.gz ./../src
QMAKE_EXTRA_TARGETS += dist

QMAKE_CLEAN += 3d_viewer.tar.gz