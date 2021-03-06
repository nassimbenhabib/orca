#include "orca/common/CartesianAccelerationPID.h"

using namespace orca::common;
using namespace orca::optim;

CartesianAccelerationPID::CartesianAccelerationPID(const std::string& name)
: CartesianServoController(name)
{
    this->addParameter("pid",&pid_);
}

const Eigen::Vector3d& CartesianAccelerationPID::getCurrentCartesianPosition() const
{
    return cart_position_curr_;
}

const Eigen::Matrix3d& CartesianAccelerationPID::getCurrentCartesianRotation() const
{
    return cart_rot_curr_;
}

const Eigen::Matrix4d& CartesianAccelerationPID::getCurrentCartesianPose() const
{
    return cart_pos_curr_;
}

const Vector6d& CartesianAccelerationPID::getCurrentCartesianVelocity() const
{
    return cart_vel_curr_;
}

const Eigen::Matrix4d& CartesianAccelerationPID::getDesiredCartesianPose() const
{
    return cart_pos_des_;
}
const Vector6d& CartesianAccelerationPID::getDesiredCartesianVelocity() const
{
    return cart_vel_des_;
}
const Vector6d& CartesianAccelerationPID::getDesiredCartesianAcceleration() const
{
    return cart_acc_des_;
}

void CartesianAccelerationPID::onResize()
{
    if(pid_.isSet())
        pid_.get()->resize(6);
    cart_pos_curr_.setZero();
    cart_pos_des_.setZero();
    cart_acc_cmd_.setZero();
    cart_acc_bias_.setZero();
    cart_acc_des_.setZero();
    cart_vel_des_.setZero();
    cart_pos_err_.setZero();
    cart_vel_err_.setZero();
    cart_vel_curr_.setZero();
}

void CartesianAccelerationPID::print() const
{
    std::cout << "[" << getName() << "]" << '\n';
    std::cout << "  Position desired :\n" << '\n';
    std::cout << cart_pos_des_ << '\n';
    std::cout << "  Velocity desired : " << '\n';
    std::cout << cart_vel_des_<< '\n';
    std::cout << "  Acceleration desired : " << '\n';
    std::cout << cart_acc_des_ << '\n';
    std::cout << "  Position error : " << '\n';
    std::cout << cart_pos_err_.transpose() << '\n';
    std::cout << "  Velocity error : " << '\n';
    std::cout << cart_vel_err_.transpose() << '\n';
    pid_.get()->print();
    std::cout << "  Acceleration command : " << '\n';
    std::cout << cart_acc_cmd_.transpose() << '\n';
}

void CartesianAccelerationPID::setDesired(const Eigen::Matrix4d& cartesian_position_traj
                                        , const Vector6d& cartesian_velocity_traj
                                        , const Vector6d& cartesian_acceleration_traj)
{
    cart_pos_des_ = cartesian_position_traj;
    cart_vel_des_ = cartesian_velocity_traj;
    cart_acc_des_ = cartesian_acceleration_traj;
    desired_set_ = true;
}

PIDController::Ptr CartesianAccelerationPID::pid()
{
    return pid_.get();
}

void CartesianAccelerationPID::onActivation()
{
    // If no frame has been set before, use the default Floating Base.
    if(!getParameter("base_frame")->isSet())
    {
        setBaseFrame(robot()->getBaseFrame());
    }
    
    if(!desired_set_)
    {
        // Defaults the task to the current pose of the robot
        cart_pos_des_ = robot()->getRelativeTransform(getBaseFrame(),getControlFrame());
    }
}

const Vector6d& CartesianAccelerationPID::getCartesianPoseError() const
{
    return cart_pos_err_;
}

const Vector6d& CartesianAccelerationPID::getCartesianVelocityError() const
{
    return cart_vel_err_;
}

void CartesianAccelerationPID::onCompute(double current_time, double dt)
{
    cart_position_curr_ = cart_pos_curr_.block<3,1>(0,2);
    cart_rot_curr_ = cart_pos_curr_.topLeftCorner<3,3>();
    // Compute Cartesian Position Error
    cart_pos_curr_ = robot()->getRelativeTransform(getBaseFrame(),getControlFrame());
    cart_pos_err_ = math::diffTransform(cart_pos_curr_ , cart_pos_des_);

    // Compute Cartesian Velocity Error
    cart_vel_curr_ = robot()->getFrameVel(getControlFrame());
    cart_vel_err_ = cart_vel_des_ - cart_vel_curr_;

    // Compute Cartesian Acceleration Command
    cart_acc_cmd_ = cart_acc_des_ + pid_.get()->computeCommand( cart_pos_err_ , cart_vel_err_ , dt);
}

const Vector6d& CartesianAccelerationPID::getCommand() const
{
    return cart_acc_cmd_;
}

ORCA_REGISTER_CLASS(orca::common::CartesianAccelerationPID)
