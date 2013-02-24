#ifndef CHARACTER_H
#define CHARACTER_H
#include <QString>
#include <QList>
#include <QPair>
#include <qHash>
class Character
{
public:
    Character();
    void addVertex(float vertex);
    void addStraightVertex(float vertex);
    void addCurveVertex(float vertex);
    void addTimestep(int timestep);
    void addStraightLine(QPair<QPair<float, float>*, QPair<float, float>*> *vertex_pair);
    void addCurveLine(QPair<QPair<float, float>*, QPair<float, float>*> *vertex_pair);
    QList<float> *getVertices();
    QList<float> *getStraightVertices();
    QList<float> *getCurveVertices();
    QList<int> *getTimesteps();
    QList<QPair<QPair<float, float>*, QPair<float, float>*> *> *getStraightLines();
    QList<QPair<QPair<float, float>*, QPair<float, float>*> *> *getCurveLines();
    void setID(int id);
    void setName(QString name);
    int getID();
    QString getName();
    void setColor(QList<float> *_color);
    QList<float> *getColor();

private:
    QString name;
    int id;
    int weight;
    QList<float> *vertices;
    QList<float> *curve_vertices;
    QList<float> *straight_vertices;
    QList<int> *timesteps;
    QList<QPair<QPair<float, float>*, QPair<float, float>*> *> *straight_lines;
    QList<QPair<QPair<float, float>*, QPair<float, float>*> *> *curve_lines;
    QList<float> *color;
    float red, green, blue;
};
inline bool operator==(Character &ch1, Character &ch2)
{
    return ch1.getName() == ch2.getName();
}
#endif // CHARACTER_H
