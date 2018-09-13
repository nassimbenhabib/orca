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
// 'http://www.cecill.info'.
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
using namespace orca::all;

std::string urdf_string = "<?xml version='1.0' ?>"
"<robot name='lwr' xmlns:controller='http://playerstage.sourceforge.net/gazebo/xmlschema/#controller' xmlns:xacro='http://ros.org/wiki/xacro'>"
"<material name='color_j0'>"
"<color rgba='1.0 0.423529411765 0.196078431373 1.0'/>"
"</material>"
"<material name='color_j1'>"
"<color rgba='1.0 0.423529411765 0.196078431373 1.0'/>"
"</material>"
"<material name='color_j2'>"
"<color rgba='1.0 0.423529411765 0.196078431373 1.0'/>"
"</material>"
"<material name='color_j3'>"
"<color rgba='1.0 0.423529411765 0.196078431373 1.0'/>"
"</material>"
"<material name='color_j4'>"
"<color rgba='1.0 0.423529411765 0.196078431373 1.0'/>"
"</material>"
"<material name='color_j5'>"
"<color rgba='1.0 0.423529411765 0.196078431373 1.0'/>"
"</material>"
"<material name='color_j6'>"
"<color rgba='0.3 0.3 0.3 1.0'/>"
"</material>"
"<material name='color_j7'>"
"<color rgba='0.7 0.7 0.7 1.0'/>"
"</material>"
"<!-- world-->"
"<link name='world'/>"
"<!-- lbr -->"
"<joint name='world_joint' type='fixed'>"
"<parent link='world'/>"
"<child link='table_link'/>"
"<origin xyz='0 0 0.845'/>"
"<limit effort='400000' lower='0' upper='0' velocity='0'/>"
"</joint>"
"<link name='table_link'>"
"<inertial>"
"<origin rpy='0 0 0' xyz='0 0 0'/>"
"<mass value='200'/>"
"<inertia ixx='0.1' ixy='0' ixz='0' iyy='0.1' iyz='0' izz='0.1'/>"
"</inertial>"
"<visual>"
"<origin rpy='0 0 0' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/table/table.stl'/>"
"</geometry>"
"<material name='table_color'>"
"<color rgba='0.823529411765 0.666666666667 0.470588235294 1.0'/>"
"</material>"
"</visual>"
"<collision>"
"<origin rpy='0 0 0' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/table/table_collision.stl'/>"
"</geometry>"
"</collision>"
"</link>"
"<joint name='table_to_base_joint' type='fixed'>"
"<origin rpy='0 0 0' xyz='0 0 0'/>"
"<parent link='table_link'/>"
"<child link='base_link'/>"
"<limit effort='40000' lower='0' upper='0' velocity='0'/>"
"</joint>"
"<link name='base_link'>"
"<visual>"
"<origin rpy='0 0 0' xyz='0 0 0.01'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr_base/lwr_base.stl'/>"
"</geometry>"
"<material name='color_j7'/>"
"</visual>"
"<collision>"
"<origin rpy='0 0 0' xyz='0 0 0.01'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr_base/lwr_base_collision.stl'/>"
"</geometry>"
"</collision>"
"</link>"
"<!-- joint between {parent} and lwr_0_link -->"
"<joint name='joint_base' type='fixed'>"
"<origin rpy='0 0 2.09439510239' xyz='0 0 0.02'/>"
"<parent link='base_link'/>"
"<child link='link_0'/>"
"</joint>"
"<link name='link_0'>"
"<inertial>"
"<origin rpy='0 0 0' xyz='-0.000638499331014356 5.02538509694617E-06 0.0482289968116927'/>"
"<mass value='1.21032454350876'/>"
"<inertia ixx='0.0262560565710656' ixy='-5.2754950052563E-07' ixz='3.77940202490646E-05' iyy='0.0280724642508563' iyz='-2.56972470148208E-07' izz='0.0306998250407766'/>"
"</inertial>"
"<visual>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link0.stl'/>"
"</geometry>"
"<material name='color_j0'/>"
"</visual>"
"<collision>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link0_c2.stl'/>"
"</geometry>"
"</collision>"
"</link>"
"<!-- joint between lwr_0_link and lwr_1_link -->"
"<joint name='joint_0' type='revolute'>"
"<parent link='link_0'/>"
"<child link='link_1'/>"
"<limit effort='2000.0' lower='-2.96705972839' upper='2.96705972839' velocity='1.96349540849'/>"
"<dynamics damping='0.2' friction='1.0'/>"
"<origin rpy='0 0 0' xyz='0 0 0.102'/>"
"<axis xyz='0 0 1'/>"
"</joint>"
"<link name='link_1'>"
"<inertial>"
"<origin rpy='0 0 0' xyz='-6.33965437334127E-08 0.0233273473346096 0.118146290406178'/>"
"<mass value='2.30339938771869'/>"
"<inertia ixx='0.156081163626041' ixy='5.97319920503909E-08' ixz='-1.64780770629425E-07' iyy='0.153467542173805' iyz='0.0319168949093809' izz='0.0440736079943446'/>"
"</inertial>"
"<visual>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link1.stl'/>"
"</geometry>"
"<material name='color_j1'/>"
"</visual>"
"<collision>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link1_c2.stl'/>"
"</geometry>"
"</collision>"
"</link>"
"<!-- joint between lwr_1_link and lwr_2_link -->"
"<joint name='joint_1' type='revolute'>"
"<parent link='link_1'/>"
"<child link='link_2'/>"
"<limit effort='2000.0' lower='-2.09439510239' upper='2.09439510239' velocity='1.96349540849'/>"
"<dynamics damping='0.18' friction='1.0'/>"
"<origin rpy='0 0 0' xyz='0 0.0 0.2085'/>"
"<axis xyz='0 -1 0'/>"
"</joint>"
"<link name='link_2'>"
"<inertial>"
"<origin rpy='0 0 0' xyz='1.26774962153076E-06 -0.032746486541291 0.0736556727355962'/>"
"<mass value='2.30343543179071'/>"
"<inertia ixx='0.0142348526057094' ixy='-3.73763310100809E-08' ixz='1.70703603169075E-07' iyy='0.0141319978448755' iyz='0.00228090337255746' izz='0.00424792208583136'/>"
"</inertial>"
"<visual>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link2.stl'/>"
"</geometry>"
"<material name='color_j2'/>"
"</visual>"
"<collision>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link2_c2.stl'/>"
"</geometry>"
"<surface>"
"<friction>"
"  <ode>"
"    <mu>0.998000</mu>"
"    <mu2>0.998000</mu2>"
"  </ode>"
"</friction>"
"</surface>"
"</collision>"
"</link>"
"<!-- joint between lwr_2_link and lwr_3_link -->"
"<joint name='joint_2' type='revolute'>"
"<parent link='link_2'/>"
"<child link='link_3'/>"
"<limit effort='1000.0' lower='-2.96705972839' upper='2.96705972839' velocity='1.96349540849'/>"
"<dynamics damping='0.1' friction='1.0'/>"
"<origin rpy='0 0 0' xyz='0 0 0.1915'/>"
"<axis xyz='0 0 1'/>"
"</joint>"
"<link name='link_3'>"
"<inertial>"
"<origin rpy='0 0 0' xyz='-1.40921289121243E-06 -0.0233297626126898 0.11815047247629'/>"
"<mass value='2.30342143971329'/>"
"<inertia ixx='0.0156098024078732' ixy='4.75479645197283E-08' ixz='1.17852233217589E-07' iyy='0.0153476851366831' iyz='-0.00319215869825882' izz='0.0044071430916942'/>"
"</inertial>"
"<visual>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link3.stl'/>"
"</geometry>"
"<material name='color_j3'/>"
"</visual>"
"<collision>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link3_c2.stl'/>"
"</geometry>"
"</collision>"
"</link>"
"<!-- joint between lwr_3_link and lwr_4_link -->"
"<joint name='joint_3' type='revolute'>"
"<parent link='link_3'/>"
"<child link='link_4'/>"
"<limit effort='1000.0' lower='-2.09439510239' upper='2.09439510239' velocity='1.96349540849'/>"
"<dynamics damping='0.15' friction='1.0'/>"
"<origin rpy='0 0 0' xyz='0 0 0.2085'/>"
"<axis xyz='0 1 0'/>"
"</joint>"
"<link name='link_4'>"
"<inertial>"
"<origin rpy='0 0 0' xyz='1.12239473548659E-07 0.0327442387470235 0.073658815701594'/>"
"<mass value='2.30343586527606'/>"
"<inertia ixx='0.0142336552604204' ixy='-5.89296043886227E-08' ixz='-1.568273589226E-07' iyy='0.0141315528954361' iyz='-0.00228056254422505' izz='0.00424816761410708'/>"
"</inertial>"
"<visual>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link4.stl'/>"
"</geometry>"
"<material name='color_j4'/>"
"</visual>"
"<collision>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link4_c2.stl'/>"
"</geometry>"
"</collision>"
"</link>"
"<!-- joint between lwr_4_link and lwr_5_link -->"
"<joint name='joint_4' type='revolute'>"
"<parent link='link_4'/>"
"<child link='link_5'/>"
"<limit effort='1000.0' lower='-2.96705972839' upper='2.96705972839' velocity='3.14159265359'/>"
"<dynamics damping='0.12' friction='0.4'/>"
"<origin rpy='0 0 0' xyz='0 0.0 0.1915'/>"
"<axis xyz='0 0 1'/>"
"</joint>"
"<link name='link_5'>"
"<inertial>"
"<origin rpy='0 0 0' xyz='-6.00824789920296E-07 0.0207751869661564 0.0862053948486382'/>"
"<mass value='1.60059828363332'/>"
"<inertia ixx='0.00880806620496216' ixy='1.22820321842462E-07' ixz='-5.66844221164893E-08' iyy='0.00813520145401624' iyz='0.00261443543508601' izz='0.00359712267754715'/>"
"</inertial>"
"<visual>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link5.stl'/>"
"</geometry>"
"<material name='color_j5'/>"
"</visual>"
"<collision>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link5_c2.stl'/>"
"</geometry>"
"</collision>"
"</link>"
"<!-- joint between lwr_5_link and lwr_6_link -->"
"<joint name='joint_5' type='revolute'>"
"<parent link='link_5'/>"
"<child link='link_6'/>"
"<limit effort='300.0' lower='-2.09439510239' upper='2.09439510239' velocity='1.96349540849'/>"
"<dynamics damping='0.1' friction='0.1'/>"
"<origin rpy='0 0 0' xyz='0 0 0.1985'/>"
"<axis xyz='0 -1 0'/>"
"</joint>"
"<link name='link_6'>"
"<inertial>"
"<origin rpy='0 0 0' xyz='-2.64519244286276E-08 -0.00451753627467652 -0.00295324741635017'/>"
"<mass value='1.49302436988808'/>"
"<inertia ixx='0.0298541138330797' ixy='-3.97658663154265E-09' ixz='-1.71667243685877E-09' iyy='0.0299834927882566' iyz='-2.53647350791604E-05' izz='0.0323627047307316'/>"
"</inertial>"
"<visual>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link6.stl'/>"
"</geometry>"
"<material name='color_j6'/>"
"</visual>"
"<collision>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link6_c2.stl'/>"
"</geometry>"
"</collision>"
"</link>"
"<!-- joint between lwr_6_link and lwr_7_link -->"
"<joint name='joint_6' type='revolute'>"
"<parent link='link_6'/>"
"<child link='link_7'/>"
"<limit effort='300.0' lower='-2.96705972839' upper='2.96705972839' velocity='1.96349540849'/>"
"<dynamics damping='0.1' friction='0.1'/>"
"<origin rpy='0 0 0' xyz='0 0 0.078'/>"
"<axis xyz='0 0 1'/>"
"</joint>"
"<link name='link_7'>"
"<inertial>"
"<origin rpy='0 0 0' xyz='2.77555756156289E-17 1.11022302462516E-16 -0.015814675599801'/>"
"<mass value='0.108688241139613'/>"
"<inertia ixx='4.17908737998876E-02' ixy='0' ixz='0' iyy='4.17908737998876E-02' iyz='0' izz='7.00756879151782E-02'/>"
"</inertial>"
"<visual>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link7.stl'/>"
"</geometry>"
"<material name='color_j7'/>"
"</visual>"
"<collision>"
"<origin rpy='0 0 3.14159265359' xyz='0 0 0'/>"
"<geometry>"
"<mesh filename='package://lwr_description/meshes/lwr/link7_c2.stl'/>"
"</geometry>"
"</collision>"
"</link>"
"<!-- The state publisher is done by the RTTStatePublisher() as on the real HW -->"
"<!--gazebo>"
"<plugin name='gz_state_pub' filename='libgazebo_ros_joint_state_publisher.so'>"
"<jointName>joint_0,joint_1,joint_2,joint_3,joint_4,joint_5,joint_6</jointName>"
"<updateRate>100</updateRate>"
"<alwaysOn>true</alwaysOn>"
"</plugin>"
"</gazebo-->"
"<gazebo reference='base_link'>"
"<material>Gazebo/Grey</material>"
"</gazebo>"
"<gazebo reference='joint_0'>"
"<provideFeedback>true</provideFeedback>"
"<!-- Dangerous option :) -->"
"<!--implicitSpringDamper>true</implicitSpringDamper-->"
"<!--stopCfm>0.9</stopCfm-->"
"<!--stopErp>0.5</stopErp-->"
"</gazebo>"
"<gazebo reference='joint_1'>"
"<provideFeedback>true</provideFeedback>"
"<!-- Dangerous option :) -->"
"<!--implicitSpringDamper>true</implicitSpringDamper-->"
"<!--stopCfm>0.9</stopCfm-->"
"<!--stopErp>0.5</stopErp-->"
"</gazebo>"
"<gazebo reference='joint_2'>"
"<provideFeedback>true</provideFeedback>"
"<!-- Dangerous option :) -->"
"<!--implicitSpringDamper>true</implicitSpringDamper-->"
"<!--stopCfm>0.9</stopCfm-->"
"<!--stopErp>0.5</stopErp-->"
"</gazebo>"
"<gazebo reference='joint_3'>"
"<provideFeedback>true</provideFeedback>"
"<!-- Dangerous option :) -->"
"<!--implicitSpringDamper>true</implicitSpringDamper-->"
"<!--stopCfm>0.9</stopCfm-->"
"<!--stopErp>0.5</stopErp-->"
"</gazebo>"
"<gazebo reference='joint_4'>"
"<provideFeedback>true</provideFeedback>"
"<!-- Dangerous option :) -->"
"<!--implicitSpringDamper>true</implicitSpringDamper-->"
"<!--stopCfm>0.9</stopCfm-->"
"<!--stopErp>0.5</stopErp-->"
"</gazebo>"
"<gazebo reference='joint_5'>"
"<provideFeedback>true</provideFeedback>"
"<!-- Dangerous option :) -->"
"<!--implicitSpringDamper>true</implicitSpringDamper-->"
"<!--stopCfm>0.9</stopCfm-->"
"<!--stopErp>0.5</stopErp-->"
"</gazebo>"
"<gazebo reference='joint_6'>"
"<provideFeedback>true</provideFeedback>"
"<!-- Dangerous option :) -->"
"<!--implicitSpringDamper>true</implicitSpringDamper-->"
"<!--stopCfm>0.9</stopCfm-->"
"<!--stopErp>0.5</stopErp-->"
"</gazebo>"
"<gazebo reference='table_link'>"
"<self_collide>true</self_collide>"
"</gazebo>"
"<!-- <xacro:enable_self_collision link_name='screwdriver_link'/>"
"<xacro:enable_self_collision link_name='ati_link'/>"
"<xacro:enable_self_collision link_name='link_7'/>"
"<xacro:enable_self_collision link_name='head_cam_link'/>"
"<xacro:enable_self_collision link_name='handle_link'/> -->"
"<!-- If you want to add a Joint Torque sensor at each Joint -->"
"<!--xacro:gz_ft_sensor joint_name='joint_0'/-->"
"<!--xacro:gz_ft_sensor joint_name='joint_1'/-->"
"<!--xacro:gz_ft_sensor joint_name='joint_2'/-->"
"<!--xacro:gz_ft_sensor joint_name='joint_3'/-->"
"<!--xacro:gz_ft_sensor joint_name='joint_4'/-->"
"<!--xacro:gz_ft_sensor joint_name='joint_5'/-->"
"<!--xacro:gz_ft_sensor joint_name='joint_6'/-->"
"<gazebo reference='link_1'>"
"<material>Gazebo/Orange</material>"
"</gazebo>"
"<gazebo reference='link_2'>"
"<material>Gazebo/Orange</material>"
"</gazebo>"
"<gazebo reference='link_3'>"
"<material>Gazebo/Orange</material>"
"</gazebo>"
"<gazebo reference='link_4'>"
"<material>Gazebo/Orange</material>"
"</gazebo>"
"<gazebo reference='link_5'>"
"<material>Gazebo/Orange</material>"
"</gazebo>"
"<gazebo reference='link_6'>"
"<material>Gazebo/Grey</material>"
"</gazebo>"
"<gazebo reference='link_7'>"
"<material>Gazebo/DarkGrey</material>"
"</gazebo>"
"<transmission name='trans_0'>"
"<type>transmission_interface/SimpleTransmission</type>"
"<joint name='joint_0'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"</joint>"
"<actuator name='motor_0'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"<mechanicalReduction>1</mechanicalReduction>"
"</actuator>"
"</transmission>"
"<transmission name='trans_1'>"
"<type>transmission_interface/SimpleTransmission</type>"
"<joint name='joint_1'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"</joint>"
"<actuator name='motor_1'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"<mechanicalReduction>1</mechanicalReduction>"
"</actuator>"
"</transmission>"
"<transmission name='trans_2'>"
"<type>transmission_interface/SimpleTransmission</type>"
"<joint name='joint_2'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"</joint>"
"<actuator name='motor_2'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"<mechanicalReduction>1</mechanicalReduction>"
"</actuator>"
"</transmission>"
"<transmission name='trans_3'>"
"<type>transmission_interface/SimpleTransmission</type>"
"<joint name='joint_3'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"</joint>"
"<actuator name='motor_3'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"<mechanicalReduction>1</mechanicalReduction>"
"</actuator>"
"</transmission>"
"<transmission name='trans_4'>"
"<type>transmission_interface/SimpleTransmission</type>"
"<joint name='joint_4'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"</joint>"
"<actuator name='motor_4'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"<mechanicalReduction>1</mechanicalReduction>"
"</actuator>"
"</transmission>"
"<transmission name='trans_5'>"
"<type>transmission_interface/SimpleTransmission</type>"
"<joint name='joint_5'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"</joint>"
"<actuator name='motor_5'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"<mechanicalReduction>1</mechanicalReduction>"
"</actuator>"
"</transmission>"
"<transmission name='trans_6'>"
"<type>transmission_interface/SimpleTransmission</type>"
"<joint name='joint_6'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"</joint>"
"<actuator name='motor_6'>"
"<hardwareInterface>EffortJointInterface</hardwareInterface>"
"<hardwareInterface>PositionJointInterface</hardwareInterface>"
"<mechanicalReduction>1</mechanicalReduction>"
"</actuator>"
"</transmission>"
"</robot>";


