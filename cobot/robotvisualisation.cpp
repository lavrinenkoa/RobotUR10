#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui/QScreen>
#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>

#include <Qt3DInput/QInputAspect>
#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>

#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

#include <QPlaneMesh>

#include "robotvisualisation.h"

#include "joint.h"

RobotVisualisation::RobotVisualisation(Qt3DExtras::Qt3DWindow *view)
{
//    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
//    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));

//    QWidget *container = QWidget::createWindowContainer(view);
//    QSize screenSize = view->screen()->size();
//    container->setMinimumSize(QSize(640, 480));
//    container->setMaximumSize(screenSize);

//    ui->verticalLayout->addWidget(container);


//    // Input action
//    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
//    view->registerAspect(input);

    // Root entity
//    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();
    rootEntity = new Qt3DCore::QEntity();

    // Camera
    Qt3DRender::QCamera *cameraEntity = view->camera();

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(4, 0, 1.0f));
//    cameraEntity->setUpVector(QVector3D(0, 1, 0));
//    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
//    cameraEntity->setPosition(QVector3D(5, 0, 1));
    cameraEntity->setUpVector(QVector3D(0, 0, 3.14f));
    cameraEntity->setViewCenter(QVector3D(0, 0, 1.0f));

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    // For camera controls
    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(cameraEntity);


    // Scenemodifier
    // SceneModifier *modifier = new SceneModifier(rootEntity);

    // ADD 3D Objects
    // Sphere shape data
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
    sphereMesh->setRings(20);
    sphereMesh->setSlices(20);
    sphereMesh->setRadius(0.1f);

    // Sphere material
    Qt3DExtras::QPhongMaterial *sphereMaterial = new Qt3DExtras::QPhongMaterial();
    sphereMaterial->setDiffuse(QColor(QRgb(0xa69929)));

    // Sphere ZERO
    Qt3DExtras::QPhongMaterial *zero_sphereMaterial = new Qt3DExtras::QPhongMaterial();
    zero_sphereMaterial->setDiffuse(QColor(QRgb(0xa69929)));

    Qt3DCore::QTransform *zero_sphereTransform = new Qt3DCore::QTransform();
    zero_sphereTransform = new Qt3DCore::QTransform();
    zero_sphereTransform->setScale(0.1f);
    zero_sphereTransform->setTranslation(QVector3D(-0.0f, -0.0f, 0.0f));

    Qt3DCore::QEntity *zero_sphereEntity;
    zero_sphereEntity = new Qt3DCore::QEntity(rootEntity);
    zero_sphereEntity->addComponent(sphereMesh);
    zero_sphereEntity->addComponent(zero_sphereMaterial);
    zero_sphereEntity->addComponent(zero_sphereTransform);

    // Sphere Blue
    Qt3DExtras::QPhongMaterial *z1_sphereMaterial = new Qt3DExtras::QPhongMaterial();
    z1_sphereMaterial->setDiffuse(QColor(QRgb(0x0000FF)));

    Qt3DCore::QTransform *z1_sphereTransform = new Qt3DCore::QTransform();
    z1_sphereTransform = new Qt3DCore::QTransform();
    z1_sphereTransform->setScale(0.1f);
    z1_sphereTransform->setTranslation(QVector3D(1.0f, -0.0f, 0.0f));

    Qt3DCore::QEntity *z1_sphereEntity;
    z1_sphereEntity = new Qt3DCore::QEntity(rootEntity);
    z1_sphereEntity->addComponent(sphereMesh);
    z1_sphereEntity->addComponent(z1_sphereMaterial);
    z1_sphereEntity->addComponent(z1_sphereTransform);

    // Sphere Green
    Qt3DExtras::QPhongMaterial *z2_sphereMaterial = new Qt3DExtras::QPhongMaterial();
    z2_sphereMaterial->setDiffuse(QColor(QRgb(0x00FF00)));

    Qt3DCore::QTransform *z2_sphereTransform = new Qt3DCore::QTransform();
    z2_sphereTransform = new Qt3DCore::QTransform();
    z2_sphereTransform->setScale(0.1f);
    z2_sphereTransform->setTranslation(QVector3D(0.0f, 1.0f, 0.0f));

    Qt3DCore::QEntity *z2_sphereEntity;
    z2_sphereEntity = new Qt3DCore::QEntity(rootEntity);
    z2_sphereEntity->addComponent(sphereMesh);
    z2_sphereEntity->addComponent(z2_sphereMaterial);
    z2_sphereEntity->addComponent(z2_sphereTransform);

    // Sphere Red
    Qt3DExtras::QPhongMaterial *z3_sphereMaterial = new Qt3DExtras::QPhongMaterial();
    z3_sphereMaterial->setDiffuse(QColor(QRgb(0xFF0000)));

    Qt3DCore::QTransform *z3_sphereTransform = new Qt3DCore::QTransform();
    z3_sphereTransform = new Qt3DCore::QTransform();
    z3_sphereTransform->setScale(0.1f);
    z3_sphereTransform->setTranslation(QVector3D(0.0f, -0.0f, 1.0f));

    Qt3DCore::QEntity *z3_sphereEntity;
    z3_sphereEntity = new Qt3DCore::QEntity(rootEntity);
    z3_sphereEntity->addComponent(sphereMesh);
    z3_sphereEntity->addComponent(z3_sphereMaterial);
    z3_sphereEntity->addComponent(z3_sphereTransform);

    // Sphere
