#ifndef XMLRPCSERVERTHREAD_H
#define XMLRPCSERVERTHREAD_H

#include <QThread>
#include <QObject>
#include <QVector3D>

#include "common.h"

class XMLRPCServerThread : public QThread
{
    Q_OBJECT
public:
    explicit XMLRPCServerThread(QObject *parent);

//protected:
    void run();

    void emitSignalPositionReady(int id);
    void emitSignalPositionCurrent(QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles);
    void emitSignalGetPosition(QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles);

signals:
    void positionReady(int position_id);
    void positionCurrent( QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles);
    void receivedPosition(QVector3D xyz, QVector3D rxryrz, FloatVector jointAngles);
};

#endif // XMLRPCSERVERTHREAD_H
