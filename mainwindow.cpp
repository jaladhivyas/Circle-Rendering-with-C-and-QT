#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->image_window,SIGNAL(send_mousepressPosition(QPoint&)),this,SLOT(show_mousepressposition(QPoint&)));
    connect(ui->image_window,SIGNAL(send_mousemovePosition(QPoint&)),this,SLOT(show_mousemoveposition(QPoint&)));
    connect(ui->image_window,SIGNAL(send_mousereleasePosition(QPoint&)),this,SLOT(show_mousereleaseposition(QPoint&)));
    ui->image_window->init_grid();
    ui->image_window->init_window();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_mousepressposition(QPoint &pos)
{
    randering.start_xpoint=pos.x();
    randering.start_ypoint=pos.y();
}

void MainWindow::show_mousemoveposition(QPoint &pos)
{
    randering.curr_xpoint = pos.x();
    randering.curr_ypoint = pos.y();
    randering.curr_radius = int(sqrt(pow(( randering.curr_xpoint-randering.start_xpoint),2)+ \
                                    pow((randering.curr_ypoint-randering.start_ypoint),2)));
    randering.inner_radius = randering.outer_radius = randering.curr_radius;

    int rect_xpoint  =  randering.start_xpoint-randering.curr_radius;
    int rect_ypoint  =  randering.start_ypoint-randering.curr_radius;
    int rect_width   =  2*randering.curr_radius;
    int rect_height  =  2*randering.curr_radius;

    bool check_circle= ui->image_window->check_pointinRect(rect_xpoint,rect_ypoint,rect_width,rect_height);
    if(check_circle==false){
         // ui->data_value->setText("False");
          ui->image_window->init_window();
    }
    else{
        randering.draw_circle=true;
        randering.nearest_points=false;
        ui->image_window->init_window();
        ui->image_window->display_randering(randering);
    }
}

void MainWindow::show_mousereleaseposition(QPoint &pos)
{
    randering.draw_circle=true;
    randering.nearest_points=true;
    ui->image_window->init_window();
    ui->image_window->display_randering(randering);

}
