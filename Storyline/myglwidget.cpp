#include "myglwidget.h"

myGLWidget::myGLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    _canvas_width = parent->width();
    _canvas_height = parent->height();


}

QList<GLfloat> *myGLWidget::compute_curve_coordinates(float x1, float y1, float x2, float y2, int num_of_time_step){

    /* the curve will contain "num_of_time_step" lines,
      each line consists of 2 vertices,
      and each vertex is represented by 2 points.
      Therefore, the size of the output array will be "num_of_time_step" * 4 */

    float x_step;
    x_step = float(x2 - x1) / (float)num_of_time_step;


    int size_of_output_array;
    size_of_output_array = num_of_time_step * 4;

    QList<GLfloat> *vertices = new QList<GLfloat>();

    vertices->insert(0, x1);
    vertices->insert(1, y1);

    for (int i = 1; i < num_of_time_step; i++){
        float x, y, left, right;
        x = x1 + (x_step * (float)i);
        left = x - x1;
        right = x2 - x;
        left = left * left;
        right = right * right;
        y = (right* y1 + left* y2) / (left+right);// + 0.5;

        vertices->insert(i*4-2, x);
        vertices->insert(i*4-1, y);
        vertices->insert(i*4, x);
        vertices->insert(i*4+1, y);
    }

    vertices->insert(num_of_time_step*4-2, x2);
    vertices->insert(num_of_time_step*4-1, y2);

    return vertices;

}
void myGLWidget::takeLayout(QHash<QString, QHash<int, int>* >* _layout) {
    layout = _layout;
    QHash<QString, QHash<int, int>* >::iterator it = layout->begin();
    int member_count = 0;

    for (; it != layout->end(); it++) {
        Character* character = new Character();

        QString member = it.key();
        character->setID(member_count);
        character->setName(member);

        GLfloat prev_y_pos;
        int prev_timestep = 0;
        QHash<int, int>* timesteps_and_positions = it.value();
        QList<int> sorted_times = timesteps_and_positions->keys();
        qSort(sorted_times.begin(), sorted_times.end());

        int time_count = 0;
        for (int i = 0; i < sorted_times.size(); i++)
        {
            int timestep = sorted_times.at(i);
            GLfloat y_pos = static_cast< float >(timesteps_and_positions->value(timestep));

            character->addVertex((float)(timestep)*width_per_time);
            character->addVertex((float)y_pos*height_per_time);

            if (time_count > 0) {

                if (abs(prev_timestep - timestep) == 1 ) {
                    if (prev_y_pos == y_pos) {
                        character->addStraightVertex((float)(prev_timestep)*width_per_time);
                        character->addStraightVertex((float)prev_y_pos*height_per_time);
                        character->addTimestep(prev_timestep);
                        character->addTimestep(prev_timestep);
                        character->addStraightVertex((float)(timestep)*width_per_time);
                        character->addStraightVertex((float)y_pos*height_per_time);
                        character->addTimestep(timestep);
                        character->addTimestep(timestep);
                    }
                    else {
                        GLfloat temp_vertex1_x = (float)(prev_timestep) * width_per_time;
                        GLfloat temp_vertex1_y = (float)prev_y_pos* height_per_time;
                        GLfloat temp_vertex2_x = (float)(timestep) * width_per_time;
                        GLfloat temp_vertex2_y = (float)y_pos * height_per_time;

                        int num_of_curve_steps = 10;
                        QList<GLfloat> *temp_curve = compute_curve_coordinates(temp_vertex1_x, temp_vertex1_y, temp_vertex2_x, temp_vertex2_y, num_of_curve_steps);
                        for (int i = 0; i < num_of_curve_steps*4; i++) {
                            character->addCurveVertex(temp_curve->at(i));
                            character->addTimestep(timestep);
                        }

                    }
                }
            }

            prev_y_pos = y_pos;
            prev_timestep = timestep;
            time_count++;

        }
        character->setColor();
        characters->append(character);
        member_count++;
    }

    setSize();

}
void myGLWidget::visualize() {
    updateGL();
}

