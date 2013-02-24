#include "myglwidget.h"

myGLWidget::myGLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    _canvas_width = parent->width();
    _canvas_height = parent->height();


    prev_characters = new QList<Character*>();
    setAutoFillBackground(true);
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
    characters = new QList<Character*>();

    /*Extract each member's line path*/
    for (; it != layout->end(); it++) {
        /*Extract the character information*/
        Character* character = new Character();
        QString member = it.key();
        character->setID(member_count);
        character->setName(member);
        /*Initialize the path*/
        GLfloat prev_y_pos = FLT_MAX;
        int prev_timestep = 0;


        QHash<int, int>* timesteps_and_positions = it.value();
        QList<int> sorted_times = timesteps_and_positions->keys();
        qSort(sorted_times.begin(), sorted_times.end());


        int time_count = 0;
        int timestep = 0;
        for (int i = 0; i < sorted_times.size(); i++)
        {
            timestep = sorted_times.at(i);
            GLfloat y_pos = static_cast< float >(timesteps_and_positions->value(timestep));
            qDebug() << "path of " << member << ":" << i << timestep << ", " << y_pos;
            character->addVertex((float)(timestep)*width_per_time);
            character->addVertex((float)y_pos*height_per_time);

            if (timestep == 0) {
                character->addTimestep(timestep);
                character->addTimestep(timestep);
            }
            if (time_count > 0) {

                if (abs(prev_timestep - timestep) == 1 ) {

                    if (prev_y_pos == y_pos) {
                        /*Create a pair of a pair of vertices*/
                        QPair<QPair<float, float>*, QPair<float, float>*> *vertex_pair = new QPair<QPair<float, float>*, QPair<float, float>*>();
                        QPair<float, float> *vertex1 = new QPair<float, float>();
                        vertex1->first = (float)(timestep)*width_per_time;
                        vertex1->second = (float)y_pos*height_per_time;
                        QPair<float, float> *vertex2 = new QPair<float, float>();
                        vertex2->first = (float)(timestep+1)*width_per_time;
                        vertex2->second = (float)y_pos*height_per_time;
                        vertex_pair->first = vertex1;
                        vertex_pair->second = vertex2;
                        character->addStraightLine(vertex_pair);
                        character->addStraightVertex((float)(timestep)*width_per_time);
                        character->addStraightVertex((float)y_pos*height_per_time);
                        character->addTimestep(timestep);
                        character->addTimestep(timestep);
                        character->addStraightVertex((float)(timestep+1)*width_per_time);
                        character->addStraightVertex((float)y_pos*height_per_time);
                        character->addTimestep(timestep+1);
                        character->addTimestep(timestep+1);

                    }
                    else {
                        GLfloat temp_vertex1_x = (float)(timestep) * width_per_time;
                        GLfloat temp_vertex1_y = (float)prev_y_pos* height_per_time;
                        GLfloat temp_vertex2_x = (float)(timestep+1) * width_per_time;
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

        if (prev_characters->isEmpty()) {

            character->setColor(NULL);
        }
        else {

            for (int i = 0; i < prev_characters->size(); i++)
            {

                if(prev_characters->at(i)->getName().compare(character->getName()) == 0)
                {
                    QList<float> *color = prev_characters->at(i)->getColor();
                    character->setColor(color);
                    break;

                }

                else
                {
                    character->setColor(NULL);

                }

            }
        }
        characters->append(character);
        member_count++;

    }

    prev_characters = characters;
    setSize();

}
void myGLWidget::visualize() {
    updateGL();
}

void myGLWidget::setSize() {
    //Get the minimum position of one node member (A<B)
    QList<float> *time_vertices = new QList<float>();
    QList<float> *y_pos_vertices = new QList<float>();
    if (characters != NULL) {
        for (int i = 0; i < characters->size(); i++) {
            Character *character = characters->at(i);
            if (!character->getCurveVertices()->isEmpty()) {
                for (int j = 0; j < character->getCurveVertices()->size(); j++) {
                    if (j % 2 == 0) {
                        time_vertices->append(character->getCurveVertices()->at(j));
                    }
                    else {
                        y_pos_vertices->append(character->getCurveVertices()->at(j));
                    }
                }
            }
            if (!character->getStraightVertices()->isEmpty()) {
                for (int j = 0; j < character->getStraightVertices()->size(); j++) {
                    if (j % 2 == 0) {
                        time_vertices->append(character->getStraightVertices()->at(j));
                    }
                    else {
                        y_pos_vertices->append(character->getStraightVertices()->at(j));
                    }
                }
            }
        }

        QList<float> *sorted_time_vertices = time_vertices;
        QList<float> *sorted_y_pos_vertices = y_pos_vertices;
        if (!sorted_time_vertices->isEmpty() && !sorted_y_pos_vertices->isEmpty())
        {
            qSort(sorted_time_vertices->begin(), sorted_time_vertices->end());
            qSort(sorted_y_pos_vertices->begin(), sorted_y_pos_vertices->end());
            bottom_in_height = sorted_y_pos_vertices->first();
            top_in_height = sorted_y_pos_vertices->last();
            left_in_width = sorted_time_vertices->first();
            right_in_width = sorted_time_vertices->last();
            qDebug() << "painted in this window, left: " << left_in_width-0.01 << "right: "<< right_in_width+0.01 << "bottom: " << bottom_in_height*2 << "top: " << top_in_height*2;
        }
    }
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
    qglClearColor(QColor(255, 255, 255, 255));
    glDisable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glDisable(GL_TEXTURE_2D);
}

void myGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D (left_in_width-0.01, right_in_width+0.01, bottom_in_height*2, top_in_height*2);


    glEnable (GL_LINE_SMOOTH);
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (characters != NULL) {
        for (int i = 0; i < characters->size(); i++) {

            Character *character = characters->at(i);
            if (character != NULL) {
                glColor3f(character->getColor()->at(0), character->getColor()->at(1), character->getColor()->at(2));

                glRasterPos2f(character->getVertices()->at(0)+0.09, character->getVertices()->at(1));

                char *s = character->getName().toUtf8().data();

                if (s && strlen(s)) {
                      while (*s) {
                         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *s);
                         s++;
                      }
                 }
                glLineWidth(3);
                glBegin(GL_LINES);

                for (int a = 0; a < character->getCurveVertices()->size() - 3; a+=4)
                {
                    glVertex2f(character->getCurveVertices()->at(a), character->getCurveVertices()->at(a+1));
                    glVertex2f(character->getCurveVertices()->at(a+2), character->getCurveVertices()->at(a+3));
                }
                glEnd();

                glBegin(GL_LINES);
                for (int a = 0; a < character->getStraightLines()->size(); a++)
                {
                    glVertex2f (character->getStraightLines()->at(a)->first->first, character->getStraightLines()->at(a)->first->second);
                    //qDebug() << "straight line" << character->getStraightLines()->at(a)->first->first <<character->getStraightLines()->at(a)->first->second << character->getStraightLines()->at(a)->second->first << character->getStraightLines()->at(a)->second->second;
                    glVertex2f (character->getStraightLines()->at(a)->second->first, character->getStraightLines()->at(a)->second->second);
                }
                glEnd();
    //            glBegin(GL_LINES);
    //            for (int a = 0; a < character->getStraightVertices()->size()-3; a+=2)
    //            {
    //                if (character->getStraightVertices()->at(a+1) == character->getStraightVertices()->at(a+3))
    //                {
    //                    glVertex2f(character->getStraightVertices()->at(a), character->getStraightVertices()->at(a+1));
    //                    glVertex2f(character->getStraightVertices()->at(a+2), character->getStraightVertices()->at(a+3));
    //                }
    //            }
    //            glEnd();
                //glFlush();
            }
            glLineWidth(1);
            glBegin(GL_LINES);

            glColor3f(0, 0, 0);
            for (int a = 0; a < character->getTimesteps()->size(); a++)
            {
                glVertex2f((float)(character->getTimesteps()->at(a)*width_per_time), bottom_in_height*2);
                glVertex2f((float)(character->getTimesteps()->at(a)*width_per_time), top_in_height*2);
            }
            glEnd();
        }
    }

}
void myGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    gluOrtho2D (left_in_width-0.01, right_in_width+0.01, bottom_in_height*2, top_in_height*2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
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
