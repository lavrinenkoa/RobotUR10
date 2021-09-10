#ifndef ROBOT_H
#define ROBOT_H


#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QMap>
#include <QNetworkInterface>

#include "xmlrpcserverthread.h"
#include "joint.h"
#include "plane.h"
#include "common.h"
#include "cobotzone.h"
#include "enginezone.h"
#include "robotvisualisation.h"

typedef QMap <QString, QString> UrScriptParameters;

class Robot: public QObject
{
    Q_OBJECT
public:
    explicit Robot(QObject *parent, QString robot_host, quint16 robot_port,
                                    QString xmlRpcInterface, quint16 xmlRpcPort,
                                    CobotZone* pCobotZone, EngineZone* pEngineZone,
                                    RobotVisualisation* robotView);
    ~Robot();

    // View model
    RobotVisualisation* robotView;
//    Joint* joints[NJOINTS+2];
//    Joint* elbows[NELBOW];
//    Cyl* segments[9];

    // Dangerous zone
    bool haltEnabled=true;
    bool haltStatus=false;
    float haltDistance=0.1f;
    CobotZone* cobotZone;
    EngineZone* engineZone;
    // Engine zone

    // Model phisical
    // TCP coordinats
    QVector3D tcp_xyz;
    QVector3D tcp_rxryrz;

    // Joints angles
    float j0=0,
          j1=-M_PI_2f32,
          j2=0,
          j3=-M_PI_2f32,
          j4=0,
          j5=0;

    // DH parameters
    float a[NJOINTS+2];
    float d[NJOINTS+2];
    float af[NJOINTS+2];
    float tt[NJOINTS+2];

    // Joints coordinats and Elbow coordinats
    QVector3D J0xyz = QVector3D(0,0,0);
    QVector3D J1xyz = QVector3D(0,0,0);
    QVector3D J2xyz = QVector3D(0,0,0);
    QVector3D J3xyz = QVector3D(0,0,0);
    QVector3D J4xyz = QVector3D(0,0,0);
    QVector3D J5xyz = QVector3D(0,0,0);
    QVector3D J6xyz = QVector3D(0,0,0);

    QVector3D *Vertex[NJOINTS+2];
    // Segemt central point coordinats
//    QVector3D L0;

    // Elbow cetral point coordinats
    QVector3D E1;
    QVector3D E2;
    QVector3D E3;
    QVector3D E5;

    // Segments size
    float v = 0.5f;
    float h = 0.2f;
    float L0h = 0.176f;
    float L0v = 0.181f;
    float L1h = 0.137f;
    float L1v = 0.613f;
    float L2h = 0.135f;
    float L2v = 0.571f;
    float L3h = 0.117f;
    float L3v = 0.120f;

    float S0d = 0.143f;
    float S1d = 0.139f;
    float S2d = 0.108f;
    float S3d = 0.100f;
    float S4d = 0.080f;
    float S5d = 0.080f;
    float S6d = 0.080f;
    float S7d = 0.080f;

    int currentPositionId=0;

    void updateModel();
//    void updateModel_ok();
    bool isSafety();

    // Control
    int sendProgram(char *code);
    int sendUrpScript(QString fileName);
    int moveTo(int id, QVector3D xyz,
                      float rx,float ry,float rz);
    int moveTo(int id, float x, float y, float z,
                       float rx,float ry,float rz);
    void moveToZeroPosition(int id);
    int setJointTo(int id, FloatVector jointAngles);
    int setJointTo(int id, float j0, float j1, float j2,
                           float j3, float j4, float j5);
    int getJointPosition(int id);

    void halt();

public slots:
//    void robotPositionReady(int position_id);
    void robotPositionCurrent(QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles);
    void robotReceivedPosition(QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles);
    void robotJ0(int j0);
    void robotJ1(int j1);
    void robotJ2(int j2);
    void robotJ3(int j3);
    void robotJ4(int j4);
    void robotJ5(int j5);


signals:
    void receivedPosition(QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles);
    void positionReady(int position_id);
    void jointPosition(FloatVector jointAngles);
    void halted(int position_id);

private:
    XMLRPCServerThread *xmlRpcServer;
    QString host;
    quint16 port = 30002;
    UrScriptParameters urScriptParameters;
    QString rpcUrl;
    QTcpSocket urScriptSocket;

    int sendAll(QTcpSocket *s, char *buf, int *len);
    QNetworkAddressEntry getLocalIp(QString interface);

private slots:
    void urScriptSocketReadyRead();

};

#endif // ROBOT_H
