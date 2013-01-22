#-------------------------------------------------
#
# Project created by QtCreator 2012-12-16T12:43:17
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Storylines
TEMPLATE = app
INCLUDEPATH += /Users/yeseulpark/Storylines/boost_1_52_0
LIBS += -L/Users/yeseulpark/Storylines/boost_1_52_0/libs

SOURCES += main.cpp\
        mainwindow.cpp \
    interactionsession.cpp \
    character.cpp \
    interactionsessioncontainer.cpp \
    slotbaselayout.cpp \
    sessionlayout.cpp \
    slotsegments.cpp \
    slotlayout.cpp \
    slotsegment.cpp \
    global_functions.cpp \
    myglwidget.cpp

HEADERS  += mainwindow.h \
    interactionsession.h \
    character.h \
    interactionsessioncontainer.h \
    slotbaselayout.h \
    sessionlayout.h \
    slotsegments.h \
    slotlayout.h \
    slotsegment.h \
    global_functions.h \
    myglwidget.h

FORMS    += mainwindow.ui















































