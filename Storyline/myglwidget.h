#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#include <character.h>
#include <QtOpenGL/QGLWidget>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <QFile>
#include <QHash>
#include <QDebug>
#include <cfloat>
#include "interactionsession.h"
#define MAXIMUM INT_MAX
#define MINIMUM INT_MIN
class myGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit myGLWidget(QWidget *parent = 0);
    //QSize minimumSizeHint() const;
    void takeLayout(QHash<QString, QHash<int, int>* >* _layout);
    void visualize();
    void setSize();
    void set_width_per_time(float _width_per_time);
    void set_height_per_time(float _height_per_time);
    QList<GLfloat> *compute_curve_coordinates(float x1, float y1, float x2, float y2, int num_of_time_step);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int, int);
//    void mousePressEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);
//    void mouseMoveEvent(QMouseEvent *);
//    void wheelEvent(QWheelEvent *);
private:
    int _canvas_width, _canvas_height;
    QHash<QString, QHash<int, int>* >* layout;
    QHash<QString, QHash<int, int>* >* prev_layout;
    float width_per_time, height_per_time;
    int number_of_vertices, number_of_curve_vertices, number_of_straight_vertices;
    QList<GLfloat> *vertice_array, *curve_vertice_array, *color_array, *timesteps;
    QList<GLfloat> *temp_vertices, *temp_vertices_color, *temp_curve_vertices;
    QList<Character*> *characters;
    QList<Character*> *prev_characters;
    QList<Character*> *current_characters;
    float line_width;
    float bottom_in_height;
    float top_in_height;
    float left_in_width;
    float right_in_width;
    float scale;
    QPoint anchor;
};

#endif // MYGLWIDGET_H
