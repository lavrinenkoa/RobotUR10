#include "joint.h"


Joint::Joint(Qt3DCore::QEntity *rootEntity, float size, QColor color,QVector3D position):
size(size), color(color),  position(position), rootEntity(rootEntity)
{

    // Sphere shape data
    mesh = new Qt3DExtras::QSphereMesh();
    mesh->setRings(20);
    mesh->setSlices(20);
    mesh->setRadius(size); // size

    // Sphere Material Color
    material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);

    // Sphere Position and transformation's
    transform = new Qt3DCore::QTransform();
    transform->setTranslation(position);

    // Joint entity
    Qt3DCore::QEntity *joint;
    joint = new Qt3DCore::QEntity(rootEntity);
    joint->addComponent(mesh);               // form
    joint->addComponent(material);           // color
    joint->addComponent(transform);          // zero-position
}

TcpPoint::TcpPoint(QString text, Qt3DCore::QEntity *rootEntity, float size, QColor color,QVector3D position, QVector3D positionF):
size(size), color(color),  position(position),positionF(positionF), rootEntity(rootEntity)
{

    // Sphere shape data
    mesh = new Qt3DExtras::QSphereMesh();
    mesh->setRings(20);
    mesh->setSlices(20);
    mesh->setRadius(size); // size

    // Sphere Material Color
    material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);

    // Sphere Position and transformation's
    transform = new Qt3DCore::QTransform();
//    transform->setTranslation(positionF);
    transform->setTranslation(position);

    // Joint entity
    Qt3DCore::QEntity *joint;
    joint = new Qt3DCore::QEntity(rootEntity);
    joint->addComponent(mesh);               // form
    joint->addComponent(material);           // color
    joint->addComponent(transform);          // zero-position
    Text3D* text3d = new Text3D(rootEntity, 0.1f, color,
                                position + QVector3D(0, 0, 0.1f));
//                              positionF + QVector3D(0, 0, 0.1f));
//                              QVector3D(position.x(), position.y(), position.z()));
    text3d->mesh->setText(text);
}

Cyl::Cyl(Qt3DCore::QEntity *rootEntity, float length, float diam, QColor color,QVector3D position):
length(length), diam(diam), color(color), position(position), rootEntity(rootEntity)
{

    // Sphere shape data
    mesh = new Qt3DExtras::QConeMesh();  //QCylinderMesh
//    mesh->setRings(20);
//    mesh->setSlices(20);
//    mesh->setRadius(size); // size
//    mesh->setLength(0.5f);

    mesh->setTopRadius(diam/2);
    mesh->setBottomRadius(diam/2);
    mesh->setLength(length);
    mesh->setRings(50);
    mesh->setSlices(20);

    // Sphere Material Color
    material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);

    // Sphere Position and transformation's
    transform = new Qt3DCore::QTransform();
    transform->setTranslation(position);

//    QQuaternion rot;
//    rot.fromDirection()
//    transform->setRotation(rot);

    // Joint entity
    Qt3DCore::QEntity *joint;
    joint = new Qt3DCore::QEntity(rootEntity);
    joint->addComponent(mesh);               // form
    joint->addComponent(material);           // color
    joint->addComponent(transform);          // zero-position
}

Text3D::Text3D(Qt3DCore::QEntity *rootEntity, float size, QColor color,QVector3D position):
size(size), color(color),  position(position), rootEntity(rootEntity)
{

    // Sphere shape data
    mesh = new Qt3DExtras::QExtrudedTextMesh();  //QCylinderMesh

    mesh->setDepth(.4f);
    QFont font("Times", 10, QFont::Bold);
    mesh->setFont(font);
//    mesh->setText("Engine Zone");

    // Sphere Material Color
    material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);

    // Sphere Position and transformation's
    transform = new Qt3DCore::QTransform();
    transform->setTranslation(position);
    transform->setRotationZ(90.0f);
    transform->setRotationY(90.0f);
    transform->setScale(size);

//    QQuaternion rot;
//    rot.fromDirection()
//    transform->setRotation(rot);

    // Joint entity
    Qt3DCore::QEntity *joint;
    joint = new Qt3DCore::QEntity(rootEntity);
    joint->addComponent(mesh);               // form
    joint->addComponent(material);           // color
    joint->addComponent(transform);          // zero-position
}

Obj3D::Obj3D(Qt3DCore::QEntity *rootEntity, float size, QColor color, QVector3D position):
size(size), color(color),  position(position), rootEntity(rootEntity)
{                                                                  //RGB
    Qt3DCore::QEntity *m_planeEntity;

    QUrl url;
//    url = QUrl::fromLocalFile("/home/lvr/lvr/TCPOC/qt-test/cobot/e6000.obj");
    url = QUrl::fromLocalFile("/home/lvr/lvr/TCPOC/qt-test/cobot/1.obj");
    mesh = new Qt3DRender::QMesh();
    mesh->setSource(url);

    // Plane mesh transform
    transform = new Qt3DCore::QTransform();
//    planeTransform->setRotationZ(-90.0f);
//    planeTransform->setRotationY(-90.0f);
//    transform->setScale(0.001f);
    transform->setScale(0.04f);
    transform->setTranslation(position);

//    OK
    material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);

    // Plane
    m_planeEntity = new Qt3DCore::QEntity(rootEntity);
    m_planeEntity->addComponent(mesh);
    m_planeEntity->addComponent(material);
    m_planeEntity->addComponent(transform);
}
