#ifndef COBOTZONE_H
#define COBOTZONE_H

#include "safetyplane.h"

#define PLANES_NAMBER (6)

class CobotZone
{
public:
    CobotZone(QVector3D zonePosition, QVector3D zoneSize);
    void setZonePositionAndSize();
    bool isPointOutsideZone(QVector3D* robotPoint, float haltDistance);

// private:
    float Rx, Ry, Rz; // Robot position, central point R(x,y,z)
    float Zx, Zy, Zz; // Zone position
    float Zh, Zw, Zd; // Zone size

    int planesNumber=PLANES_NAMBER;
    SafetyPlane* planes[PLANES_NAMBER];

    SafetyPlane top;
    SafetyPlane bottom;
    SafetyPlane left;
    SafetyPlane rigth;
    SafetyPlane front;
    SafetyPlane back;
};

#endif // COBOTZONE_H