int main(int argc, char const *argv[])
{
    auto robot_model = std::make_shared<RobotModel>();
    robot->loadModelFromString(urdf_string);
    robot->setBaseFrame("base_link");
    robot->setGravity(Eigen::Vector3d(0,0,-9.81));
    RobotState eigState;
    eigState.resize(robot->getNrOfDegreesOfFreedom());
    eigState.jointPos.setZero();
    eigState.jointVel.setZero();
    robot->setRobotState(eigState.jointPos,eigState.jointVel);
    orca::optim::Controller controller(
        "controller"
        ,robot
        ,orca::optim::ResolutionStrategy::OneLevelWeighted
        ,QPSolverImplType::qpoases
    );
    auto cart_task = std::make_shared<CartesianTask>("CartTask-EE");
    controller.addTask(cart_task);
    cart_task->setControlFrame("link_7"); //
    Eigen::Affine3d cart_pos_ref;
    cart_pos_ref.translation() = Eigen::Vector3d(1.,0.75,0.5); // x,y,z in meters
    cart_pos_ref.linear() = Eigen::Quaterniond::Identity().toRotationMatrix();
    Vector6d cart_vel_ref = Vector6d::Zero();
    Vector6d cart_acc_ref = Vector6d::Zero();
    Vector6d P;
    P << 1000, 1000, 1000, 10, 10, 10;
    cart_task->servoController()->pid()->setProportionalGain(P);
    Vector6d D;
    D << 100, 100, 100, 1, 1, 1;
    cart_task->servoController()->pid()->setDerivativeGain(D);
    cart_task->servoController()->setDesired(cart_pos_ref.matrix(),cart_vel_ref,cart_acc_ref);
    const int ndof = robot->getNrOfDegreesOfFreedom();
    auto jnt_trq_cstr = std::make_shared<JointTorqueLimitConstraint>("JointTorqueLimit");
    controller.addConstraint(jnt_trq_cstr);
    Eigen::VectorXd jntTrqMax(ndof);
    jntTrqMax.setConstant(200.0);
    jnt_trq_cstr->setLimits(-jntTrqMax,jntTrqMax);
    auto jnt_pos_cstr = std::make_shared<JointPositionLimitConstraint>("JointPositionLimit");
    controller.addConstraint(jnt_pos_cstr);
    auto jnt_vel_cstr = std::make_shared<JointVelocityLimitConstraint>("JointVelocityLimit");
    controller.addConstraint(jnt_vel_cstr);
    Eigen::VectorXd jntVelMax(ndof);
    jntVelMax.setConstant(2.0);
    jnt_vel_cstr->setLimits(-jntVelMax,jntVelMax);
    controller.activateTasksAndConstraints();
    robot->setRobotState(eigState.jointPos,eigState.jointVel);
    controller.update(0, 0.1);

    std::cout << "\n\nORCA installation was a success. You may now orca yourself." << '\n';
    return 0;
}
