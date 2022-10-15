#include "../Viewer/viewer.h"

#include "../GLWidget/scene.h"
#include "ui_viewer.h"

viewer::viewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::viewer) {
  ui->setupUi(this);
  this->setWindowTitle("3D Viewer");
  ui->horizontalSlider_lineWidth->setValue(ui->widget->lineWidth);
  ui->lcdNumber_lineWidth->display((int)ui->widget->lineWidth);
  ui->horizontalSlider_versize->setValue(ui->widget->vertexSize);
  ui->lcdNumber_versize->display((int)ui->widget->vertexSize);
  hiden = false, is_recording = false;
  time = 0.0;
  record_time = new QTimer(this);
  connect(record_time, &QTimer::timeout, this, &viewer::recording);
}

viewer::~viewer() { delete ui; }

void viewer::on_actionOpen_triggered() {
  QString fname = QFileDialog::getOpenFileName(
      this, "Choose File", QDir::homePath(), tr("OBJ (*.obj)"));
  if (fname != "") {
    ui->widget->filename = fname;
    ui->widget->update();
    if (ui->widget->ver_cub != NULL) {
      free(ui->widget->ver_cub);
      free(ui->widget->indices);
    }
    ui->widget->ver_cub = (double *)calloc(3, sizeof(double));
    ui->widget->indices = (int *)calloc(1, sizeof(int));

    ui->widget->size_indices = 0;
    ui->widget->size_vertex = 3;

    QByteArray t = fname.toLocal8Bit();
    char const *pa = t.data();

    parseVertex(pa, &(ui->widget->ver_cub), &(ui->widget->size_vertex));
    parseIndices(pa, &(ui->widget->indices), &(ui->widget->size_indices));
  }
}

void viewer::on_actionClose_triggered() {
  if (ui->widget->ver_cub != NULL) {
    free(ui->widget->ver_cub);
    free(ui->widget->indices);
    ui->widget->indices = NULL, ui->widget->ver_cub = NULL;
    ui->widget->size_indices = 0, ui->widget->size_vertex = 3;
    ui->widget->filename = "";
    ui->widget->update();
  }
}

void viewer::on_actionInfo_triggered() {
  QMessageBox::information(
      this, "Information",
      "Filename: " + ui->widget->filename +
          "\nVertices: " + QString::number((ui->widget->size_vertex - 3) / 3) +
          "\nLines: " + QString::number(ui->widget->size_indices / 3 - 4));
}

void viewer::keyPressEvent(QKeyEvent *event) {
  ui->widget->keyPressEvent(event);
}

void viewer::on_pushButton_bg_clicked() {
  QColor BG_color = QColorDialog::getColor();
  ui->widget->red_bg = BG_color.red();
  ui->widget->green_bg = BG_color.green();
  ui->widget->blue_bg = BG_color.blue();
  ui->widget->alpha_bg = BG_color.alpha();
  this->setStyleSheet("QMainWindow{ background-color: rgb(" +
                      QString::number(BG_color.red()) + ", " +
                      QString::number(BG_color.green()) + ", " +
                      QString::number(BG_color.blue()) + ");}");
}

void viewer::on_pushButton_vertex_clicked() {
  QColor ver_color = QColorDialog::getColor();
  ui->widget->red_vertex = ver_color.red();
  ui->widget->green_vertex = ver_color.green();
  ui->widget->blue_vertex = ver_color.blue();
}

void viewer::on_pushButton_lines_clicked() {
  QColor line_color = QColorDialog::getColor();
  ui->widget->red_lines = line_color.red();
  ui->widget->green_lines = line_color.green();
  ui->widget->blue_lines = line_color.blue();
}

void viewer::on_horizontalSlider_lineWidth_sliderMoved(int position) {
  ui->widget->lineWidth = position;
  ui->lcdNumber_lineWidth->display(position);
  ui->widget->update();
}

void viewer::on_horizontalSlider_lineWidth_sliderPressed() {
  on_horizontalSlider_lineWidth_sliderMoved(
      ui->horizontalSlider_lineWidth->value());
}

void viewer::on_horizontalSlider_versize_sliderMoved(int position) {
  ui->widget->is_none = false;
  ui->widget->vertexSize = position;
  ui->lcdNumber_versize->display(position);
  ui->widget->update();
}

void viewer::on_horizontalSlider_versize_sliderPressed() {
  on_horizontalSlider_versize_sliderMoved(
      ui->horizontalSlider_versize->value());
}

void viewer::on_pushButton_line_solid_clicked() {
  ui->widget->dashedSolid = false;
  ui->widget->update();
}

void viewer::on_pushButton_line_dashed_clicked() {
  ui->widget->dashedSolid = true;
  ui->widget->update();
}

void viewer::on_pushButton_ver_circle_clicked() {
  ui->widget->is_none = false;
  ui->widget->circleSquare = false;
  ui->widget->update();
}

