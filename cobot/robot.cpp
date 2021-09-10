#include <iostream>
#include <cmath>

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QDataStream>
#include <QFile>
#include <QMap>
#include <QNetworkInterface>
#include <QDebug>
#include "robot.h"

#include <QTransform>

using namespace std;

double toAngle(double x, double y);

Robot::Robot(QObject *parent, QString robot_host, quint16 robot_port,
                              QString xmlRpcInterface, quint16 xmlRpcPort,
                              CobotZone* pCobotZone, EngineZone* pEngineZone,
                              RobotVisualisation* pRobotView):
    QObject(parent)
{
    qDebug() << "Robot NEW";
    robotView = pRobotView;

    Vertex[0]=&J0xyz;
    Vertex[1]=&J1xyz;
    Vertex[2]=&J2xyz;
    Vertex[3]=&J3xyz;
    Vertex[4]=&J4xyz;
    Vertex[5]=&J5xyz;
    Vertex[6]=&J6xyz; // TCP

    // Init SafetyZone
    cobotZone = pCobotZone;
    engineZone = pEngineZone;
    // Init network interfaces
    host = robot_host;
    port = robot_port;
    QString localIp = getLocalIp(xmlRpcInterface).ip().toString();
    rpcUrl = "http://" + localIp + ":" + QString::number(xmlRpcPort) + "/RPC2";

    // XML-RPC Server
    xmlRpcServer = new XMLRPCServerThread(parent);
//    connect(xmlRpcServer, SIGNAL(positionReady(int)), SLOT(robotPositionReady(int)));
    connect(xmlRpcServer, SIGNAL(positionReady(int)), SIGNAL(positionReady(int)));
    connect(xmlRpcServer, SIGNAL(receivedPosition(QVector3D, QVector3D, FloatVector)),
                            SLOT(robotReceivedPosition(QVector3D, QVector3D, FloatVector)));
    connect(xmlRpcServer, SIGNAL(positionCurrent(QVector3D, QVector3D, FloatVector)),
                            SLOT(robotPositionCurrent(QVector3D, QVector3D, FloatVector)));

    xmlRpcServer->start();

    // Init urScriptParameters map
    urScriptParameters["RPC_URL"]=rpcUrl;

    // Init DH parameters
    // UR10e
    a[0] = 0.0f;
    a[1] = 0.0f;
    a[2] = -0.6127f;
    a[3] = -0.57155f;
    a[4] = 0.0f;
    a[5] = 0.0f;
    a[6] = 0.0f;
    a[7] = 0.0f;

    d[0] = 0.0f;
    d[1] = 0.1807f;
    d[2] = 0.0f;
    d[3] = 0.0f;
    d[4] = 0.17415f;
    d[5] = 0.11985f;
    d[6] = 0.11655f;
    d[7] = 0.20f;         // TCP distanse

// UR10
//    a[0] = 0.0f;
//    a[1] = 0.0f;
//    a[2] = -0.612f;
//    a[3] = -0.5723f;
//    a[4] = 0.0f;
//    a[5] = 0.0f;
//    a[6] = 0.0f;
//    a[7] = 0.0f;

//    d[0] = 0.0f;
//    d[1] = 0.1273F;
//    d[2] = 0.0f;
//    d[3] = 0.0f;
//    d[4] = 0.163941f;
//    d[5] = 0.1157f;
//    d[6] = 0.0922f;
//    d[7] = 0.20f;         // TCP distanse

    af[0] = 0.0f;
    af[1] = M_PI_2f32;
    af[2] = 0.0f;
    af[3] = 0.0f;
    af[4] = M_PI_2f32;
    af[5] = -M_PI_2f32;
    af[6] = 0.0f;
    af[7] = 0.0f;

    tt[0] = 0.0f;
    tt[1] = 0.0f;
    tt[2] = 0.0f;
    tt[3] = 0.0f;
    tt[4] = 0.0f;
    tt[5] = 0.0f;
    tt[6] = 0.0f;
    tt[7] = 0.0f;

    // Get current robot position
    getJointPosition(0);
//    updateModel();
}

Robot::~Robot()
{
    qDebug() << "Robot DEL";
}

QNetworkAddressEntry Robot::getLocalIp(QString interface)
{
    QNetworkAddressEntry entry;
    QNetworkInterface eth1Ip = QNetworkInterface::interfaceFromName(interface);
    QList<QNetworkAddressEntry> entries = eth1Ip.addressEntries();
    if (!entries.isEmpty()) {
        entry = entries.first();
        qDebug() << entry.ip().toString();
    }

    return entry;
}