void myGLWidget::setSize() {
    //Get the minimum position of one node member (A<B)
    QList<float> *time_vertices = new QList<float>();
    QList<float> *y_pos_vertices = new QList<float>();

    for (int i = 0; i < characters->size(); i++) {
        Character *character = characters->at(i);
        for (int j = 0; j < character->getCurveVertices()->size(); j++) {
            if (j % 2 == 0) {
                time_vertices->append(character->getCurveVertices()->at(j));
            }
            else {
                y_pos_vertices->append(character->getCurveVertices()->at(j));
            }
        }
        for (int j = 0; j < character->getStraightVertices()->size(); j++) {
            if (j % 2 == 0) {
                time_vertices->append(character->getStraightVertices()->at(j));
            }
            else {
                y_pos_vertices->append(character->getStraightVertices()->at(j));
            }
        }
    }
    QList<float> *sorted_time_vertices = time_vertices;
    QList<float> *sorted_y_pos_vertices = y_pos_vertices;
    qSort(sorted_time_vertices->begin(), sorted_time_vertices->end());
    qSort(sorted_y_pos_vertices->begin(), sorted_y_pos_vertices->end());
    bottom_in_height = sorted_y_pos_vertices->first();
    top_in_height = sorted_y_pos_vertices->last();
    left_in_width = sorted_time_vertices->first();
    right_in_width = sorted_time_vertices->last();
}

void myGLWidget::set_width_per_time(float _width_per_time) {
    width_per_time = _width_per_time;
}
void myGLWidget::set_height_per_time(float _height_per_time) {
    height_per_time = _height_per_time;
}

void myGLWidget::initializeGL()
{
    set_width_per_time(0.1);
    set_height_per_time(0.1);
    characters = new QList<Character*>();

    glClearColor(1.f, 1.f, 1.f, 0.f);
    glDisable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
}

void myGLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluOrtho2D (left_in_width, right_in_width, bottom_in_height*2, top_in_height*2);
    qDebug() << left_in_width << right_in_width << bottom_in_height*2 << top_in_height*2;
    glLineWidth(2);

    glEnable (GL_LINE_SMOOTH);
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < characters->size(); i++) {
        Character *character = characters->at(i);
        glColor3f(character->getColor()->at(0), character->getColor()->at(1), character->getColor()->at(2));

        glBegin(GL_LINES);

        for (int a = 0; a < character->getCurveVertices()->size() - 3; a+=4)
        {
            glVertex2f(character->getCurveVertices()->at(a), character->getCurveVertices()->at(a+1));
            glVertex2f(character->getCurveVertices()->at(a+2), character->getCurveVertices()->at(a+3));
        }
        glEnd();
        glBegin(GL_LINES);
        for (int a = 0; a < character->getStraightVertices()->size()-3; a+=4)
        {
            glVertex2f(character->getStraightVertices()->at(a), character->getStraightVertices()->at(a+1));
            glVertex2f(character->getStraightVertices()->at(a+2), character->getStraightVertices()->at(a+3));
        }
        glEnd();
    }
}
void myGLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport(((width - side)/2), ((height -side)/2), side, side);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D (left_in_width, right_in_width, bottom_in_height*2, top_in_height*2);
    glMatrixMode(GL_MODELVIEW);
}
//void myGLWidget::mousePressEvent(QMouseEvent *event)
//{
//    anchor = event->pos();
//}
//void myGLWidget::mouseMoveEvent(QMouseEvent *event) {
//    QPoint diff = event->pos() - anchor;
//    if (e->buttons() & Qt::LeftButton) {
//        rot_x += diff.y()/5.0f;
//        rot_y += diff.x()/5.0f;
//    } else if (e->buttons() & Qt::RightButton) {
//        rot_z += diff.x()/5.0f;
//    }

//    anchor = event->pos();
//    updateGL();
//}
//void myGLWidget::mouseReleaseEvent(QMouseEvent *event) {

//}
//void myGLWidget::wheelEvent(QWheelEvent *event)
//{
//    event->delta() > 0 ? scale += scale*0.1f : scale -= scale*0.1f;
//    updateGL();
//}
