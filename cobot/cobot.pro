#-------------------------------------------------
#
# Project created by QtCreator 2019-01-31T15:40:51
#
#-------------------------------------------------

QT += core gui
QT += 3dcore 3drender 3dinput 3dextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cobot
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    xmlrpcserverthread.cpp \
    robot.cpp \
    robotvisualisation.cpp \
    joint.cpp \
    plane.cpp \
    cobotzone.cpp \
    safetyplane.cpp \
    enginezone.cpp

HEADERS += \
        mainwindow.h \
    xmlrpcserverthread.h \
    robot.h \
    robotvisualisation.h \
    joint.h \
    common.h \
    plane.h \
    cobotzone.h \
    safetyplane.h \
    enginezone.h

FORMS += \
        mainwindow.ui

LIBS += -lcurl \
        -lxmlrpc_server++ \
        -lxmlrpc++ \
        -lxmlrpc_client++ \
        -lxmlrpc_server_abyss++
#        -lxmlrpc_server \
#        -lxmlrpc \
#        -lxmlrpc_server_pstream++ \
#        -lxmlrpc_packetsocket \
#        -lxmlrpc_xmlparse \
#        -lxmlrpc_xmltok \
#        -lxmlrpc_util \
#        -lxmlrpc_client




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    rect.txt \
    template.urp \
    rect.urp \
    halt.urp \
    zero_position.urp \
    set_joint_to.urp \
    move_to.urp \
    get_position.urp



