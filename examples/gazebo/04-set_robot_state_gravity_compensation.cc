// This file is a part of the ORCA framework.
// Copyright 2017, ISIR / Universite Pierre et Marie Curie (UPMC)
// Copyright 2018, Fuzzy Logic Robotics
// Main contributor(s): Antoine Hoarau, Ryan Lober, and
// Fuzzy Logic Robotics <info@fuzzylogicrobotics.com>
//
// ORCA is a whole-body reactive controller framework for robotics.
//
// This software is governed by the CeCILL-C license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL-C
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy,
// modify and redistribute granted by the license, users are provided only
// with a limited warranty  and the software's author,  the holder of the
// economic rights,  and the successive licensors  have only  limited
// liability.
//
// In this respect, the user's attention is drawn to the risks associated
// with loading,  using,  modifying and/or developing or reproducing the
// software by the user in light of its specific status of free software,
// that may mean  that it is complicated to manipulate,  and  that  also
// therefore means  that it is reserved for developers  and  experienced
// professionals having in-depth computer knowledge. Users are therefore
// encouraged to load and test the software's suitability as regards their
// requirements in conditions enabling the security of their systems and/or
// data to be ensured and,  more generally, to use and operate it in the
// same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL-C license and that you accept its terms.

/** @file
 @copyright 2018 Fuzzy Logic Robotics <info@fuzzylogicrobotics.com>
 @author Antoine Hoarau
 @author Ryan Lober
*/

#include <orca/orca.h>
#include <orca/gazebo/GazeboServer.h>
#include <orca/gazebo/GazeboModel.h>

using namespace orca::all;
using namespace orca::gazebo;

int main(int argc, char** argv)
{
    // Get the urdf file from the command line
    if(argc < 2)
    {
        std::cerr << "Usage : " << argv[0] << " /path/to/robot-urdf.urdf" << "\n";
        return -1;
    }
    std::string urdf_url(argv[1]);

    // Instanciate the gazebo server with de dedfault empty world
    GazeboServer gz_server(argc,argv);
    // This is equivalent to GazeboServer gz("worlds/empty.world")
    // Insert a model onto the server and create the GazeboModel from the return value
    // You can also set the initial pose, and override the name in the URDF
    auto gz_model = GazeboModel(gz_server.insertModelFromURDFFile(urdf_url));

    // Create an ORCA robot
    auto robot_model = std::make_shared<RobotModel>();
    robot_model->loadModelFromFile(urdf_url);
    robot_model->print();

    // Set the gazebo model init pose
    // auto joint_names = robot_model->getJointNames();
    // std::vector<double> init_joint_positions(robot_model->getNrOfDegreesOfFreedom(),0);

    // gz_model.setModelConfiguration(joint_names,init_joint_positions);
    // or like this
    // gz_model.setModelConfiguration({"joint_2","joint_5"},{1.5,0.0});

    // Update the robot on at every iteration
    gz_model.executeAfterWorldUpdate([&](uint32_t n_iter,double current_time,double dt)
    {
        robot_model->setRobotState(gz_model.getWorldToBaseTransform().matrix()
                            ,gz_model.getJointPositions()
                            ,gz_model.getBaseVelocity()
                            ,gz_model.getJointVelocities()
                            ,gz_model.getGravity()
                        );
        gz_model.setJointGravityTorques(robot_model->getJointGravityTorques());
    });

    // Run the main simulation loop.
    // This is a blocking call that runs the simulation steps
    // It can be stopped by CTRL+C
    // You can optionally add a callback that happends after WorldUpdateEnd
    std::cout << "Simulation running... (GUI with \'gzclient\')" << "\n";
    gz_server.run();
    return 0;
}
