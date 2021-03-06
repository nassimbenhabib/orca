#include "orca/task/JointTorqueTask.h"
#include "orca/utils/Utils.h"


using namespace orca::task;
using namespace orca::optim;
using namespace orca::robot;
using namespace orca::common;
using namespace orca::utils;


JointTorqueTask::JointTorqueTask(const std::string& name)
: GenericTask(name,ControlVariable::JointTorque)
{
    this->addParameter("pid",&pid_);
    this->addParameter("feedforward",&feedforward_,ParamPolicy::Optional);
}

void JointTorqueTask::setDesired(const Eigen::VectorXd& desired_joint_torque)
{
    assertSize(desired_joint_torque,current_jnt_trq_);
    jnt_trq_des_ = desired_joint_torque;
    desired_set_ = true;
}

void JointTorqueTask::onActivation()
{
    if(!feedforward_.isSet())
    {
        feedforward_ = Eigen::VectorXd::Zero(robot()->getNrOfDegreesOfFreedom());
    }
}

void JointTorqueTask::onUpdateAffineFunction(double current_time, double dt)
{
    if(!desired_set_)
    {
        LOG_WARNING << "[" << getName() << "] Desired torque has not been set, setting the desired to the gravity torques";
        jnt_trq_des_ = robot()->getJointGravityTorques();
    }

    f() = - getFeedforward() - pid_.get()->computeCommand(current_jnt_trq_ - jnt_trq_des_ , dt);
}

void JointTorqueTask::setCurrent(const Eigen::VectorXd& current_joint_torque)
{
    assertSize(current_joint_torque,current_jnt_trq_);
    current_jnt_trq_ = current_joint_torque;
}

PIDController::Ptr JointTorqueTask::pid()
{
    return pid_.get();
}

void JointTorqueTask::setFeedforward(const Eigen::VectorXd& ff)
{
    feedforward_ = ff;
}
const Eigen::VectorXd& JointTorqueTask::getFeedforward() const
{
    return feedforward_.get();
}

void JointTorqueTask::onResize()
{
    const unsigned int dof = robot()->getNrOfDegreesOfFreedom();

    if(this->cols() != dof)
    {
        euclidianNorm().resize(dof,dof);

        pid_.get()->resize(dof);

        jnt_trq_des_.setZero(dof);
        current_jnt_trq_.setZero(dof);

        E().setIdentity();
    }
}

ORCA_REGISTER_CLASS(orca::task::JointTorqueTask)
