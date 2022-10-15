#ifndef SCENE_H
#define SCENE_H

#define GL_SILENCE_DEPRECATION

#include <QtOpenGLWidgets/qopenglwidget.h>

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QSettings>
#include <cstdlib>

extern "C" {
#include "../CPart/parser.h"
}
class scene : public QOpenGLWidget {
  Q_OBJECT

 public:
  scene(QWidget *parent = nullptr);

  void keyPressEvent(QKeyEvent *) override;
  QSettings *settings;
  bool projection;

  float red_bg, green_bg, blue_bg, alpha_bg;
  float red_vertex, green_vertex, blue_vertex;
  float red_lines, green_lines, blue_lines;
  void save_settings();
  void load_settings();
  unsigned lineWidth, vertexSize;
  bool circleSquare, dashedSolid, is_none;

  int *indices;
  double *ver_cub;
  size_t size_vertex, size_indices;

  QString filename;

 private:
  float xRot, yRot, zRot, zoomScale, x_trans, y_trans, start_y, start_x;
  bool moving, dragging;

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void wheelEvent(QWheelEvent *) override;

  void cube();
};

#endif  // SCENE_H
