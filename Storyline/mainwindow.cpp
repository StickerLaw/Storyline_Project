#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global_functions.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    _current_dir = "/Users/yeseulpark/Storyline_Project/Storyline/Data";

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
    //loadInteractionSessions(fileName, iscontainer);

    outputFileName = _current_dir + "outputFile";
    /*Load data and set interaction sessions*/
    //loadData(fileName, iscontainer);
    QFile file_input;
    file_input.setFileName(fileName);
    if (!file_input.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QByteArray line;
    int id = 0;
    int temp_id = 0;
    int location = -1;
    int prev_time = 0;
    iscontainer->createCompareList_prev();
    iscontainer->open = true;
    while(!file_input.atEnd()) {
        line = file_input.readLine();
        QStringList temp;

        temp = ((QString)line).split("\t");

        int time = ((QString)temp[0]).toInt();

        iscontainer->createCompareList_now();

        if (temp.size() > 1)
        {
            qDebug() << "\n";
            qDebug() << "When " << time << ": ";
            for (int i = 1; i < temp.size(); i++)
            {
                /*Get the list of the members per time*/
                QStringList members;
                QString membersSingleStr;
                members = ((QString)temp[i]).trimmed().split(",");
                members.sort();
                membersSingleStr = members.join(",");
                location = i;

                //InteractionSession* existing_is = iscontainer->find(membersSingleStr);
                /*Get interaction sessions of the previous time*/
                QList<InteractionSession*> *_tisList = iscontainer->getTempISList_prev();
                InteractionSession* existing_is = NULL;

                if (!_tisList->isEmpty()) {
                    for (int j = 0; j < _tisList->size(); j++)
                    {
                        /*Check if the current members exists in the previous interaction sessions*/
                        if (_tisList->at(j)->getMembersOneStr().compare(membersSingleStr) == 0)
                        {
                            existing_is = _tisList->at(j);
                        }
                    }
                }
                /*If no exist in the previous interaction sessions, create new interaction session for these current members*/
                if (existing_is == NULL) {
                    InteractionSession* is = new InteractionSession(id, members, time, time+1, location);
                    is->setMembersOneStr(membersSingleStr);
                    iscontainer->add(is);

                    id++;
                }
                /*Exist in the previous interaction sessions, update the ending time of the interaction session of the current members*/
                else {
                    iscontainer->update(existing_is, prev_time, time+1);
                }

            }

        }

        /*Set the interaction sessions of the current timeslot*/
        for (int k = 1; k < temp.size(); k++)
        {
            QStringList members;
            QString membersSingleStr;
            members = ((QString)temp[k]).trimmed().split(",");
            members.sort();
            membersSingleStr = members.join(",");
            location = k;
            InteractionSession* temp_is = new InteractionSession(temp_id, members, time, time+1, location);
            temp_is->setMembersOneStr(membersSingleStr);
            iscontainer->addTemp(temp_is);
            temp_id++;
        }
//        /*Print prevIS list and current IS list for each time*/
//        QList<InteractionSession*> *tempISList_now = iscontainer->getTempISList_now();
//        QList<InteractionSession*> *tempISList_prev = iscontainer->getTempISList_prev();
//        qDebug() << "Previous interaction sessions:";
//        for (int j = 0; j < tempISList_prev->size(); j++) {
//            InteractionSession *is = tempISList_prev->at(j);
//            is->printIS();
//        }
//        qDebug() << "Current interaction sessions:";
//        for (int j = 0; j < tempISList_now->size(); j++) {
//            InteractionSession *is = tempISList_now->at(j);
//            is->printIS();
//        }

        /*Update the previous time*/
        prev_time = time+1;


        /*Print all IS list for each time*/
        qDebug() << "Whole interaction sessions:";
        QList<InteractionSession*> *tempISList = iscontainer->getISList();
        for (int i = 0; i < tempISList->size(); i++) {
            InteractionSession *is = tempISList->at(i);
            is->printIS();
        }

        initializeConnections(iscontainer->getISList());
        /*Set following interaction sessions and proceeding interaction sessions*/
        registerConnections(iscontainer->getISList());

        /*Compute the layout with the current interaction sessions*/
        layout = computeLayout(iscontainer);

//        /*For current ISs classified*/
//        if (!iscontainer->getClassifiedInteractionSessions()->isEmpty()) {

//            for (int i = 0; i <  iscontainer->getClassifiedInteractionSessions()->size(); i++)
//            {
//                QPair<InteractionSession*, QString> *pair = iscontainer->getClassifiedInteractionSessions()->at(i);
//                InteractionSession* is = pair->first;
//                QString str = pair->second;
//                qDebug() << "classified:" << is->getMembersOneStr() << str << is->getStartTime() << is->getEndTime();
//            }
//        }


//        /*Check if the slot is correctly assigned into the interactionsession*/
//        for (int i = 0; i < iscontainer->getInteractionSessionsWithSlot()->size(); i++)
//        {
//            QPair<InteractionSession*, int> *pair = iscontainer->getInteractionSessionsWithSlot()->at(i);
//            InteractionSession* interaction_session = pair->first;
//            int slot_number = pair->second;
//            qDebug() << "slot assigned:" << interaction_session->getMembersOneStr() << interaction_session->getStartTime() << interaction_session->getEndTime() << "slot number:" << slot_number;

//        }


        /*Set interaction sessions of the previous timeslot*/
        iscontainer->setPrevISList(iscontainer->getTempISList_now());

        if (layout != NULL) {

//            /*Check the layout inside*/
//            QHash<QString, QHash<int, int>* >::iterator it = layout->begin();
//            for (; it != layout->end(); it++)
//            {
//                QString member = it.key();
//                QHash<int, int>* times_and_positions = it.value();
//                QHash<int, int>::iterator it_ = times_and_positions->begin();
//                for (; it_ != times_and_positions->end(); it_++)
//                {
//                    int time = it_.key();
//                    int position = it_.value();
//                    qDebug() << "layout has"<< member << time << position;
//                }
//            }


            /*Write the output layout into txt file*/
            outputLayout(outputFileName, layout, iscontainer);

            /*Render the layout with OpenGL*/
            if (!layout->isEmpty()) {
                sleep(1);
                ui->widget->takeLayout(layout);
                ui->widget->visualize();
            }
        }

        iscontainer->open = false;
    }
    qDebug() << "Visualization over";
    file_input.close();


//    /*Get interaction sessions*/
//    QList<InteractionSession*> *interaction_sessions = iscontainer->getISList();
//    //printContainer();

//    /*Set following interaction sessions and proceeding interaction sessions*/
//    registerConnections(interaction_sessions);

//    /*Compute the layout with the interaction sessions*/
//    layout = computeLayout(iscontainer);

//    /*Write the output layout into txt file*/
//    outputLayout(outputFileName, layout, iscontainer);

//    /*Render the layout with OpenGL*/
//    ui->widget->takeLayout(layout);
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