void viewer::on_pushButton_ver_square_clicked() {
  ui->widget->is_none = false;
  ui->widget->circleSquare = true;
  ui->widget->update();
}

void viewer::on_pushButton_ver_none_clicked() {
  ui->widget->is_none = true;
  ui->widget->update();
}

void viewer::on_horizontalSlider_scale_sliderMoved(int position) {
  if (ui->widget->filename != "") {
    scale(&(ui->widget->ver_cub), ui->widget->size_vertex, (double)position);
  }
  ui->lcdNumber_scale->display(position);
  ui->widget->update();
}

void viewer::on_doubleSpinBox_valueChanged(double arg1) {
  if (ui->widget->filename != "") {
    scale(&(ui->widget->ver_cub), ui->widget->size_vertex, (double)arg1);
  }
  ui->widget->update();
}

void viewer::on_horizontalSlider_scale_sliderPressed() {
  on_doubleSpinBox_valueChanged(ui->horizontalSlider_scale->value());
}

void viewer::on_doubleSpinBox_x_move_valueChanged() {
  if (ui->widget->filename != "") {
    double x = ui->doubleSpinBox_x_move->value();
    double y = ui->doubleSpinBox_y_move->value();
    double z = ui->doubleSpinBox_z_move->value();

    move_x_y_z(&(ui->widget->ver_cub), ui->widget->size_vertex, x, y, z);
    ui->widget->update();
  }
}

void viewer::on_doubleSpinBox_y_move_valueChanged() {
  on_doubleSpinBox_x_move_valueChanged();
}

void viewer::on_doubleSpinBox_z_move_valueChanged() {
  on_doubleSpinBox_x_move_valueChanged();
}

void viewer::on_doubleSpinBox_x_rot_valueChanged(double arg1) {
  if (ui->widget->filename != "") {
    rotate_x_y_z(&(ui->widget->ver_cub), 'x', ui->doubleSpinBox_x_rot->value(),
                 ui->widget->size_vertex);
    ui->widget->update();
  }
}

void viewer::on_doubleSpinBox_y_rot_valueChanged(double arg1) {
  if (ui->widget->filename != "") {
    rotate_x_y_z(&(ui->widget->ver_cub), 'y', ui->doubleSpinBox_y_rot->value(),
                 ui->widget->size_vertex);
    ui->widget->update();
  }
}

void viewer::on_doubleSpinBox_z_rot_valueChanged(double arg1) {
  if (ui->widget->filename != "") {
    rotate_x_y_z(&(ui->widget->ver_cub), 'z', ui->doubleSpinBox_z_rot->value(),
                 ui->widget->size_vertex);
    ui->widget->update();
  }
}

void viewer::on_actionOrthographic_Perspective_triggered() {
  if (ui->widget->projection) {
    QKeyEvent *key = new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::NoModifier);
    ui->widget->keyPressEvent(key);
    delete key;
  } else {
    QKeyEvent *key = new QKeyEvent(QEvent::KeyPress, Qt::Key_P, Qt::NoModifier);
    ui->widget->keyPressEvent(key);
    delete key;
  }
}

void viewer::on_actionHide_triggered() {
  if (hiden) {
    ui->dockWidget_main->show();
    hiden = false;
  } else {
    ui->dockWidget_main->hide();
    hiden = true;
  }
}

void viewer::saveImage(QString format) {
  QString str = QFileDialog::getSaveFileName(this, "Save file as",
                                             QDir::homePath(), format);
  if (str != "") ui->widget->grabFramebuffer().save(str);
}

void viewer::on_actionJPEG_triggered() { saveImage("*.jpeg"); }

void viewer::on_actionBMP_triggered() { saveImage("*.bmp"); }

void viewer::on_actionGIF_triggered() {
  if (!is_recording) {
    is_recording = true;
    this->setStyleSheet("QMainWindow{ background-color: red;}");
    record_time->start(100);
  }
}

void viewer::recording() {
  if (is_recording && time <= 5.0) {
    GIF.push_back(ui->widget->grab().toImage());
    time += 0.1;
  } else {
    saveGIF();
    record_time->stop();
  }
}

void viewer::saveGIF() {
  QString str = QFileDialog::getSaveFileName(
      this, tr("Save GIF"), QDir::homePath(), tr("GIF (*.gif)"));
  if (str != "") {
    QGifImage gif(QSize(640, 480));

    gif.setDefaultTransparentColor(Qt::black);
    gif.setDefaultDelay(100);

    for (QVector<QImage>::Iterator frame = GIF.begin(); frame != GIF.end();
         frame++) {
      gif.addFrame(*frame);
    }

    gif.save(str);
    GIF.clear();
  }
  time = 0.0;
  is_recording = false;

  this->setStyleSheet("QMainWindow{ background-color: rgb(" +
                      QString::number(ui->widget->red_bg) + ", " +
                      QString::number(ui->widget->green_bg) + ", " +
                      QString::number(ui->widget->blue_bg) + ");}");
}
