#ifndef ROBOT_VISUALISATION_H
#define ROBOT_VISUALISATION_H

//#include "mainwindow.h"
//#include "ui_mainwindow.h"

#include <Qt3DExtras>

#include "common.h"
#include "joint.h"
#include "plane.h"

class RobotVisualisation
{
public:
    RobotVisualisation(Qt3DExtras::Qt3DWindow  *view);
    void add_robot();
    void add_cuboid();
    void add_text();
    void add_mesh_engine();

//    RobotSize* robotSize;

    // Around robot arm
    TcpPoint* points[5];
    Plane* planes[2];
    Cuboid* engine[2];
    Text3D* text[2];

    // robot arm visualisation
    Joint* joints[NJOINTS+2];
    Joint* elbows[NELBOW];
    Cyl* segments[9];

};
#endif // ROBOT_VISUALISATION_H
