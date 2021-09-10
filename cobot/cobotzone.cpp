#include "cobotzone.h"

CobotZone::CobotZone(QVector3D zonePosition, QVector3D zoneSize)
{
    Rx=0;
    Ry=0;
    Rz=0;

    Zx = zonePosition.x();
    Zy = zonePosition.y();
    Zz = zonePosition.z();

    Zh = zoneSize.x();
    Zw = zoneSize.y();
    Zd = zoneSize.z();

    planes[0] = &top;
    planes[1] = &bottom;
    planes[2] = &left;
    planes[3] = &rigth;
    planes[4] = &front;
    planes[5] = &back;

    setZonePositionAndSize();

//    top;
//    bottom;
//    left;
//    rigth;
//    front;
//    back;
}



void CobotZone::setZonePositionAndSize()
{
    top.plane = QVector3D(0, 0, Zz+Zh);
    top.normal = QVector3D(0,0,-1);

    bottom.plane = QVector3D(0, 0, Zz);
    bottom.normal = QVector3D(0,0,1);

    left.plane = QVector3D(0, Zy, 0);
    left.normal = QVector3D(0,+1,0);

    rigth.plane = QVector3D(0, Zy+Zw, 0);
    rigth.normal = QVector3D(0,-1,0);

    front.plane = QVector3D(Zx, 0, 0);
    front.normal = QVector3D(-1,0,0);

    back.plane = QVector3D(Zx-Zd, 0, 0);
    back.normal = QVector3D(+1,0,0);

//---------------- Alternative ---------------
//    top.plane = QVector3D(Zx, Zy, Zz);
//    top.normal = QVector3D(0,0,+1);

//    bottom.plane = QVector3D(Zx, Zy, Zz+Zh);
//    bottom.normal = QVector3D(0,0,-1);

//    left.plane = QVector3D(Zx, Zy, Zz);
//    left.normal = QVector3D(0,-1,0);

//    rigth.plane = QVector3D(Zx, Zy+Zw, Zz);
//    rigth.normal = QVector3D(0,+1,0);

//    front.plane = QVector3D(Zx, Zy, Zz);
//    front.normal = QVector3D(+1,0,0);

//    back.plane = QVector3D(Zx-Zd, Zy, Zz);
//    back.normal = QVector3D(-1,0,0);

}

bool CobotZone::isPointOutsideZone(QVector3D* robotPoint, float haltDistance)
{
    float distance;
    bool pointOutsideZone=true;

    QVector3D zonePlane;
    QVector3D zoneNormal;

    for (int i=0; i<planesNumber; i++){
        zonePlane = planes[i]->plane;
        zoneNormal= planes[i]->normal;
        distance = robotPoint->distanceToPlane(zonePlane, zoneNormal);
        if ( distance > haltDistance){
            pointOutsideZone = false;
//            qDebug() << i << "z=" << robotPoint->z() << "distance" << distance << "OK";
        }
        else{
            // HALT
//            qDebug() << i << "z=" << robotPoint->z() << "distance" << distance << "HALT!!!";
            pointOutsideZone = true;
            return pointOutsideZone;
        }
    }


    return pointOutsideZone;
}
