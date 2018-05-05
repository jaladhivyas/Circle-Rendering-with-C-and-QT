#include "display_window.h"

display_window::display_window(QWidget *parent):QLabel(parent)
{
    this->setMouseTracking(true);
}

void display_window::init_grid()
{
    // It initialize the grid parameters by calculating
    //start and end locations of Grid with increments in x and y locations

    this-> grid_elements.start_gridx= grid_size;
    this-> grid_elements.start_gridy=grid_size;
    this->grid_elements.end_gridx= this->size().width()-grid_elements.start_gridx;
    this->grid_elements.end_gridy=this->size().height()-grid_elements.start_gridy;
    this->grid_elements.x_inc=(int)((this->size().width()-2*grid_size)/grid_size);
    this->grid_elements.y_inc= (int)((this->size().width()-2*grid_size)/grid_size);
    img= QImage(this->size().width(),this->size().height(),QImage::Format_RGB32);
    this->init_window();
}

void display_window::init_window()
{
    // It finds the grid point location and change its color to white

    int x_idx=0,y_idx=0;
    img.fill(qRgb(0, 0, 0));  // fill the image with black color only

    for(int i=grid_elements.start_gridx;i<grid_elements.end_gridx;i+=grid_elements.x_inc)
    {
        grid_elements.x_grid[x_idx]=i;// Store the locations of grid points of first columan
        for(int j=grid_elements.start_gridy;j<grid_elements.end_gridy;j+=grid_elements.y_inc)
        {
             img.setPixel(i,j,qRgb(255,255,255));
              if(i==grid_elements.start_gridx)
              {
                   grid_elements.y_grid[y_idx]=j; // Store the locations of grid points of first row
                   y_idx=y_idx+1;
              }
        }
        x_idx++;
      }

    this->setPixmap(QPixmap::fromImage(img));// write an image into label
}

void display_window::draw_nearespoints(int search_x,int search_y, int theta,MainWindow::circle_elememnt &c_elements,QPainter &painter)
{
    int found_x=0,found_y=0;
    /*
      for a given point p(search_x,search_y) the following for loops finds its nearest grid point
      (found_x, found_y)
    */
    for(int i=0;i<grid_size-1;i++){
        if(search_x >=grid_elements.x_grid[i] && search_x <=grid_elements.x_grid[i+1])
            found_x=grid_elements.x_grid[i];
        if(search_y >= grid_elements.y_grid[i] && search_y <=grid_elements.y_grid[i+1])
            found_y=grid_elements.y_grid[i];      
    }

   /* start and end locations of search _roi*/

    int start_rectx=search_x-(int)(grid_elements.x_inc/2.0);
    int start_recty=search_y-(int)(grid_elements.y_inc/2.0);

    QRect search_roi(start_rectx,start_recty,grid_elements.x_inc,grid_elements.y_inc);
    QVector<QPoint> grid_points;

    /* four nearest grid points for a given (search_x,search_y) */

    grid_points.push_back(QPoint(found_x,found_y));
    grid_points.push_back(QPoint(found_x+grid_elements.x_inc,found_y));
    grid_points.push_back(QPoint(found_x,found_y+grid_elements.x_inc));
    grid_points.push_back(QPoint(found_x+grid_elements.x_inc,found_y+grid_elements.y_inc));   

    for(int i=0;i< grid_points.size();i++){// for all nearest grid points
        if(search_roi.contains(grid_points[i])){ // check if search_roi contain grid_points[i]

            // caculate the distance  and update inner and outer radius according to it
            int dist= (int)(ceil((sqrt(pow((grid_points[i].x()-c_elements.start_xpoint),2)+ \
                      pow((grid_points[i].y()-c_elements.start_ypoint),2)))));
            if(dist<c_elements.curr_radius && dist<c_elements.inner_radius)
                c_elements.inner_radius=dist;
            if(dist>c_elements.curr_radius && dist>c_elements.outer_radius)
                c_elements.outer_radius=dist;

            // find and update other points by changing sign of sine and cosine functions.
            painter.drawPoint(grid_points[i].x(),grid_points[i].y());
            if(theta > 0 || theta < 90){
              painter.drawPoint(grid_points[i].x(),c_elements.start_ypoint+(int)(ceil(dist*sin(theta*3.14159265/180.0))));
              painter.drawPoint(c_elements.start_xpoint-(int)(dist*(cos(theta*3.14159265/180.0))),grid_points[i].y());
              painter.drawPoint(c_elements.start_xpoint-(int)(dist*(cos(theta*3.14159265/180.0))),\
                                c_elements.start_ypoint+(int)(dist*(sin(theta*3.14159265/180.0))));
            }

            if(theta==0)
                painter.drawPoint(grid_points[i].x(),c_elements.start_ypoint+(int)(dist*sin(theta*3.14159265/180.0)));
            if(theta==90)
                painter.drawPoint(c_elements.start_xpoint,c_elements.start_ypoint+dist);
        }
        // update the label with modified image
        this->setPixmap(QPixmap::fromImage(img));
    }
}


