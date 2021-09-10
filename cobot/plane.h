#ifndef PLANE_H
#define PLANE_H

#include <QObject>
#include <Qt3DCore>
#include <Qt3DExtras>

class Plane
{
public:
    Plane(Qt3DCore::QEntity *rootEntity, float size, QColor color,QVector3D position, QQuaternion rotation);

    float size;
    QColor color;
    QVector3D position;
    QQuaternion rotation;

    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *joint;

    Qt3DExtras::QPlaneMesh *mesh;
    Qt3DExtras::QPhongMaterial *material;
    Qt3DCore::QTransform *transform;
};


class Cuboid
{
public:
    Cuboid(Qt3DCore::QEntity *rootEntity, QVector3D size, QColor color,QVector3D position);

    QVector3D size;
    QColor color;
    QVector3D position;
    QQuaternion rotation;

    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *joint;

    Qt3DExtras::QCuboidMesh *mesh;
    Qt3DExtras::QPhongMaterial *material;
    Qt3DCore::QTransform *transform;
};
#endif // PLANE_H
