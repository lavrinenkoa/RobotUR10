#include "xmlrpcserverthread.h"

#include <QMutex>
#include <QObject>
#include <QtMath>
#include <QVector3D>

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <iostream>
#include <iomanip>
#include <unistd.h>

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>
#include <xmlrpc-c/base.hpp>

#include "mainwindow.h"
#include "robot.h"

QMatrix4x4 m_matrix;
QMutex m_mutex;

using namespace std;

class sampleAddMethod : public xmlrpc_c::method
{
public:
    sampleAddMethod() {
        // signature and help strings are documentation -- the client
        // can query this information with a system.methodSignature and
        // system.methodHelp RPC.
        this->_signature = "i:ii";
            // method's result and two arguments are integers
        this->_help = "This method adds two integers together";
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {

        int const addend(paramList.getInt(0));
        int const adder(paramList.getInt(1));
        qDebug() << addend << "+" << adder;
        paramList.verifyEnd(2);

        *retvalP = xmlrpc_c::value_int(addend + adder);
    }
};

class positionMethod : public xmlrpc_c::method
{
public:
    positionMethod() {
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        //{'rx': 1.88422460105229e-14, 'ry': 1.57, 'rz': 1.56988430853459e-15, 'x': 0.4000000000000159, 'y': -0.4999999924365794, 'z': 0.8000000000000914}
        //typedef std::map<std::string, xmlrpc_c::value> cstruct;

        xmlrpc_c::cstruct position = paramList.getStruct(0);
//        xmlrpc_c::cstruct joint = paramList.getStruct(1);
//        xmlrpc_c::value rx = position["rx"];

        double x = xmlrpc_c::value_double(position["x"]);
        double y = xmlrpc_c::value_double(position["y"]);
        double z = xmlrpc_c::value_double(position["z"]);
        double rx = xmlrpc_c::value_double(position["rx"]);
        double ry = xmlrpc_c::value_double(position["ry"]);
        double rz = xmlrpc_c::value_double(position["rz"]);
        qDebug() << "XYZ" << x << " " << y << " " << z << " " << rx << " " << ry << " " << rz;

        // Move
        m_mutex.lock();
        m_sphereTransform->setTranslation(QVector3D(x, y, z));
        m_mutex.unlock();

        *retvalP = xmlrpc_c::value_int(0);
    }
};

class jointMethod : public xmlrpc_c::method
{
public:
    jointMethod() {
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        xmlrpc_c::carray joint = paramList.getArray(0);
//        xmlrpc_c::cstruct joint = paramList.getStruct(1);
//        xmlrpc_c::value rx = position["rx"];

        double j0 = xmlrpc_c::value_double(joint[0]);
        double j1 = xmlrpc_c::value_double(joint[1]);
        double j2 = xmlrpc_c::value_double(joint[2]);
        double j3 = xmlrpc_c::value_double(joint[3]);
        double j4 = xmlrpc_c::value_double(joint[4]);
        double j5 = xmlrpc_c::value_double(joint[5]);

        double dj0 = qRadiansToDegrees(j0);
        double dj1 = qRadiansToDegrees(j1);
        double dj2 = qRadiansToDegrees(j2);
        double dj3 = qRadiansToDegrees(j3);
        double dj4 = qRadiansToDegrees(j4);
        double dj5 = qRadiansToDegrees(j5);
//        qDebug() << j0 << " " << j1 << " " << j2 << " " << j3 << " " << j4 << " " << j5;
        qDebug() << "J  " << dj0 << " " << dj1 << " " << dj2 << " " << dj3 << " " << dj4 << " " << dj5;

        *retvalP = xmlrpc_c::value_int(0);
    }
};

class modelMethod : public xmlrpc_c::method
{
public:
    XMLRPCServerThread *sig_parent;
    modelMethod(XMLRPCServerThread *parent) {
        sig_parent = parent;
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        xmlrpc_c::carray item = paramList.getArray(0);

        double x = xmlrpc_c::value_double(item[0]);
        double y = xmlrpc_c::value_double(item[1]);
        double z = xmlrpc_c::value_double(item[2]);
        double rx = xmlrpc_c::value_double(item[3]);
        double ry = xmlrpc_c::value_double(item[4]);
        double rz = xmlrpc_c::value_double(item[5]);

        double j0 = xmlrpc_c::value_double(item[6]);
        double j1 = xmlrpc_c::value_double(item[7]);
        double j2 = xmlrpc_c::value_double(item[8]);
        double j3 = xmlrpc_c::value_double(item[9]);
        double j4 = xmlrpc_c::value_double(item[10]);
        double j5 = xmlrpc_c::value_double(item[11]);

        double dj0 = qRadiansToDegrees(j0);
        double dj1 = qRadiansToDegrees(j1);
        double dj2 = qRadiansToDegrees(j2);
        double dj3 = qRadiansToDegrees(j3);
        double dj4 = qRadiansToDegrees(j4);
        double dj5 = qRadiansToDegrees(j5);
        qDebug() << "XYZ:" << x << " " << y << " " << z << " " << rx << " " << ry << " " << rz << "   J:" << j0 << " " << j1 << " " << j2 << " " << j3 << " " << j4 << " " << j5;

        *retvalP = xmlrpc_c::value_int(0);

        // Send robot status
        FloatVector jointAngles(NJOINTS);
        for (int i = 0; i < NJOINTS; ++i) {
            jointAngles[i] =  xmlrpc_c::value_double(item[i+6]);
        }
        sig_parent->emitSignalPositionCurrent(QVector3D(x, y, z),
                                              QVector3D(rx, ry, rz),
                                              jointAngles);
    }
};


void XMLRPCServerThread::emitSignalPositionCurrent(QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles)
{
//    qDebug() << "XML emitSignalPositionCurrent";
    emit positionCurrent(xyz, rxryrz, jointAngles);
}


//---------------------
class getPositionMethod : public xmlrpc_c::method
{
public:
    XMLRPCServerThread *sig_parent;
    getPositionMethod(XMLRPCServerThread *parent) {
        sig_parent = parent;
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        xmlrpc_c::carray item = paramList.getArray(0);

        double x = xmlrpc_c::value_double(item[0]);
        double y = xmlrpc_c::value_double(item[1]);
        double z = xmlrpc_c::value_double(item[2]);
        double rx = xmlrpc_c::value_double(item[3]);
        double ry = xmlrpc_c::value_double(item[4]);
        double rz = xmlrpc_c::value_double(item[5]);

        double j0 = xmlrpc_c::value_double(item[6]);
        double j1 = xmlrpc_c::value_double(item[7]);
        double j2 = xmlrpc_c::value_double(item[8]);
        double j3 = xmlrpc_c::value_double(item[9]);
        double j4 = xmlrpc_c::value_double(item[10]);
        double j5 = xmlrpc_c::value_double(item[11]);

        qDebug() << "XYZ:" << x << " " << y << " " << z << " " << rx << " " << ry << " " << rz << "   J:" << j0 << " " << j1 << " " << j2 << " " << j3 << " " << j4 << " " << j5;

        *retvalP = xmlrpc_c::value_int(0);

        // Send robot status
        FloatVector jointAngles(NJOINTS);
        for (int i = 0; i < NJOINTS; ++i) {
            jointAngles[i] =  xmlrpc_c::value_double(item[i+6]);
        }
        sig_parent->emitSignalGetPosition(QVector3D(x, y, z),
                                          QVector3D(rx, ry, rz),
                                          jointAngles);
    }
};


void XMLRPCServerThread::emitSignalGetPosition(QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles)
{
//    qDebug() << "XML emitSignalGetPosition";
    emit receivedPosition(xyz, rxryrz, jointAngles);
}
//---------------------

class positionReadyMethod : public xmlrpc_c::method
{
public:
    XMLRPCServerThread *sig_parent;
    positionReadyMethod(XMLRPCServerThread *parent) {
        // signature and help strings are documentation -- the client
        // can query this information with a system.methodSignature and
        // system.methodHelp RPC.
        this->_signature = "i:";
        // method's result one arguments is integer
        this->_help = "This method adds one integer";
        sig_parent = parent;
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {

        int const position_id(paramList.getInt(0));
        qDebug() << "position_id = " << position_id;
//        cout << "sig = " << sig << endl;
        paramList.verifyEnd(1);

        sig_parent->emitSignalPositionReady(position_id);

        *retvalP = xmlrpc_c::value_int(0);
    }
};

XMLRPCServerThread::XMLRPCServerThread(QObject *parent)
{
}

void XMLRPCServerThread::emitSignalPositionReady(int position_id)
{
    qDebug() << "XML emitSignalPositionReady";
    emit positionReady(position_id);
}

void XMLRPCServerThread::run()
{
    try
    {
        xmlrpc_c::registry myRegistry;

        xmlrpc_c::methodPtr const sampleAddMethodP(new sampleAddMethod);
        xmlrpc_c::methodPtr const positionP(new positionMethod);
        xmlrpc_c::methodPtr const jointP(new jointMethod);
        xmlrpc_c::methodPtr const modelP(new modelMethod(this));
        xmlrpc_c::methodPtr const positionReadyP(new positionReadyMethod(this));
        xmlrpc_c::methodPtr const getPositionP(new getPositionMethod(this));

        myRegistry.addMethod("add", sampleAddMethodP);
        myRegistry.addMethod("position", positionP);
        myRegistry.addMethod("joint", jointP);
        myRegistry.addMethod("model", modelP);
        myRegistry.addMethod("position_ready", positionReadyP);
        myRegistry.addMethod("get_position", getPositionP);

        qDebug() << "XML-RPC server creating...";
        xmlrpc_c::serverAbyss myAbyssServer(
            xmlrpc_c::serverAbyss::constrOpt()
            .registryP(&myRegistry)
            .portNumber(8080));

        myAbyssServer.run();
        // xmlrpc_c::serverAbyss.run() never returns
        assert(false);
    }
    catch (exception const& e)
    {
        cerr << "Something failed.  " << e.what();
    }
    qDebug() << "XML-RPC server started...";
}
