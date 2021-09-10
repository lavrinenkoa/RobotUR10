#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <Qt3DExtras>
#include <QtMath>

//#include "robotvisualisation.h"
//#include "robot.h"


using namespace std;

Qt3DCore::QEntity *rootEntity;
Qt3DCore::QTransform *m_sphereTransform;
Qt3DCore::QEntity *m_sphereEntity;

Robot *robot;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Robot 3D Visualisation
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));

    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(640, 480));
    container->setMaximumSize(screenSize);

    ui->verticalLayout->addWidget(container);

    // Robot Visualisation objects
    robotView = new RobotVisualisation(view);
    robotView->add_mesh_engine();

    // Safety Zone
    QVector3D zonePosition = QVector3D(2, -2, 0);
    QVector3D zoneSize = QVector3D(4.6f, 4.6f, 4.6f);
    CobotZone* cobotZone= new CobotZone(zonePosition, zoneSize);
    zonePosition = QVector3D(-1.0f, 0.1f, 0);
    zoneSize = QVector3D(1.5f, 1.4f, 1.15f);
    EngineZone* engineZone= new EngineZone(zonePosition, zoneSize);
    // Robot
    robot = new Robot(this, "10.0.13.200", 30002, "enp0s31f6", 8080,
                      cobotZone, engineZone, robotView);

    connect(robot,SIGNAL(positionReady(int)), SLOT(posReadyAction(int)));
    connect(robot,SIGNAL(halted(int)), SLOT(haltMessage(int)));

    ui->spinBox_0->setSingleStep(10);
    ui->spinBox_1->setSingleStep(10);
    ui->spinBox_2->setSingleStep(10);
    ui->spinBox_3->setSingleStep(10);
    ui->spinBox_4->setSingleStep(10);
    ui->spinBox_5->setSingleStep(10);
    ui->spinBox_0->setRange(-360*5, +360*5);
    ui->spinBox_1->setRange(-360*5, +360*5);
    ui->spinBox_2->setRange(-360*5, +360*5);
    ui->spinBox_3->setRange(-360*5, +360*5);
    ui->spinBox_4->setRange(-360*5, +360*5);
    ui->spinBox_5->setRange(-360*5, +360*5);
    ui->spinBox_1->setValue(-90);
    ui->spinBox_3->setValue(-90);
    connect(ui->spinBox_0, SIGNAL(valueChanged(int)), robot, SLOT(robotJ0(int)));
    connect(ui->spinBox_1, SIGNAL(valueChanged(int)), robot, SLOT(robotJ1(int)));
    connect(ui->spinBox_2, SIGNAL(valueChanged(int)), robot, SLOT(robotJ2(int)));
    connect(ui->spinBox_3, SIGNAL(valueChanged(int)), robot, SLOT(robotJ3(int)));
    connect(ui->spinBox_4, SIGNAL(valueChanged(int)), robot, SLOT(robotJ4(int)));
    connect(ui->spinBox_5, SIGNAL(valueChanged(int)), robot, SLOT(robotJ5(int)));

    connect(ui->haltButton, SIGNAL(toggled(bool)), SLOT(haltButtonChanged(bool)));
    connect(ui->zeroButton, SIGNAL(clicked()), SLOT(zeroButtonClicked()));
    connect(ui->p1Button, SIGNAL(clicked()), SLOT(p1ButtonClicked()));
    connect(ui->p2Button, SIGNAL(clicked()), SLOT(p2ButtonClicked()));
    connect(ui->p3Button, SIGNAL(clicked()), SLOT(p3ButtonClicked()));
    connect(ui->p4Button, SIGNAL(clicked()), SLOT(p4ButtonClicked()));
    connect(ui->p5Button, SIGNAL(clicked()), SLOT(p5ButtonClicked()));
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(pushButtonClicked()));
    connect(ui->jontButton, SIGNAL(clicked()), SLOT(jontButtonClicked()));

    connect(robot,SIGNAL(jointPosition(FloatVector)), SLOT(jointPosition(FloatVector)));
    connect(robot,SIGNAL(receivedPosition(QVector3D, QVector3D, FloatVector)),
                    SLOT(receivedPosition(QVector3D, QVector3D, FloatVector)));
    robotView->add_robot();
    robot->updateModel();


    // Table
    active.resize(npoints);
    joints.resize(npoints);
    tcps.resize(npoints);
    // Table
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Joint"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("TCP"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Chk"));
    ui->tableWidget->setCurrentCell(0,0);
    ui->tableWidget->setRowCount(npoints);
    for (int i=0;i<npoints;i++){
        active[i] = new QCheckBox();
        ui->tableWidget->setCellWidget (i,2, active[i]);
    }

    connect(ui->savePosButton, SIGNAL(clicked()), SLOT(savePosButtonClicked()));
    connect(ui->loadPosButton, SIGNAL(clicked()), SLOT(loadPosButtonClicked()));
    connect(ui->startButton  , SIGNAL(clicked()), SLOT(startButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::haltButtonChanged(bool t)
{
    robot->haltEnabled = t;
    robot->haltStatus = false;
    qDebug() << "Real robot halt:" << robot->haltEnabled;
}

void MainWindow::zeroButtonClicked()
{
    robot->haltStatus = false;
    robot->moveToZeroPosition(100);
}

void MainWindow::p1ButtonClicked()
{
    robot->haltStatus = false;
    robot->moveTo(1, robotView->points[0]->position, 0, qDegreesToRadians(-90.0f), 0);
}

void MainWindow::p2ButtonClicked()
{
    robot->haltStatus = false;
    robot->moveTo(2, robotView->points[1]->position, 0, qDegreesToRadians(-90.0f), 0);
}

void MainWindow::p3ButtonClicked()
{
    robot->haltStatus = false;
    robot->moveTo(3, robotView->points[2]->position, 0, qDegreesToRadians(-90.0f), 0);
}

void MainWindow::p4ButtonClicked()
{
    robot->haltStatus = false;
    robot->moveTo(4, robotView->points[3]->position, 0, qDegreesToRadians(-90.0f), 0);
}

void MainWindow::p5ButtonClicked()
{
    robot->haltStatus = false;
    robot->moveTo(5, robotView->points[4]->position, 0, qDegreesToRadians(-90.0f), 0);
}

void MainWindow::pushButtonClicked()
{
    if (ui->checkBox->isChecked())
    {
            robot->haltStatus = false;
            robot->moveTo(10, robotView->points[0]->position, 0, qDegreesToRadians(-90.0f), 0);
    }
    else
       posReadyAction(10);
}

void MainWindow::posReadyAction(int id)
{
    qDebug() << "position" << id;
    if (id == 10){
        if (ui->checkBox_2->isChecked())
        {
            QThread::sleep(2);
            robot->haltStatus = false;
            robot->moveTo(11, robotView->points[1]->position, 0, qDegreesToRadians(-90.0f), 0);
        }
        else
           posReadyAction(11);
    }
    if (id == 11){
        if (ui->checkBox_3->isChecked())
        {
            QThread::sleep(2);
            robot->haltStatus = false;
            robot->moveTo(12, robotView->points[2]->position, 0, qDegreesToRadians(-90.0f), 0);
        }
        else
           posReadyAction(12);
    }
    if (id == 12){
        if (ui->checkBox_4->isChecked())
        {
            QThread::sleep(2);
            robot->haltStatus = false;
            robot->moveTo(13, robotView->points[3]->position, 0, qDegreesToRadians(-90.0f), 0);
        }
        else
           posReadyAction(13);
    }
    if (id == 13){
        if (ui->checkBox_5->isChecked())
        {
            QThread::sleep(2);
            robot->haltStatus = false;
            robot->moveTo(14, robotView->points[4]->position, 0, qDegreesToRadians(-90.0f), 0);
        }
        else
           posReadyAction(14);
    }
    if (id == 14){
        QThread::sleep(2);
        robot->haltStatus = false;
        robot->moveToZeroPosition(100);
    }

    if (id >=200){
        current++;
        qDebug() << current;
        // TODO Check valid joints item
        if (current < npoints)
            if (active[current]->isChecked()){
                robot->haltStatus = false;
                robot->setJointTo(current+200, joints[current]);
            }
            else
                posReadyAction(200+current);
    }
}

void MainWindow::startButtonClicked()
{
    for (int i=0;i<npoints;i++){
        qDebug() << joints[i];
    }
    current = 0;
    if (active[current]->isChecked()){
        robot->haltStatus = false;
        robot->setJointTo(200+current, joints[current]);
    }
    else {
        posReadyAction(200+current);
    }
    return;

    // wait
    // get position ready
    // send next position
}


void MainWindow::jontButtonClicked()
{
    robot->haltStatus = false;
    robot->setJointTo(20, -1.53915f, -1.97043f,   2.28087f,
                          -0.312216f, 0.0306979f, 1.56973f);
//    robot->setJointTo(20, -1.55019f, -1.68492f,  1.27177f,
//                           0.413331f, 0.020606f, 1.57062f);
}

void MainWindow::jointPosition(FloatVector jointAngles)
{
    qDebug() << jointAngles[0];
    ui->spinBox_0->setValue((int)qRadiansToDegrees(jointAngles[0]));
    ui->spinBox_1->setValue((int)qRadiansToDegrees(jointAngles[1]));
    ui->spinBox_2->setValue((int)qRadiansToDegrees(jointAngles[2]));
    ui->spinBox_3->setValue((int)qRadiansToDegrees(jointAngles[3]));
    ui->spinBox_4->setValue((int)qRadiansToDegrees(jointAngles[4]));
    ui->spinBox_5->setValue((int)qRadiansToDegrees(jointAngles[5]));
}

void MainWindow::haltMessage(int id)
{
    id;
    robotView->text[0]->mesh->setText("Safety Zone");
}

void MainWindow::savePosButtonClicked()
{
    int c = ui->tableWidget->selectionModel()->currentIndex().column();
    int r = ui->tableWidget->selectionModel()->currentIndex().row();
    qDebug() << c << r;
    if ((c>=0) and (r>=0))
    {
        robot->getJointPosition(r);
        savePositions();
    }
    cSlot = c;
    rSlot = r;

    // wait
    // get result
    // save result at receivedPosition slot
}

void MainWindow::receivedPosition(QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles)
{
    QString strJ;
    QString strTCP;
    QTableWidgetItem item;


    robot->updateModel();
    if (!(cSlot>=0) and !(rSlot>=0))
        return;

    qDebug() << "receivedPosition";

    joints[rSlot] = jointAngles;
    tcps[rSlot] = xyz;
    active[rSlot]->setChecked(true);
    for (int i=0; i<6;i++){
        strJ = strJ + " " + QString::number(int(qRadiansToDegrees(double(jointAngles[i]))));
    }
    for (int i=0; i<3;i++){
        strTCP = strTCP + " " + QString::number(double(xyz[i]));
    }

    item.setText(strJ);
    ui->tableWidget->setItem(rSlot,0, new QTableWidgetItem(strJ));
    ui->tableWidget->setItem(rSlot,1, new QTableWidgetItem(strTCP));
    ui->tableWidget->resizeColumnsToContents();

    int i= rSlot;
    points[i] = new TcpPoint(QString::number(i+1), rootEntity, 0.02f, QColor(QRgb(0x0000FF)),
                           xyz,  QVector3D(-0.468239f, 0.569581f, 0.69225f));

}

void MainWindow::savePositions()
{

    QString filename = "saved_positions.txt";

    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly))
        return;
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_5);
    ds << joints << tcps;
    f.close();
}

