#----------------------------------------------------------------------------------
#
# Project created by QtCreator 2012-12-16T12:43:17
# Rewritten with C++ by Yeseul Park from the original source of
# http://vis.cs.ucdavis.edu/~tanahashi/data_downloads/storyline_visualizations/
#
#----------------------------------------------------------------------------------

QT       += core gui opengl

TARGET = Storylines
TEMPLATE = app

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

mac: LIBS += -framework GLUT
else:unix|win32: LIBS += -lGLUT
