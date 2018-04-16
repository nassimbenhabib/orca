#include "orca/constraint/JointVelocityLimitConstraint.h"


using namespace orca::constraint;
using namespace orca::optim;
using namespace orca::robot;
using namespace orca::common;

JointVelocityLimitConstraint::JointVelocityLimitConstraint()
: JointLimitConstraint(ControlVariable::JointSpaceAcceleration)
{}

void JointVelocityLimitConstraint::setHorizon(double horizon)
{
    MutexLock lock(this->mutex);

    horizon_ = horizon;
}

void JointVelocityLimitConstraint::updateConstraintFunction()
{
    MutexLock lock(this->mutex);

    const Eigen::VectorXd& min_jnt_vel_(min_);
    const Eigen::VectorXd& max_jnt_vel_(max_);

    const Eigen::VectorXd& current_jnt_vel = robot()->getJointVel();

    constraintFunction().lowerBound().noalias() = ( min_jnt_vel_ - current_jnt_vel ) / horizon_ ;
    constraintFunction().upperBound().noalias() = ( max_jnt_vel_ - current_jnt_vel ) / horizon_ ;
}

void JointVelocityLimitConstraint::resize()
{
    MutexLock lock(this->mutex);

    JointLimitConstraint::resize();
}