//    sphereMaterial->setDiffuse(QColor(QRgb(0xa00929)));
//    // Sphere mesh transform
//    // Qt3DCore::QTransform *m_sphereTransform = new Qt3DCore::QTransform();
//    m_sphereTransform = new Qt3DCore::QTransform();
////    m_sphereTransform->setTranslation(QVector3D(0.4f, -0.5f, 0.8f));
//    m_sphereTransform->setScale(0.5f);
//    m_sphereTransform->setTranslation(QVector3D(0.4f, 0.0f, 0.0f));

//    m_sphereEntity = new Qt3DCore::QEntity(rootEntity);
//    m_sphereEntity->addComponent(sphereMesh);
//    m_sphereEntity->addComponent(sphereMaterial);
//    m_sphereEntity->addComponent(m_sphereTransform);
//    m_sphereEntity->blockSignals(true);



//    add_robot();
//    add_obj();

    // Set root object of the scene
    view->setRootEntity(rootEntity);
}

void RobotVisualisation::add_robot()
{
    planes[0] = new Plane(rootEntity,
                          3.0f,
                          QColor(QRgb(0x00FF00)),
                          QVector3D(0.0f, 0.5f, 0.0f),
                          QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 90.0f));

//    robot->planes[1] = new Plane(rootEntity,
//                                 3.0f,
//                                 QColor(QRgb(0xFF0000)),
//                                 QVector3D(0.0f, 0.0f, +1.0f),
//                                 QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 90.0f));

    float xd = robot->engineZone->Zd;
    float yw = robot->engineZone->Zw;
    float zh = robot->engineZone->Zh;
    float cx = robot->engineZone->CZx;
    float cy = robot->engineZone->CZy;
    float cz = robot->engineZone->CZz;
    engine[0] = new Cuboid(rootEntity,
                           QVector3D(xd, yw, zh),
                           QColor(QRgb(0xFF0000)),
                           QVector3D(cx, cy, cz));

    text[0] = new Text3D(rootEntity, 0.25f, QColor(QRgb(0xFF0000)), QVector3D(0.5f, -1.5f, 1.8f));
//    text[1] = new Text3D(rootEntity, 0.25f, QColor(QRgb(0xFF0000)), QVector3D(0.5f, -1.5f, 1.5f));

