#include "scene.h"

#include "../Viewer/viewer.h"

scene::scene(QWidget* parent) : QOpenGLWidget(parent) {
  settings = new QSettings(QDir::homePath() + "/3DViewerConfig/settings.conf",
                           QSettings::IniFormat);
  indices = NULL, ver_cub = NULL;
  size_vertex = 3, size_indices = 0;
  zoomScale = -10;
  moving = false, dragging = false;
  start_x = 0, start_y = 0;
  xRot = 1, yRot = 1;
  x_trans = 0, y_trans = 0;

  load_settings();
}

void scene::save_settings() {
  settings->beginGroup("coordinate");

  settings->setValue("dashedSolid", dashedSolid);
  settings->setValue("projection", projection);
  settings->setValue("circleSquare", circleSquare);
  settings->setValue("is_none", is_none);
  settings->endGroup();

  settings->beginGroup("rgb");
  settings->setValue("red_bg", red_bg);
  settings->setValue("green_bg", green_bg);
  settings->setValue("blue_bg", blue_bg);
  settings->setValue("alpha_bg", alpha_bg);
  settings->setValue("red_vertex", red_vertex);
  settings->setValue("green_vertex", green_vertex);
  settings->setValue("blue_vertex", blue_vertex);
  settings->setValue("red_lines", red_lines);
  settings->setValue("green_lines", green_lines);
  settings->setValue("blue_lines", blue_lines);
  settings->endGroup();

  settings->beginGroup("size");
  settings->setValue("line_width", lineWidth);
  settings->setValue("vertexSize", vertexSize);
  settings->setValue("zoomScale", zoomScale);
  settings->endGroup();
}

void scene::load_settings() {
  settings->beginGroup("coordinate");

  dashedSolid = settings->value("dashedSolid", false).toBool();
  projection = settings->value("projection", true).toBool();
  circleSquare = settings->value("circleSquare", false).toBool();
  is_none = settings->value("is_none", false).toBool();

  settings->endGroup();

  settings->beginGroup("rgb");
  red_bg = settings->value("red_bg", 0.0).toFloat();
  green_bg = settings->value("green_bg", 0.0).toFloat();
  blue_bg = settings->value("blue_bg", 0.0).toFloat();
  alpha_bg = settings->value("alpha_bg", 0.0).toFloat();
  red_vertex = settings->value("red_vertex", 0.0).toFloat();
  green_vertex = settings->value("green_vertex", 0.0).toFloat();
  blue_vertex = settings->value("blue_vertex", 0.0).toFloat();
  red_lines = settings->value("red_lines", 255.).toFloat();
  green_lines = settings->value("green_lines", 0.).toFloat();
  blue_lines = settings->value("blue_lines", 45.).toFloat();
  settings->endGroup();

  settings->beginGroup("size");
  lineWidth = settings->value("line_width", 5).toUInt();
  zoomScale = settings->value("zoomScale", -10).toFloat();
  vertexSize = settings->value("vertexSize", 1).toUInt();
  settings->endGroup();
}

void scene::initializeGL() { glEnable(GL_DEPTH_TEST); }

void scene::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void scene::paintGL() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (projection) {
    glFrustum(-1, 1, -1, 1, 1, 1000000);
  } else {
    glOrtho(-1, 1, -1, 1, -1, 1000000);
  }

  if (projection)
    glTranslatef(x_trans, y_trans, zoomScale);
  else
    glScaled(zoomScale, zoomScale, zoomScale);

  glClearColor(red_bg / 255.0f, green_bg / 255.0f, blue_bg / 255.0f, alpha_bg);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glRotatef(xRot, 1, 0, 0);
  glRotatef(yRot, 0, 1, 0);

  cube();
}

void scene::mousePressEvent(QMouseEvent* mouse) {
  switch (mouse->button()) {
    case Qt::LeftButton:
      moving = true;
      dragging = false;
      break;
    case Qt::MiddleButton:
      dragging = true;
      moving = false;
      break;
    default:
      moving = false;
      dragging = false;
      break;
  }
  start_x = mouse->pos().x();
  start_y = mouse->pos().y();
}

void scene::mouseMoveEvent(QMouseEvent* mouse) {
  if (moving) {
    yRot = yRot + (mouse->pos().x() - start_x);
    xRot = xRot + (mouse->pos().y() - start_y);
  }
  if (dragging) {
    x_trans = x_trans + (mouse->pos().x() - start_x) / 4.f;
    y_trans = y_trans - (mouse->pos().y() - start_y) / 4.f;
  }
  start_x = mouse->pos().x();
  start_y = mouse->pos().y();
  update();
}

void scene::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_R:
      x_trans = 0, y_trans = 0;
      zoomScale = -10;
      break;
    case Qt::Key_O:
      x_trans = 0, y_trans = 0, zoomScale = 0.1;
      projection = false;
      break;
    case Qt::Key_P:
      x_trans = 0, y_trans = 0, zoomScale = -10;
      projection = true;
      break;
  }
  update();
}

void scene::wheelEvent(QWheelEvent* event) {
  QPoint numDegrees = event->angleDelta();
  if (numDegrees.y() < 0)
    zoomScale = projection ? zoomScale * 1.1 : zoomScale / 1.1;
  if (numDegrees.y() > 0)
    zoomScale = projection ? zoomScale / 1.1 : zoomScale * 1.1;
  update();
}

void scene::cube() {
  glVertexPointer(3, GL_DOUBLE, 0, ver_cub);
  glEnableClientState(GL_VERTEX_ARRAY);

  glColor3d(red_vertex / 255.0f, green_vertex / 255.0f, blue_vertex / 255.0f);
  glPointSize(vertexSize);
  if (!circleSquare) glEnable(GL_POINT_SMOOTH);
  if (!is_none) glDrawArrays(GL_POINTS, 1, ((size_vertex)-3) / 3);

  glColor3d(red_lines / 255.0f, green_lines / 255.0f, blue_lines / 255.0f);
  glLineWidth(lineWidth);
  if (dashedSolid) {
    glLineStipple(1, 0x00FF);
    glEnable(GL_LINE_STIPPLE);
  }
  glDrawElements(GL_LINES, size_indices, GL_UNSIGNED_INT, indices);
  save_settings();
  glDisable(GL_LINE_STIPPLE);
  glDisable(GL_POINT_SMOOTH);
  glDisableClientState(GL_VERTEX_ARRAY);
}
