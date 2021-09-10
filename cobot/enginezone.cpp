#include "enginezone.h"

EngineZone::EngineZone(QVector3D zonePosition, QVector3D zoneSize)
{
    Rx=0;
    Ry=0;
    Rz=0;

    Zx = zonePosition.x();
    Zy = zonePosition.y();
    Zz = zonePosition.z();

    Zd = zoneSize.x();
    Zw = zoneSize.y();
    Zh = zoneSize.z();

    CZx = Zx - Zd/2.0f;
    CZy = Zy + Zw/2.0f;
    CZz = Zz + Zh/2.0f;

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

void EngineZone::setZonePositionAndSize()
{
    top.plane = QVector3D(0, 0, Zz+Zh);
    top.normal = QVector3D(0,0,+1);

    bottom.plane = QVector3D(0, 0, Zz);
    bottom.normal = QVector3D(0,0,-1);

    left.plane = QVector3D(0, Zy, 0);
    left.normal = QVector3D(0,-1,0);

    rigth.plane = QVector3D(0, Zy+Zw, 0);
    rigth.normal = QVector3D(0,+1,0);

    front.plane = QVector3D(Zx, 0, 0);
    front.normal = QVector3D(+1,0,0);

    back.plane = QVector3D(Zx-Zd, 0, 0);
    back.normal = QVector3D(-1,0,0);
}

bool EngineZone::isPointInsideZone(QVector3D* robotPoint, float haltDistance)
{
    float dtop;
    float dbottom;
    float dleft;
    float drigth;
    float dfront;
    float dback;
    bool pointInsideZone=true;

    dtop    = robotPoint->distanceToPlane(top.plane,    top.normal)   - haltDistance;
    dbottom = robotPoint->distanceToPlane(bottom.plane, bottom.normal)- haltDistance;
    dleft   = robotPoint->distanceToPlane(left.plane,   left.normal)  - haltDistance;
    drigth  = robotPoint->distanceToPlane(rigth.plane,  rigth.normal) - haltDistance;
    dfront  = robotPoint->distanceToPlane(front.plane,  front.normal) - haltDistance;
    dback   = robotPoint->distanceToPlane(back.plane,   back.normal)  - haltDistance;

    pointInsideZone = (    (dtop    < 0)
                       and (dbottom < 0)
                       and (dleft   < 0)
                       and (drigth  < 0)
                       and (dfront  < 0)
                       and (dback   < 0));

    if (pointInsideZone)
        qDebug() << "HALT!!!";

    return pointInsideZone;
}


//bool EngineZone::isPointInsideZone1(QVector3D* P, float haltDistance)
//{

//    bool inside;
//    P= J0;
//    Px = P->x();
//    Py = P->y();
//    Pz = P->z();
//    inside = (Px>Ax) and (Px<B1x) and (Py>Ay) and (Py<Dy) and (Pz>Az) and (Pz<Bz);


//    if (pointInsideZone)
//        qDebug() << "HALT!!!";

//    return pointInsideZone;
//}

bool EngineZone::isSegmentInZone(QVector3D* J0, QVector3D* J1, float haltDistance)
{
    bool segmentInsideZone=true;
    QVector3D* P;
    float Px;
    float Py;
    float Pz;

    float Ax, Ay, Az;
    float Bx, By, Bz;
    float Cx, Cy, Cz;
    float Dx, Dy, Dz;

    bool I0, I1;
    bool T0, T1;
    bool B0, B1;
    bool L0, L1;
    bool R0, R1;
    bool F0, F1;
    bool Bk0,Bk1;
    bool k,l,m;

    Ax = Zx + haltDistance;
    Ay = Zy - haltDistance;
    Az = Zz - haltDistance;

    Bx = Ax;
    By = Ay;
    Bz = Az + Zh + 2*haltDistance;

    Cx = Ax - Zd + 2*haltDistance;
    Cy = Ay;
    Cz = Az;

    Dx = Ax;
    Dy = Ay+ Zw + 2*haltDistance;
    Dz = Az;

    P= J0;
    Px = P->x();
    Py = P->y();
    Pz = P->z();
    I0 = ((Px<Ax) and (Px>Cx) and (Py>Ay) and (Py<Dy) and (Pz>Az) and (Pz<Bz));
    T0 = ((Px<Ax) and (Px>Cx) and (Py>Ay) and (Py<Dy) and (Pz>Bz));
    B0 = ((Px<Ax) and (Px>Cx) and (Py>Ay) and (Py<Dy) and (Pz<Zz));
    L0 = ((Px<Ax) and (Px>Cx) and (Py<Ay) and             (Pz>Az) and (Pz<Bz));
    R0 = ((Px<Ax) and (Px>Cx) and (Py>Dy) and             (Pz>Az) and (Pz<Bz));
    F0 = ((Px>Ax) and             (Py>Ay) and (Py<Dy) and (Pz>Az) and (Pz<Bz));
    Bk0= ((Px<Cx) and             (Py>Ay) and (Py<Dy) and (Pz>Az) and (Pz<Bz));
//    qDebug() << T0 << B0 << L0 << R0 << F0 << Bk0;

    P= J1;
    Px = P->x();
    Py = P->y();
    Pz = P->z();
    I1 = ((Px<Ax) and (Px>Cx) and (Py>Ay) and (Py<Dy) and (Pz>Az) and (Pz<Bz));
    T1 = ((Px<Ax) and (Px>Cx) and (Py>Ay) and (Py<Dy) and (Pz>Bz));
    B1 = ((Px<Ax) and (Px>Cx) and (Py>Ay) and (Py<Dy) and (Pz<Zz));
    L1 = ((Px<Ax) and (Px>Cx) and (Py<Ay) and             (Pz>Az) and (Pz<Bz));
    R1 = ((Px<Ax) and (Px>Cx) and (Py>Dy) and             (Pz>Az) and (Pz<Bz));
    F1 = ((Px>Ax) and             (Py>Ay) and (Py<Dy) and (Pz>Az) and (Pz<Bz));
    Bk1= ((Px<Cx) and             (Py>Ay) and (Py<Dy) and (Pz>Az) and (Pz<Bz));
//    qDebug() << T1 << B1 << L1 << R1 << F1 << Bk1;


    k = (L0 and T1) or (T0 and R1)  or (R0 and  B1) or (B0 and L1);
    l = (F0 and T1) or (T0 and Bk1) or (Bk0 and B1) or (B1 and F1);
    m = (F0 and L1) or (L0 and Bk1) or (Bk1 and R1) or (R0 and F1);

//    qDebug() << k << l << m << I0 << I1;
    segmentInsideZone = k or l or m or I0 or I1;

    if (segmentInsideZone)
        qDebug() << "engine zone. HALT!!!";
    return segmentInsideZone;
}