int Robot::sendAll(QTcpSocket *s, char *buf, int *len)
{
    int total = 0;              // how many bytes we've sent
    qint64 bytesleft = *len;    // how many we have left to send
    qint64 n = 0;
    while(total < *len) {
       n = s->write(buf, bytesleft);
       if (n == -1) {
           /* print/log error details */
           break;
       }
       total += n;
       bytesleft -= n;
    }
    *len = total;           // return number actually sent here
    return n==-1?-1:0;      // return -1 on failure, 0 on success
}

int Robot::sendProgram(char *code)
{
    // connect to Robot
    // send code
    // close connection
    qDebug() << "---------------------------";
    qDebug() << code;
    qDebug() << "---------------------------";

    int len=0;
    len = static_cast<int>(strlen(code));
    QObject::connect(&urScriptSocket, SIGNAL(readyRead()), SLOT(urScriptSocketReadyRead()));

    host = "10.0.13.200";
    port = 30002;
    qDebug() << "Connecting...";
    urScriptSocket.connectToHost(host, port);
    if( urScriptSocket.waitForConnected() ) {
        qDebug() << "Connected";
        sendAll(&urScriptSocket, code, &len);
    }
    urScriptSocket.close();
    qDebug() << "Closed";
    return 0;
}

int Robot::sendUrpScript(QString fileName)
{
    // Load URScript code from file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Cannot read file ";
        return -1;
    }
    QTextStream in(&file);
    QString qcode = in.readAll();
    QByteArray ba = qcode.toLocal8Bit();
    char *code = ba.data();

    sendProgram(code);

    return 0;
}

void Robot::urScriptSocketReadyRead()
{
    // todo add read for future functional
}

int Robot::moveTo(int id, QVector3D xyz,
                  float rx,float ry,float rz)
{
    float x = xyz.x();
    float y = xyz.y();
    float z = xyz.z();
    return moveTo(id, x,y,z,rx,ry,rz);
}

int Robot::moveTo(int id, float x, float y, float z,
                          float rx,float ry,float rz)
{
    qDebug() << "go to" << id << "...";
    currentPositionId = id;

    // Load URScript code from file
    QString fileName = "../cobot/move_to.urp";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Cannot read file ";
        return -1;
    }
    QTextStream in(&file);
    QString qCode = in.readAll();

    // Set script parameters
    urScriptParameters["RPC_URL"]=rpcUrl;
    urScriptParameters["X"].setNum(x);
    urScriptParameters["Y"].setNum(y);
    urScriptParameters["Z"].setNum(z);
    urScriptParameters["RX"].setNum(rx);
    urScriptParameters["RY"].setNum(ry);
    urScriptParameters["RZ"].setNum(rz);
    urScriptParameters["ID"].setNum(id);

    // Replacement
    UrScriptParameters::const_iterator i = urScriptParameters.constBegin();
    while (i != urScriptParameters.constEnd()) {
        qCode.replace("<<"+i.key()+">>", i.value());
        ++i;
    }

    // Convert to char* and send to robot
    QByteArray ba = qCode.toLocal8Bit();
    char *code = ba.data();
    sendProgram(code);

    return 0;
}

int Robot::setJointTo(int id, FloatVector jointAngles)
{
    float tj0 = jointAngles[0];
    float tj1 = jointAngles[1];
    float tj2 = jointAngles[2];
    float tj3 = jointAngles[3];
    float tj4 = jointAngles[4];
    float tj5 = jointAngles[5];
    return setJointTo(id, tj0,tj1,tj2,tj3,tj4,tj5);
}

int Robot::setJointTo(int id, float j0, float j1, float j2,
                              float j3, float j4, float j5)
{
    qDebug() << "go to" << id << "...";
    currentPositionId = id;

    // Load URScript code from file
    QString fileName = "../cobot/set_joint_to.urp";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Cannot read file ";
        return -1;
    }
    QTextStream in(&file);
    QString qCode = in.readAll();

    // Set script parameters
    urScriptParameters["RPC_URL"]=rpcUrl;
    urScriptParameters["J0"].setNum(j0);
    urScriptParameters["J1"].setNum(j1);
    urScriptParameters["J2"].setNum(j2);
    urScriptParameters["J3"].setNum(j3);
    urScriptParameters["J4"].setNum(j4);
    urScriptParameters["J5"].setNum(j5);
    urScriptParameters["ID"].setNum(id);

    // Replacement
    UrScriptParameters::const_iterator i = urScriptParameters.constBegin();
    while (i != urScriptParameters.constEnd()) {
        qCode.replace("<<"+i.key()+">>", i.value());
        ++i;
    }

    // Convert to char* and send to robot
    QByteArray ba = qCode.toLocal8Bit();
    char *code = ba.data();
    sendProgram(code);
    return 0;
}

