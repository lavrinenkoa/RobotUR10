#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qentity.h>
#include <QMainWindow>
#include <QCheckBox>

#include "robot.h"
#include "xmlrpcserverthread.h"
#include "robotvisualisation.h"



extern Qt3DCore::QEntity *rootEntity;
extern Qt3DCore::QEntity *m_sphereEntity;
extern Qt3DCore::QTransform *m_sphereTransform;
extern Robot *robot;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    RobotVisualisation *robotView;
    ~MainWindow();

public slots:
    void posReadyAction(int id);
    void jointPosition(FloatVector jointAngles);
    void haltMessage(int id);
    void haltButtonChanged(bool t);
    void zeroButtonClicked();
    void p1ButtonClicked();
    void p2ButtonClicked();
    void p3ButtonClicked();
    void p4ButtonClicked();
    void p5ButtonClicked();
    void pushButtonClicked();
    void jontButtonClicked();
    void savePosButtonClicked();
    void receivedPosition(QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles);
    void startButtonClicked();
    void savePositions();
    void loadPosButtonClicked();

private:
    Ui::MainWindow *ui;
    XMLRPCServerThread *rpc_server;
    int cSlot=0;
    int rSlot=0;

    int npoints=10;
    QVector<FloatVector> joints;
    QVector<QVector3D> tcps;
    QVector<QCheckBox*> active;
    int current=0;

    TcpPoint* points[10];
};

#endif // MAINWINDOW_H