bool display_window::check_pointinRect(int r_left, int r_top, int r_width, int r_height)
{
   /*This function return true if a bounding box of circle is inside the grid_points otherwise it returns false*/
    QPoint p1(r_left,r_top);
    QPoint p2(r_left+r_width,r_top+r_height);
    QRect display_rect(2*grid_size,2*grid_size,this->size().width()-2*grid_size,this->size().height()-2*grid_size);
    if(display_rect.contains(p1) &&  display_rect.contains(p2))
        return true;
    else
        return false;    
}

void display_window::display_randering(MainWindow::circle_elememnt &c_elements)
{   
    this->init_window();

    QRectF rectangle1(c_elements.start_xpoint-c_elements.curr_radius,\
                     c_elements.start_ypoint-c_elements.curr_radius,\
                     2*c_elements.curr_radius, 2*c_elements.curr_radius);
    if(c_elements.draw_circle==1){
        cout<<"Inside draw_circle"<<endl;
        int foundx=0,foundy=0;
        cout<<"Inside draw_circle=2"<<endl;
       // QImage img=grid_elements.img;
        QPainter painter(&img);
        cout<<"-------"<<endl;
        for(int i=0;i<=90;i+=10){
            painter.setPen(QPen(Qt::blue, 3));
            int search_x = c_elements.start_xpoint + (int)(c_elements.curr_radius*cos(i*3.14159265/180.0));
            int search_y = c_elements.start_ypoint - (int)(c_elements.curr_radius*sin(i*3.14159265/180.0));
            cout<<"Inside draw_circle ----------- 2"<<endl;
            this->draw_nearespoints(search_x,search_y,i,c_elements,painter);
            cout<<"Inside draw_circle  2"<<endl;
         }

        if(c_elements.nearest_points==0){

            QRectF rectangle2(c_elements.start_xpoint-c_elements.inner_radius,\
                             c_elements.start_ypoint-c_elements.inner_radius, \
                             2*c_elements.inner_radius, 2*c_elements.inner_radius);
            QRectF rectangle3(c_elements.start_xpoint-c_elements.outer_radius,\
                             c_elements.start_ypoint-c_elements.outer_radius,\
                             2*c_elements.outer_radius, 2*c_elements.outer_radius);
            painter.setPen(QPen(Qt::blue, 2));
            painter.drawEllipse(rectangle1);
            painter.setPen(QPen(Qt::red, 2));
            painter.drawEllipse(rectangle2);
            painter.setPen(QPen(Qt::red, 2));
            painter.drawEllipse(rectangle3);
          }
        this->setPixmap(QPixmap::fromImage(img));
    }
}


void display_window::mouseMoveEvent(QMouseEvent *mouse_event)
{
    QPoint mouse_pos= mouse_event->pos();

    if(mouse_pos.x()<=this->size().width() && mouse_pos.y()<=this->size().height()){
        if(mouse_pos.x()>=0 &&  mouse_pos.y()>=0){
            if(mouse_event->buttons()==Qt::LeftButton)
                  emit send_mousemovePosition(mouse_pos);
        }
    }
}

void display_window::mousePressEvent(QMouseEvent *mouse_event)
{
    if(mouse_event->button()==Qt::LeftButton){
        QPoint mouse_pos= mouse_event->pos();
        emit send_mousepressPosition(mouse_pos);
    }
}

void display_window::mouseReleaseEvent(QMouseEvent *mouse_event)
{
    QPoint mouse_pos= mouse_event->pos();
    emit send_mousereleasePosition(mouse_pos);
}
