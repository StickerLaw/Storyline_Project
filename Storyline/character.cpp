#include "character.h"
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
Character::Character()
{
    vertices = new QList<float>();
    straight_vertices = new QList<float>();
    curve_vertices = new QList<float>();
    timesteps = new QList<int>();
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
void Character::setColor()
{
    //int hashCode = qHash(name);
    red = (float)rand() / (float)RAND_MAX;
    green = (float)rand() / (float)RAND_MAX;
    blue = (float)rand() / (float)RAND_MAX;
    color = new QList<float>();
    color->append(red);
    color->append(green);
    color->append(blue);
}

QList<float> *Character::getColor()
{
    return color;
}
