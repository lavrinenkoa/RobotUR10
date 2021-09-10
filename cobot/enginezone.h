#ifndef ENGINEZONE_H
#define ENGINEZONE_H

#include "safetyplane.h"

#define PLANES_NAMBER (6)

class EngineZone
{
public:
    EngineZone(QVector3D zonePosition, QVector3D zoneSize);
    void setZonePositionAndSize();
    bool isPointInsideZone(QVector3D* robotPoint, float haltDistance);
    bool isSegmentInZone(QVector3D* J0, QVector3D* J1, float haltDistance);

// private:
    float Rx, Ry, Rz; // Robot position, central point R(x,y,z)
    float Zx, Zy, Zz; // Zone position
    float Zh, Zw, Zd; // Zone size
    float CZx, CZy, CZz; // Zone center position

    int planesNumber=PLANES_NAMBER;
    SafetyPlane* planes[PLANES_NAMBER];

    SafetyPlane top;
    SafetyPlane bottom;
    SafetyPlane left;
    SafetyPlane rigth;
    SafetyPlane front;
    SafetyPlane back;
};

#endif // ENGINEZONE_H
