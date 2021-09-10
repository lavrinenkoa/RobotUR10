#ifndef SAFETYPLANE_H
#define SAFETYPLANE_H

#include <Qt3DCore>

class SafetyPlane
{
public:
    SafetyPlane();

    // Main plane parameters
    QVector3D plane;   // plane point
    QVector3D normal;  // normal vector

};

#endif // SAFETYPLANE_H