int Robot::getJointPosition(int id)
{
    currentPositionId = id;

    // Load URScript code from file
    QString fileName = "../cobot/get_position.urp";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Cannot read file ";
//        return -1;
    }
    QTextStream in(&file);
    QString qCode = in.readAll();

    // Set script parameters
    urScriptParameters["RPC_URL"]=rpcUrl;
    urScriptParameters["ID"].setNum(id);

    // Replacement
    UrScriptParameters::const_iterator i = urScriptParameters.constBegin();
    while (i != urScriptParameters.constEnd()) {
        qCode.replace("<<"+i.key()+">>", i.value());
        ++i;
    }

    // Convert to char* and send to robot
    QByteArray ba = qCode.toLocal8Bit();
    char *code = ba.data();
    sendProgram(code);
}

void Robot::moveToZeroPosition(int id)
{
    currentPositionId = id;

    // Load URScript code from file
    QString fileName = "../cobot/zero_position.urp";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Cannot read file ";
//        return -1;
    }
    QTextStream in(&file);
    QString qCode = in.readAll();

    // Set script parameters
    urScriptParameters["RPC_URL"]=rpcUrl;
    urScriptParameters["ID"].setNum(id);

    // Replacement
    UrScriptParameters::const_iterator i = urScriptParameters.constBegin();
    while (i != urScriptParameters.constEnd()) {
        qCode.replace("<<"+i.key()+">>", i.value());
        ++i;
    }

    // Convert to char* and send to robot
    QByteArray ba = qCode.toLocal8Bit();
    char *code = ba.data();
    sendProgram(code);
}


//void Robot::robotPositionReady(int position_id)
//{
//    cout << "slot robotPositionReady " << position_id << endl;
//    emit positionReady(position_id);
//}


void Robot::robotJ0(int j) { j0 = qDegreesToRadians(float(j)); updateModel();}
void Robot::robotJ1(int j) { j1 = qDegreesToRadians(float(j)); updateModel();}
void Robot::robotJ2(int j) { j2 = qDegreesToRadians(float(j)); updateModel();}
void Robot::robotJ3(int j) { j3 = qDegreesToRadians(float(j)); updateModel();}
void Robot::robotJ4(int j) { j4 = qDegreesToRadians(float(j)); updateModel();}
void Robot::robotJ5(int j) { j5 = qDegreesToRadians(float(j)); updateModel();}

void Robot::robotPositionCurrent(QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles)
{
    qDebug() << "robotPositionCurrent";
    j0 = jointAngles[0];
    j1 = jointAngles[1];
    j2 = jointAngles[2];
    j3 = jointAngles[3];
    j4 = jointAngles[4];
    j5 = jointAngles[5];

    tcp_xyz = xyz;
    tcp_rxryrz = rxryrz;

    emit jointPosition(jointAngles);

    updateModel();
}

void Robot::robotReceivedPosition(QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles)
{
    qDebug() << "robotReceivedPosition";
    j0 = jointAngles[0];
    j1 = jointAngles[1];
    j2 = jointAngles[2];
    j3 = jointAngles[3];
    j4 = jointAngles[4];
    j5 = jointAngles[5];

    tcp_xyz = xyz;
    tcp_rxryrz = rxryrz;

    emit receivedPosition(xyz,rxryrz, jointAngles);

    updateModel();
}

