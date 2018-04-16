#include "orca/task/JointAccelerationTask.h"


using namespace orca::task;
using namespace orca::optim;
using namespace orca::common;

JointAccelerationTask::JointAccelerationTask()
: GenericTask(ControlVariable::JointSpaceAcceleration)
, pid_(this->mutex)
{

}

void JointAccelerationTask::setDesired(const Eigen::VectorXd& jnt_pos_des
                                    , const Eigen::VectorXd& jnt_vel_des
                                    , const Eigen::VectorXd& jnt_acc_des)
{
    MutexLock lock(this->mutex);

    jnt_pos_des_ = jnt_pos_des;
    jnt_vel_des_ = jnt_vel_des;
    jnt_acc_des_ = jnt_acc_des;
}

PIDController<Eigen::Dynamic>& JointAccelerationTask::pid()
{
    return pid_;
}

void JointAccelerationTask::updateAffineFunction()
{
    const Eigen::VectorXd& current_jnt_pos = robot()->getJointPos();
    const Eigen::VectorXd& current_jnt_vel = robot()->getJointVel();

    f() = - (jnt_acc_des_ + pid_.computeCommand( jnt_pos_des_ - current_jnt_pos , jnt_vel_des_ - current_jnt_vel) );
}

void JointAccelerationTask::resize()
{
    MutexLock lock(this->mutex);

    const unsigned int dof = robot()->getNrOfDegreesOfFreedom();

    if(this->cols() != dof)
    {
        EuclidianNorm().resize(dof,dof);
        E().setIdentity();
        pid_.resize(dof);

        jnt_pos_des_.setZero( dof );
        jnt_vel_des_.setZero( dof );
        jnt_acc_des_.setZero( dof );
    }
}
