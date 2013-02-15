#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <myglwidget.h>
#include <qdebug.h>
#include "interactionsessioncontainer.h"
#include "interactionsession.h"
#include <QTimer>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void printContainer();
    void clearISContainer();

private:
    Ui::MainWindow *ui;
    QString _current_dir;
    InteractionSessionContainer *iscontainer;
    QString outputFileName;
    QHash<QString, QHash<int, int>* >* layout;


private slots:
    void stream_data();
    void visualize();
    void close();

};

#endif // MAINWINDOW_H