void Robot::updateModel()
{
    qDebug() << "Update Model";
    // angles transformation from UR coordinats to polar orthonormal right-handed system. (ISO 31-11)
    float j0 = this->j0 + qDegreesToRadians(270.0f);
    float j1 = -1 * (-1 * this->j1 - qDegreesToRadians(90.0f));
    float j2 = this->j2;
    float j3 = this->j3 + qDegreesToRadians(90.0f);
    float j4 = this->j4 + qDegreesToRadians(90.0f);

    // Joints
    float j0x = 0;
    float j0y = 0;
    float j0z = 0;

    float j1x = L0h*cos(j0);
    float j1y = L0h*sin(j0);
    float j1z = L0v + j0z;

    j0 = j0 - M_PI_2f32;
    float j2x = j1x + L1v*sin(j1)*cos(j0);
    float j2y = j1y + L1v*sin(j1)*sin(j0);
    float j2z = j1z + L1v*cos(j1);

    float j3x = j2x + L2v*sin(j1+j2)*cos(j0) + L2h*cos(j0-M_PI_2f32);
    float j3y = j2y + L2v*sin(j1+j2)*sin(j0) + L2h*sin(j0-M_PI_2f32);
    float j3z = j2z + L2v*cos(j1+j2);

    float j4x = j3x + L2h*sin(j3+j2+j1)*cos(j0) - L2h*cos(j0-M_PI_2f32);
    float j4y = j3y + L2h*sin(j3+j2+j1)*sin(j0) - L2h*sin(j0-M_PI_2f32);
    float j4z = j3z + L2h*cos(j3+j2+j1);
//------ J5 -----
//    DH model
    tt[1]=this->j0;
    tt[2]=this->j1;
    tt[3]=this->j2;
    tt[4]=this->j3;
    tt[5]=this->j4;
    tt[6]=this->j5;

    QMatrix4x4 A[NJOINTS+2];
    QMatrix4x4 T[NJOINTS+2];
    for (int i=1; i<NJOINTS+2; i++)
    {
        QMatrix4x4 A0(cos(tt[i]), -cos(af[i])*sin(tt[i]),  sin(af[i])*sin(tt[i]), a[i]*cos(tt[i]),
                      sin(tt[i]),  cos(af[i])*cos(tt[i]), -sin(af[i])*cos(tt[i]), a[i]*sin(tt[i]),
                               0,  sin(af[i]),             cos(af[i]),            d[i],
                      0,0,0,1);
        A[i]= A0;
        T[i] = T[i-1] * A[i];
    }

    // Result
//    J0xyz = QVector3D(T[1].column(3).x(), T[0].column(3).y(), T[0].column(3).z());
//    J1xyz = QVector3D(T[2].column(3).x(), T[1].column(3).y(), T[1].column(3).z());
//    J2xyz = QVector3D(T[3].column(3).x(), T[2].column(3).y(), T[2].column(3).z());
//    J3xyz = QVector3D(T[4].column(3).x(), T[3].column(3).y(), T[3].column(3).z());
//    J4xyz = QVector3D(T[5].column(3).x(), T[4].column(3).y(), T[4].column(3).z());
    J5xyz = QVector3D(T[6].column(3).x(), T[6].column(3).y(), T[6].column(3).z());
    J6xyz = QVector3D(T[7].column(3).x(), T[7].column(3).y(), T[7].column(3).z());

    qDebug() << J5xyz;
//---------------------
    //Elbows
    float e1x = j0x;
    float e1y = j0y;
    float e1z = j0z + L0h;

    float e2x = j2x + L1h*cos(j0 - M_PI_2f32);
    float e2y = j2y + L1h*sin(j0 - M_PI_2f32);
    float e2z = j2z;

    float e3x = j3x + L2h*cos(j0 + M_PI_2f32);
    float e3y = j3y + L2h*sin(j0 + M_PI_2f32);
    float e3z = j3z;

    // Result
    J0xyz = QVector3D(j0x,j0y,j0z);
    J1xyz = QVector3D(j1x,j1y,j1z);
    J2xyz = QVector3D(j2x,j2y,j2z);
    J3xyz = QVector3D(j3x,j3y,j3z);
    J4xyz = QVector3D(j4x,j4y,j4z);
//    J5xyz = QVector3D(j5x,j5y,j5z);
//    J6xyz = QVector3D(j6x,j6y,j6z);

    E1 = QVector3D(e1x,e1y,e1z);
    E2 = QVector3D(e2x,e2y,e2z);
    E3 = QVector3D(e3x,e3y,e3z);
//    E5 = QVector3D(e5x,e5y,e5z);

    // Check safety zone
    if (!isSafety())
    {
        qDebug() << "HALT!!!!" ;
    }



    //Segment Body
    /// S0 ///
    QVector3D S0 = J0xyz + (E1-J0xyz)/2;
    float s00 = float(M_PI_2);
    float t1[] = {1,        0,         0,
                  0, cos(s00), -sin(s00),
                  0, sin(s00),  cos(s00)};
    QMatrix3x3 S0XR(t1);
    QMatrix3x3 S0R=S0XR;

    /// S1 ///
    QVector3D S1 = J1xyz + (E1-J1xyz)/2;
    float s1j0 = j0;
    float t2[] = {cos(s1j0), -sin(s1j0), 0,
                  sin(s1j0),  cos(s1j0), 0,
                          0,          0, 1};
    QMatrix3x3 S1ZR(t2);
    QMatrix3x3 S1R = S1ZR;

    /// S2 ///
    QVector3D S2 = J1xyz + (J2xyz-J1xyz)/2;
    float s2j0 = j0 + float(M_PI_2);
    float s2j1 = j1 + float(M_PI_2);
    float v1[] = {cos(s2j0), -sin(s2j0), 0,
                  sin(s2j0),  cos(s2j0), 0,
                          0,          0, 1};
    QMatrix3x3 S2ZR(v1);
    float v2[] = {cos(s2j1), 0, sin(s2j1),
                          0, 1,         0,
                 -sin(s2j1), 0, cos(s2j1)};
    QMatrix3x3 S2YR(v2);
    float v3[] = {1,         0,           0,
                  0, cos(s2j1), -sin(s2j1),
                  0, sin(s2j1),  cos(s2j1)};
    QMatrix3x3 S2XR(v3);
    QMatrix3x3 S2R = S2ZR*S2XR;

    /// S3 ///
    QVector3D  S3 = J2xyz + (E2-J2xyz)/2;
    QMatrix3x3 S3R = S1R;

    /// S4 ///
    QVector3D S4 = E2 + (J3xyz-E2)/2;
    float s4j0 = j0 + float(M_PI_2);
    float s4j1 = j1 + j2 + float(M_PI_2);
    float v4[] = {cos(s4j0), - sin(s4j0), 0,
                  sin(s4j0),   cos(s4j0), 0,
                  0,          0,        1};
    QMatrix3x3 S4ZR(v4);
    float v6[] = {1, 0,          0,
                  0, cos(s4j1), - sin(s4j1),
                  0, sin(s4j1),   cos(s4j1)};
    QMatrix3x3 S4XR(v6);
    QMatrix3x3 S4R = S4ZR*S4XR;

    /// S5 ///
    QVector3D  S5 = J3xyz + (E3-J3xyz)/2;
    QMatrix3x3 S5R = S1R;

    /// S6 ///
    QVector3D S6 = E3 + (J4xyz-E3)/2;
    float s6j0 = j0 + float(M_PI_2);
    float s6j1 = j1 + j2 + j3 + float(M_PI_2);
    float v7[] = {cos(s6j0), - sin(s6j0), 0,
                  sin(s6j0),   cos(s6j0), 0,
                  0,          0,        1};
    QMatrix3x3 S6ZR(v7);
    float v8[] = {1, 0,          0,
                  0, cos(s6j1), - sin(s6j1),
                  0, sin(s6j1),   cos(s6j1)};
    QMatrix3x3 S6XR(v8);
    QMatrix3x3 S6R = S6ZR*S6XR;

    /// S7 ///
    /// ERRORS!!!!
    QVector3D S7 = J4xyz + (J5xyz-J4xyz)/2;
    float s7j0 = j0+j4+float(M_PI_2);
    float s7j1 = j1 + j2 + j3;
    float v9[] = {cos(s7j0), - sin(s7j0), 0,
                  sin(s7j0),   cos(s7j0), 0,
                  0,          0,        1};
    QMatrix3x3 S7ZR(v9);
    float v10[] = {1, 0,          0,
                  0, cos(s7j1), - sin(s7j1),
                  0, sin(s7j1),   cos(s7j1)};
    QMatrix3x3 S7XR(v10);
    QMatrix3x3 S7R = S7ZR*S7XR;
    QVector3D V = J5xyz-J4xyz;

    // Visualisation
    if (robotView!=nullptr)
    {
        robotView->joints[0]->transform->setTranslation(J0xyz);
        robotView->joints[1]->transform->setTranslation(J1xyz);
        robotView->joints[2]->transform->setTranslation(J2xyz);
        robotView->joints[3]->transform->setTranslation(J3xyz);
        robotView->joints[4]->transform->setTranslation(J4xyz);
        robotView->joints[5]->transform->setTranslation(J5xyz);
        robotView->joints[6]->transform->setTranslation(J6xyz);

        robotView->elbows[1]->transform->setTranslation(E1);
        robotView->elbows[2]->transform->setTranslation(E2);
        robotView->elbows[3]->transform->setTranslation(E3);
    //    elbows[3]->transform->setTranslation(E5);

        robotView->segments[0]->transform->setRotation(QQuaternion::fromRotationMatrix(S0R));
        robotView->segments[0]->transform->setTranslation(S0);
        robotView->segments[1]->transform->setRotation(QQuaternion::fromRotationMatrix(S1R));
        robotView->segments[1]->transform->setTranslation(S1);
        robotView->segments[2]->transform->setRotation(QQuaternion::fromRotationMatrix(S2R));
        robotView->segments[2]->transform->setTranslation(S2);
        robotView->segments[3]->transform->setRotation(QQuaternion::fromRotationMatrix(S3R));
        robotView->segments[3]->transform->setTranslation(S3);
        robotView->segments[4]->transform->setRotation(QQuaternion::fromRotationMatrix(S4R));
        robotView->segments[4]->transform->setTranslation(S4);
        robotView->segments[5]->transform->setRotation(QQuaternion::fromRotationMatrix(S5R));
        robotView->segments[5]->transform->setTranslation(S5);
        robotView->segments[6]->transform->setRotation(QQuaternion::fromRotationMatrix(S6R));
        robotView->segments[6]->transform->setTranslation(S6);
    //    segments[7]->transform->setRotation(QQuaternion::fromRotationMatrix(S7R));
    //    segments[7]->transform->setTranslation(S7);
    }
}

