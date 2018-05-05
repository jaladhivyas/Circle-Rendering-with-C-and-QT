#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:

    // Circle elements
    struct circle_elememnt{
        int curr_radius,outer_radius;
        int inner_radius;
        int curr_xpoint,curr_ypoint;
        int start_xpoint,start_ypoint;
        bool draw_circle,nearest_points;
        QImage img;
    }randering;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;



public slots:
  //slots
   void show_mousepressposition(QPoint& pos);
   void show_mousemoveposition(QPoint& pos);
   void show_mousereleaseposition(QPoint& pos);


};

#endif // MAINWINDOW_H