//    robot->joints[0] = new Joint(rootEntity, 0.02f, QColor(QRgb(0x0000FF)), QVector3D(0.0f, 0.0f, 0.0f));
//    robot->joints[1] = new Joint(rootEntity, 0.02f, QColor(QRgb(0x00FF00)), QVector3D(0.0f, 0.0f, 0.0f));
//    robot->joints[2] = new Joint(rootEntity, 0.02f, QColor(QRgb(0x999999)), QVector3D(0.0f, 0.0f, 0.0f));
//    robot->joints[3] = new Joint(rootEntity, 0.02f, QColor(QRgb(0xFF00FF)), QVector3D(0.0f, 0.0f, 0.0f));
//    robot->joints[4] = new Joint(rootEntity, 0.02f, QColor(QRgb(0xFFFF00)), QVector3D(0.0f, 0.0f, 0.0f));
//    robot->joints[5] = new Joint(rootEntity, 0.02f, QColor(QRgb(0xFF0000)), QVector3D(0.0f, 0.0f, 0.0f));
//    robot->joints[6] = new Joint(rootEntity, 0.02f, QColor(QRgb(0xFF0000)), QVector3D(0.0f, 0.0f, 0.0f));


    joints[0] = new Joint(rootEntity, robot->S0d/2+0.04f, QColor(QRgb(0x0000FF)), QVector3D(0.0f, 0.0f, 0.0f));
    joints[1] = new Joint(rootEntity, robot->S1d/2+0.00f, QColor(QRgb(0x00FF00)), QVector3D(0.0f, 0.0f, 0.0f));
    joints[2] = new Joint(rootEntity, robot->S2d/2+0.00f, QColor(QRgb(0x999999)), QVector3D(0.0f, 0.0f, 0.0f));
    joints[3] = new Joint(rootEntity, robot->S4d/2+0.00f, QColor(QRgb(0xFF00FF)), QVector3D(0.0f, 0.0f, 0.0f));
    joints[4] = new Joint(rootEntity, robot->S6d/2+0.00f, QColor(QRgb(0xFFFF00)), QVector3D(0.0f, 0.0f, 0.0f));
    joints[5] = new Joint(rootEntity, robot->S7d/2+0.00f, QColor(QRgb(0xFF0000)), QVector3D(0.0f, 0.0f, 0.0f));
    joints[6] = new Joint(rootEntity, 0.02f             , QColor(QRgb(0xFF0000)), QVector3D(0.0f, 0.0f, 0.0f));

    elbows[1] = new Joint(rootEntity, robot->S0d/2, QColor(QRgb(0x00FF00)), QVector3D(0.0f, 0.0f, 0.0f));
    elbows[2] = new Joint(rootEntity, robot->S3d/2, QColor(QRgb(0x999999)), QVector3D(0.0f, 0.0f, 0.0f));
    elbows[3] = new Joint(rootEntity, robot->S5d/2, QColor(QRgb(0xFF00FF)), QVector3D(0.0f, 0.0f, 0.0f));

    segments[0] = new Cyl(rootEntity, robot->L0v-0.02f, robot->S0d, QColor(QRgb(0xFFFFFF)), QVector3D(0.0f, 0.0f, 0.0f));
    segments[1] = new Cyl(rootEntity, robot->L0h-0.02f, robot->S1d, QColor(QRgb(0xFFFFFF)), QVector3D(0.0f, 0.0f, 0.0f));
    segments[2] = new Cyl(rootEntity, robot->L1v-0.02f, robot->S2d, QColor(QRgb(0xFFFFFF)), QVector3D(0.0f, 0.0f, 0.0f));
    segments[3] = new Cyl(rootEntity, robot->L1h-0.02f, robot->S3d, QColor(QRgb(0xFFFFFF)), QVector3D(0.0f, 0.0f, 0.0f));
    segments[4] = new Cyl(rootEntity, robot->L2v-0.02f, robot->S4d, QColor(QRgb(0xFFFFFF)), QVector3D(0.0f, 0.0f, 0.0f));
    segments[5] = new Cyl(rootEntity, robot->L2h-0.02f, robot->S5d, QColor(QRgb(0xFFFFFF)), QVector3D(0.0f, 0.0f, 0.0f));
    segments[6] = new Cyl(rootEntity, robot->L3v-0.02f, robot->S6d, QColor(QRgb(0xFFFFFF)), QVector3D(0.0f, 0.0f, 0.0f));
    segments[7] = new Cyl(rootEntity, robot->L3h-0.02f, robot->S7d, QColor(QRgb(0xFFFFFF)), QVector3D(0.0f, 0.0f, 0.0f));

    // Not robot points
//    points[0] = new JointF("p1", rootEntity, 0.05f, QColor(QRgb(0x0000FF)), QVector3D(0.4f,-0.5f, 0.7f), QVector3D(0.553595, -0.703272, 0.952442));
//    points[1] = new JointF("p2", rootEntity, 0.05f, QColor(QRgb(0x0000FF)), QVector3D(0.4f,-0.5f, 0.0f), QVector3D(0.534252, -0.731913, -0.0756228));
//    points[2] = new JointF("p3", rootEntity, 0.05f, QColor(QRgb(0x0000FF)), QVector3D(0.4f, 0.5f, 0.0f), QVector3D(0.573997, 0.703732, -0.0830903));
//    points[3] = new JointF("p4", rootEntity, 0.05f, QColor(QRgb(0x0000FF)), QVector3D(-0.3f, 0.4f, 0.5f),QVector3D(-0.468239, 0.569581, 0.69225));

    points[0] = new TcpPoint("", rootEntity, 0.02f, QColor(QRgb(0xFFFFFF)), QVector3D(-0.3f, 0.3f, 0.45f), QVector3D( 0.553595f, -0.703272f, 0.952442f));
    points[1] = new TcpPoint("", rootEntity, 0.02f, QColor(QRgb(0xFFFFFF)), QVector3D(-0.3f, 0.45f, 0.9f), QVector3D( 0.534252f, -0.731913f, -0.0756228f));
    points[2] = new TcpPoint("", rootEntity, 0.02f, QColor(QRgb(0xFFFFFF)), QVector3D(-0.3f, 0.7f, 0.8f),  QVector3D( 0.573997f, 0.703732f, -0.0830903f));
    points[3] = new TcpPoint("", rootEntity, 0.02f, QColor(QRgb(0xFFFFFF)), QVector3D(-0.3f, 0.9f, 0.5f),  QVector3D(-0.468239f, 0.569581f, 0.69225f));
    points[4] = new TcpPoint("", rootEntity, 0.02f, QColor(QRgb(0xFFFFFF)), QVector3D(-0.3f, 1.1f, 0.6f),  QVector3D(-0.468239f, 0.569581f, 0.69225f));
}

