#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global_functions.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    _current_dir = "/Users/yeseulpark/Storylines/";

    QAction *a;
    QMenu *m = ui->menuFile;
    a = m->actions().at(0);
    connect(a, SIGNAL(triggered()), this, SLOT(stream_data()));
    a = m->actions().at(1);
    connect(a, SIGNAL(triggered()), this, SLOT(visualize()));
    a = m->actions().at(2);
    connect(a, SIGNAL(triggered()), this, SLOT(close()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::visualize()
{
    ui->widget->visualize();
}
void MainWindow::close()
{
    clearISContainer();
    this->close();
}

void MainWindow::stream_data()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open a file"),
                                                    _current_dir,
                                                    tr("(*.*)"));

    iscontainer = new InteractionSessionContainer();

    /*Load interaction sessions directly*/
    loadInteractionSessions(fileName, iscontainer);

    /*Load data and set interaction sessions*/
    //loadData(fileName, iscontainer);

    outputFileName = _current_dir + "outputFile";

    /*Get interaction sessions*/
    QList<InteractionSession*> *interaction_sessions = iscontainer->getISList();
    //printContainer();

    /*Set following interaction sessions and proceeding interaction sessions*/
    registerConnections(interaction_sessions);

    /*Compute the layout with the interaction sessions*/
    layout = computeLayout(iscontainer);

    /*Write the output layout into txt file*/
    outputLayout(outputFileName, layout, iscontainer);

    /*Render the layout with OpenGL*/
    ui->widget->takeLayout(layout);
}

void MainWindow::clearISContainer()
{
    iscontainer->clear();
}


void MainWindow::printContainer()
{
    QList<InteractionSession*> *_isList = iscontainer->getISList();
    for (int i = 0; i < _isList->size(); i++)
    {
        InteractionSession *_is = _isList->at(i);
        _is->printIS();

    }

//    QList<InteractionSession*> *_tisList = iscontainer->getTempISList_prev();
//    if (_tisList->isEmpty()) qDebug() << "nothing yet";
//    else {
//        for (int j = 0; j < _tisList->size(); j++)
//        {
//            InteractionSession *_tis = _tisList->at(j);
//            _tis->printIS();
//        }
//    }
    qDebug() << "\n";
}


