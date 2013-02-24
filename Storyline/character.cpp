#include "character.h"
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
Character::Character()
{
    vertices = new QList<float>();
    straight_vertices = new QList<float>();
    curve_vertices = new QList<float>();
    timesteps = new QList<int>();
    straight_lines = new QList<QPair<QPair<float, float>*, QPair<float, float>*> *>();
    curve_lines = new QList<QPair<QPair<float, float>*, QPair<float, float>*> *>();
}
void Character::addVertex(float vertex)
{
    vertices->append(vertex);
}

void Character::addStraightVertex(float vertex)
{
    straight_vertices->append(vertex);
}

void Character::addCurveVertex(float vertex)
{
    curve_vertices->append(vertex);
}
void Character::addTimestep(int timestep)
{
    timesteps->append(timestep);
}
void Character::addStraightLine(QPair<QPair<float, float> *, QPair<float, float> *> *vertex_pair)
{
    straight_lines->append(vertex_pair);
}
void Character::addCurveLine(QPair<QPair<float, float> *, QPair<float, float> *> *vertex_pair)
{
    curve_lines->append(vertex_pair);
}
QList<QPair<QPair<float, float>*, QPair<float, float>*> *> *Character::getStraightLines()
{
    return straight_lines;
}
QList<QPair<QPair<float, float>*, QPair<float, float>*> *> *Character::getCurveLines()
{
    return curve_lines;
}
QList<int> * Character::getTimesteps()
{
    return timesteps;
}

QList<float> * Character::getStraightVertices()
{
    return straight_vertices;
}

QList<float> * Character::getCurveVertices()
{
    return curve_vertices;
}

QList<float>* Character::getVertices()
{
    return vertices;
}

void Character::setName(QString _name)
{
    name = _name;
}
void Character::setID(int _id)
{
    id = _id;
}
int Character::getID()
{
   return id;
}
QString Character::getName()
{
    return name;
}
void Character::setColor(QList<float> *_color)
{
    if (_color == NULL) {
        //int hashCode = qHash(name);
        red = (float)rand() / (float)RAND_MAX;
        green = (float)rand() / (float)RAND_MAX;
        blue = (float)rand() / (float)RAND_MAX;
        QList<float> *tmpColor = new QList<float>();
        tmpColor->append(red);
        tmpColor->append(green);
        tmpColor->append(blue);
        color = tmpColor;
    }
    else {
        color = _color;
    }

}

QList<float> *Character::getColor()
{
    return color;
}
