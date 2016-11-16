#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "cvutil.h"
#include "pixelsort.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage edges;
    QImage buffer;

    void loadFile(const QString &filename);
    void saveFile(const QString &filename);
    void drawBuffer();
    void drawEdges();

private slots:
    void find();
    void load();
    void save();
    void sort();
    void detectEdges();
    void detectAndDrawEdges();
};

#endif // MAINWINDOW_H