double toAngle(double x, double y)
{
    //        arctg(y0/x0), if x0 > 0, y0 >=0,
    //        arctg(y0/x0) + 180, if x0 < 0, y0 - ...,
    //        arctg(y0/x0) + 360, if x0 > 0, y0 < 0
    //        90, if x0 = 0. y > 0
    //        270, if x0 = 0, y < 0
    //        Undefined, if x0 = 0, y0 = 0.
    double a;
    if ((x>0) and (y>0)) return a = qAtan(y/x);
    if ((x<0)          ) return a = qAtan(y/x) + M_PI;
    if ((x>0) and (y<0)) return a = qAtan(y/x) + 2*M_PI;

    if ((x==0.0) and (y>0)) return a = M_PI_2;
    if ((x==0.0) and (y<0)) return a = 3*M_PI_2;
    if ((x==0.0) and (y==0.0)) return a = 0;
    return a = 20* M_PI;
};

//bool Robot::isSafety()
//{
//    // ОК. но не понятно как визуализировать плоскость по 3-м точкам (QVector3D, QVector3D, QVector3D)
//    QVector3D plane1 = QVector3D(1,0,0);
//    QVector3D plane2 = QVector3D(0,1,0);
//    QVector3D plane3 = QVector3D(0,0,0);
//    d = Vertex[0]->distanceToPlane(plane1, plane2, plane3);