void RobotVisualisation::add_cuboid()
{                                                                  //RGB
    Qt3DCore::QEntity *m_planeEntity;

    // Plane shape data
    Qt3DExtras::QCuboidMesh *planeMesh = new Qt3DExtras::QCuboidMesh();
    planeMesh->setXExtent(1);
    planeMesh->setYExtent(1);
    planeMesh->setZExtent(1);

    // Plane mesh transform
    Qt3DCore::QTransform *planeTransform = new Qt3DCore::QTransform();
//    planeTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 90.0f));
    planeTransform->setTranslation(QVector3D(-1.0f, 1.0f, 0.0f));

    Qt3DExtras::QPhongMaterial *planeMaterial = new Qt3DExtras::QPhongMaterial();
    planeMaterial->setDiffuse(QColor(QRgb(0xa69929)));

    // Plane
    m_planeEntity = new Qt3DCore::QEntity(rootEntity);
    m_planeEntity->addComponent(planeMesh);
    m_planeEntity->addComponent(planeMaterial);
    m_planeEntity->addComponent(planeTransform);
}

void RobotVisualisation::add_text()
{                                                                  //RGB
    Qt3DCore::QEntity *m_planeEntity;

    // Plane shape data
    Qt3DExtras::QExtrudedTextMesh *mesh = new Qt3DExtras::QExtrudedTextMesh();
    mesh->setDepth(.45f);
//    QFont font(family, 32, -1, false);
    mesh->setFont(QFont("monospace"));
    mesh->setText("Engine Zone");

    // Plane mesh transform
    Qt3DCore::QTransform *planeTransform = new Qt3DCore::QTransform();
//    planeTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 90.0f));
    planeTransform->setRotationZ(90.0f);
    planeTransform->setRotationY(90.0f);
    planeTransform->setTranslation(QVector3D(-1.0f, 1.0f, 0.0f));

    Qt3DExtras::QPhongMaterial *planeMaterial = new Qt3DExtras::QPhongMaterial();
    planeMaterial->setDiffuse(QColor(QRgb(0xa69929)));

    // Plane
    m_planeEntity = new Qt3DCore::QEntity(rootEntity);
    m_planeEntity->addComponent(mesh);
    m_planeEntity->addComponent(planeMaterial);
    m_planeEntity->addComponent(planeTransform);
}

void RobotVisualisation::add_mesh_engine()
{                                                                  //RGB
    Qt3DCore::QEntity *entity;

    QUrl url;
    url = QUrl::fromLocalFile("1.obj");
    Qt3DRender::QMesh * mesh = new Qt3DRender::QMesh();
    mesh->setSource(url);

    // Plane mesh transform
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
//    planeTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 90.0f));
//    planeTransform->setRotationX(90.0f);
    transform->setRotationZ(-90.0f);
    transform->setRotationY(-90.0f);
    transform->setRotationX(-90.0f);
    transform->setScale(0.0013f);
    transform->setTranslation(QVector3D(-1.7f, 1.4f, 0.3f));

//    OK
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(QRgb(0x888888)));
//    material->setDiffuse(QColor(QRgb(0xFF00000)));

//    Qt3DExtras::QPhongAlphaMaterial *planeMaterial = new Qt3DExtras::QPhongAlphaMaterial();
//    planeMaterial->setDiffuse(QColor(QRgb(0x0000FF)));
//    planeMaterial->setAlpha(0.5);

    // Plane
    entity = new Qt3DCore::QEntity(rootEntity);
    entity->addComponent(mesh);
    entity->addComponent(material);
    entity->addComponent(transform);
}

