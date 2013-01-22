#ifndef CHARACTER_H
#define CHARACTER_H
#include <QString>
#include <QList>
#include <qHash>
class Character
{
public:
    Character();
    void addVertex(float vertex);
    void addStraightVertex(float vertex);
    void addCurveVertex(float vertex);
    void addTimestep(int timestep);
    QList<float> *getVertices();
    QList<float> *getStraightVertices();
    QList<float> *getCurveVertices();
    QList<int> *getTimesteps();
    void setID(int id);
    void setName(QString name);
    int getID();
    QString getName();
    void setColor();
    QList<float> *getColor();

private:
    QString name;
    int id;
    int weight;
    QList<float> *vertices;
    QList<float> *curve_vertices;
    QList<float> *straight_vertices;
    QList<int> *timesteps;
    QList<float> *color;
    float red, green, blue;
};

#endif // CHARACTER_H
