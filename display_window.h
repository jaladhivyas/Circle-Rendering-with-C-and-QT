#ifndef DISPLAY_WINDOW_H
#define DISPLAY_WINDOW_H
#include <QObject>
#include <QWidget>
#include <QLabel>
#include<QPixmap>
#include<QImage>
#include<QPainter>
#include<iostream>
#include<stdio.h>
#include<math.h>
#include<vector>
#include <QMouseEvent>
#include<mainwindow.h>
#define grid_size 20
using namespace std;

class display_window : public QLabel
{

   Q_OBJECT


public:
    display_window(QWidget *parent=0);

    void init_grid();//Initialize display grid parameters

    void init_window();//  display window with grid points only

    void draw_nearespoints(int search_x,int search_y, int theta,MainWindow::circle_elememnt &c_elements,QPainter &painter);  // draw the nearest points for a given point on the circle

    bool check_pointinRect(int r_left, int r_top, int r_width, int r_height);   //  check the boundary points of circle are inside the grid or not

    void display_randering(MainWindow::circle_elememnt &c_elements);     // show the circles and nearest points
    QImage img;
private:

    // Grid parameters
    struct grid{
        int start_gridx,start_gridy;
        int end_gridx,end_gridy;
        int x_inc,y_inc;
        int x_grid[grid_size],y_grid[grid_size];

    }grid_elements;

protected:


    void mouseMoveEvent(QMouseEvent *mouse_event);
    void mousePressEvent(QMouseEvent *mouse_event);
    void mouseReleaseEvent(QMouseEvent *mouse_event);

signals:
    // Mouse event signals
    void send_mousepressPosition(QPoint&);
    void send_mousemovePosition(QPoint&);
    void send_mousereleasePosition(QPoint&);

};

#endif // DISPLAY_WINDOW_H
