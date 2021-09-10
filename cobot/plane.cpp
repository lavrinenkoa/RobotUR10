#include "plane.h"

Plane::Plane(Qt3DCore::QEntity *rootEntity, float size, QColor color,QVector3D position, QQuaternion rotation):
size(size), color(color),  rotation(rotation), position(position), rootEntity(rootEntity)
{

    // Plane shape data
    mesh = new Qt3DExtras::QPlaneMesh();
    mesh->setWidth(size);  // size
    mesh->setHeight(size);

    // Plane Material Color
    material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);

//    Qt3DExtras::QPhongAlphaMaterial *material = new Qt3DExtras::QPhongAlphaMaterial();
//    material->setDiffuse(color);
//    material->setAlpha(0.99f);

    // Plane Position and transformation's
    transform = new Qt3DCore::QTransform();
//    transform->setTranslation(position);
//    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setRotation(rotation);
    transform->setTranslation(position);

    // Plane entity
    Qt3DCore::QEntity *entity;
    entity = new Qt3DCore::QEntity(rootEntity);
    entity->addComponent(mesh);               // form
    entity->addComponent(material);           // color
    entity->addComponent(transform);          // zero-position
}

Cuboid::Cuboid(Qt3DCore::QEntity *rootEntity, QVector3D size, QColor color,QVector3D position):
size(size), color(color), position(position), rootEntity(rootEntity)
{

    // Plane shape data
    mesh = new Qt3DExtras::QCuboidMesh();
    mesh->setXExtent(size.x());
    mesh->setYExtent(size.y());
    mesh->setZExtent(size.z());

    // Plane Material Color
//    material = new Qt3DExtras::QPhongMaterial();
//    material->setDiffuse(color);

    Qt3DExtras::QPhongAlphaMaterial *material = new Qt3DExtras::QPhongAlphaMaterial();
    material->setDiffuse(color);
    material->setAlpha(0.2f);

    // Plane Position and transformation's
    transform = new Qt3DCore::QTransform();
//    transform->setTranslation(position);
//    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setRotation(rotation);
    transform->setTranslation(position);

    // Plane entity
    Qt3DCore::QEntity *entity;
    entity = new Qt3DCore::QEntity(rootEntity);
    entity->addComponent(mesh);               // form
    entity->addComponent(material);           // color
    entity->addComponent(transform);          // zero-position
}