//    // ОК. но не понятно как визуализировать плоскость по точке и вектору (QVector3D, QVector3D)
//    QVector3D plane = QVector3D(0,0,0);
//    QVector3D normal= QVector3D(0,0,1);
//    d = Vertex[0]->distanceToPlane(plane, normal);

//    QVector3D* robotPoint;
//    QVector3D* zonePlane;
//    QVector3D* zoneNormal;

//    for (int j = 1; j < NJOINTS; j++){
//        robotPoint = Vertex[j];
//        for (int i=0; i<cobotZone->planesNumber; i++){
//            zonePlane = &cobotZone->planes[i]->plane;
//            zoneNormal= &cobotZone->planes[i]->normal;
//            distance = robotPoint->distanceToPlane(*zonePlane, *zoneNormal);
//            if ( distance > haltDistance){
//                ret = true;
//                qDebug() << i << j << "z=" << robotPoint->z() << "distance" << distance << "OK";
//            }
//            else{
//                halt();
//                ret = false;
//                qDebug() << i << j << "z=" << robotPoint->z() << "distance" << distance << "HALT!!!";
//            }
//        }
//    }
//    return ret;

bool Robot::isSafety()
{
    QVector3D* robotPoint;
    bool pointOutsideZone=true;
    bool segmentInZone=true;
    bool safety = true;
    for (int j = 1; j < NJOINTS; j++){
        robotPoint = Vertex[j];
        pointOutsideZone = cobotZone->isPointOutsideZone(robotPoint, haltDistance);
        if ( pointOutsideZone ){
            robotView->text[0]->mesh->setText("out of Cobot Zone");
            robotView->text[0]->material->setDiffuse(0xFF0000);
            halt();
            safety = false;
            break;
        }
    }

//  TCP only
//    pointOutsideZone = engineZone->isPointInsideZone(robotPoint, haltDistance);
//    segmentInZone = engineZone->isSegmentInZone(Vertex[5], Vertex[6], haltDistance);
//    if ( segmentInZone ){
//        halt();
//        safety = false;
//    }

    for (int j = 1; j <= NJOINTS; j++){
        segmentInZone = engineZone->isSegmentInZone(Vertex[j-1], Vertex[j], haltDistance);
        if ( segmentInZone ){
            if (robotView!=nullptr){
                robotView->text[0]->mesh->setText("inside Engine Zone");
                robotView->text[0]->material->setDiffuse(0xFF0000);
            }
            halt();
            safety = false;
            break;
        }
    }

    if (safety){
        if (robotView!=nullptr){
            robotView->text[0]->mesh->setText("ok");
            robotView->text[0]->material->setDiffuse(0x00FF00);
        }
    }

//    test
//    QVector3D J0 = QVector3D(-0.5f, -0.5f, 0.5f);
//    QVector3D J1 = QVector3D(-0.5f, 0.5f, 1.11f);
//    haltDistance = 0.1f;
//    pointOutsideZone = engineZone->isSegmentInZone(&J0, &J1, haltDistance);

    return safety;
}