void MainWindow::loadPosButtonClicked()
{
    QVector<int> x, y;

    QString filename = "saved_positions.txt";
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return;
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_5);
    ds >> joints >> tcps;
    f.close();

    qDebug() << joints;
    qDebug() << joints.size();
    qDebug() << "TCP:" << tcps;
    qDebug() << "tcp size"<< tcps.size();

    current = 0;
    for (int i=0;i<joints.size();i++){
        QString strJ;
        QString strTCP;
        FloatVector jointAngles = joints[i];
        QVector3D xyz = tcps[i];
        if (jointAngles.size()==6){
            for (int j=0; j<6;j++){
                strJ = strJ + " " + QString::number(int(qRadiansToDegrees(double(jointAngles[j]))));
            }

            for (int i=0; i<3;i++){
                strTCP = strTCP + " " + QString::number(double(xyz[i]));
            }

            ui->tableWidget->setItem(i,0, new QTableWidgetItem(strJ));
            ui->tableWidget->setItem(i,1, new QTableWidgetItem(strTCP));
            active[i] = new QCheckBox();
            active[i]->setChecked(true);
            ui->tableWidget->setCellWidget (i,2, active[i]);
        }
        points[i] = new TcpPoint(QString::number(i+1), rootEntity, 0.02f, QColor(QRgb(0x0000FF)),
                               xyz,  QVector3D(-0.468239f, 0.569581f, 0.69225f));
    }
    ui->tableWidget->resizeColumnsToContents();

}
