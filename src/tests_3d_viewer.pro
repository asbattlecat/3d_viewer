TEMPLATE = app
CONFIG += c++20 console qt
QT += core gui widgets opengl openglwidgets

TARGET = test_3d_viewer

INCLUDEPATH += \
    . \
    include \
    tests

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
    tests/tests.cpp

HEADERS += \
    include/gui.h \
    include/3dmodel.h \
    include/controller.h \
    include/scene.h \
    include/gif_creator.h \
    tests/tests.h

# ImageMagick
IMAGEMAGICK_CFLAGS = $$system(pkg-config --cflags Magick++-6.Q16)
IMAGEMAGICK_LIBS   = $$system(pkg-config --libs Magick++-6.Q16 MagickWand-6.Q16 MagickCore-6.Q16)

QMAKE_CXXFLAGS += $$IMAGEMAGICK_CFLAGS
LIBS += $$IMAGEMAGICK_LIBS

# OpenMP
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

# GTest
LIBS += -lgtest -lgtest_main -pthread