void Robot::halt()
{
    if (not haltEnabled){
        qDebug() << "Warning! HALT! but ...continue...";
        return;
    }
    if (!haltStatus){
        qDebug() << "Send HALT script!";
        sendUrpScript("../cobot/halt.urp");
        haltStatus = true;
        emit halted(currentPositionId);
    }
//    def program():
//        halt
//    end
//    program()
}

// Same
//    QQuaternion rot = QQuaternion::fromEulerAngles(0,190,180);
//    QVector3D euler(0,190,180);
//                rot = QQuaternion::fromEulerAngles(euler);
//    segments[0]->transform->setRotation(rot);

/*
    QVector3D S1 = J1xyz + (J2xyz-J1xyz)/2;
    joints[7]->transform->setTranslation(S1);

//    qRadiansToDegrees();
//    QQuaternion rot = QQuaternion::rotationTo(J1xyz, J2xyz);
//    segments[0]->transform->setRotationZ(qRadiansToDegrees(j0)+90.0f);
//    segments[0]->transform->setRotationX(90);
//    segments[0]->transform->setRotationY(90);
//    segments[0]->transform->setRotationZ(90);

//    segments[0]->transform->setRotationZ(qRadiansToDegrees(j0)+90.0f);
//    segments[0]->transform->setRotationX(-qRadiansToDegrees(j1)+90.0f);

//    QQuaternion rot = QQuaternion::fromEulerAngles(j0, j1, j3);
//    QQuaternion rot = QQuaternion::fromDirection(J0xyz, J2xyz);
//    QQuaternion rot = QQuaternion::rotationTo(J0xyz, J2xyz);
//    segments[0]->transform->setRotation(rot);

    float x,y,z;
    QVector3D J2J1 = J2xyz-J1xyz;

    x = J2J1.x();
    y = J2J1.y();
    z = J2J1.z();
    if (z < 0) { }
    float xa = toAngle(y,z);
    float ya = toAngle(x,z);
    float za = toAngle(y,x);

//    QMatrix3x3 rot3x3;
//    QQuaternion rot = QQuaternion::fromRotationMatrix(rot3x3);
//    rot = QQuaternion::fromEulerAngles(0,
//                                       qRadiansToDegrees(this->j1),
//                                       qRadiansToDegrees(this->j0));
//    segments[0]->transform->setRotation(rot);


//    segments[0]->transform->setRotationX(90);
//    segments[0]->transform->setRotationY(qRadiansToDegrees(this->j1));
//    segments[0]->transform->setRotationZ(qRadiansToDegrees(this->j0));
*/
