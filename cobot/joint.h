#ifndef JOINT_H
#define JOINT_H

#include <QObject>
#include <Qt3DCore>
#include <Qt3DExtras>

class Joint
{
public:
    Joint(Qt3DCore::QEntity *rootEntity, float size, QColor color,QVector3D position);

    float size;
    QColor color;
    QVector3D position;

    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *joint;

    Qt3DExtras::QSphereMesh *mesh;
    Qt3DExtras::QPhongMaterial *material;
    Qt3DCore::QTransform *transform;
};

class TcpPoint
{
public:
    TcpPoint(QString text, Qt3DCore::QEntity *rootEntity, float size, QColor color,QVector3D position, QVector3D positionF);

    float size;
    QColor color;
    QVector3D position;
    QVector3D positionF;

    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *joint;

    Qt3DExtras::QSphereMesh *mesh;
    Qt3DExtras::QPhongMaterial *material;
    Qt3DCore::QTransform *transform;
};

class Cyl
{
public:
    Cyl(Qt3DCore::QEntity *rootEntity, float length, float diam, QColor color, QVector3D position);

    float length;
    float diam;
    QColor color;
    QVector3D position;

    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *joint;

//    Qt3DExtras::QCylinderMesh *mesh;
    Qt3DExtras::QConeMesh *mesh;
    Qt3DExtras::QPhongMaterial *material;
    Qt3DCore::QTransform *transform;
};

class Text3D
{
public:
    Text3D(Qt3DCore::QEntity *rootEntity, float size, QColor color,QVector3D position);

    float size;
    QColor color;
    QVector3D position;

    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *joint;

//    Qt3DExtras::QCylinderMesh *mesh;
    Qt3DExtras::QExtrudedTextMesh *mesh;
    Qt3DExtras::QPhongMaterial *material;
    Qt3DCore::QTransform *transform;
};

class Obj3D
{
public:
    Obj3D(Qt3DCore::QEntity *rootEntity, float size, QColor color, QVector3D position);

    float size;
    QColor color;
    QVector3D position;

    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *joint;

//    Qt3DExtras::QCylinderMesh *mesh;
    Qt3DRender::QMesh *mesh;
    Qt3DExtras::QPhongMaterial *material;
    Qt3DCore::QTransform *transform;
};
#endif // JOINT_H
