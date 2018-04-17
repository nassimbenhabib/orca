#include "orca/common/TaskBase.h"
#include "orca/optim/ControlProblem.h"

using namespace orca::common;
using namespace orca::optim;
using namespace orca::robot;

TaskBase::TaskBase(ControlVariable control_var)
: control_var_(control_var)
{
}

TaskBase::~TaskBase()
{

}


// void TaskBase::setRobotState(const Eigen::Matrix4d& world_H_base
//                 , const Eigen::VectorXd& jointPos
//                 , const Eigen::Matrix<double,6,1>& baseVel
//                 , const Eigen::VectorXd& jointVel
//                 , const Eigen::Vector3d& gravity)
// {
//     MutexLock lock(this->mutex);
// 
//     robot_->setRobotState(world_H_base,jointPos,baseVel,jointVel,gravity);
// }

void TaskBase::setRobotModel(std::shared_ptr<RobotDynTree> robot)
{
    MutexLock lock(this->mutex);

    if(robot)
    {
        if(robot->getNrOfDegreesOfFreedom() <= 0)
        {
            throw std::runtime_error("Robot does not seem to have any DOF");
        }

        robot_ = robot;
        if(problem_)
            this->resize();
    }
    else
    {
        throw std::runtime_error("Robot pointer is null");
    }
}

bool TaskBase::loadRobotModel(const std::string& file_url)
{
    MutexLock lock(this->mutex);

    if(!robot_->loadModelFromFile(file_url))
    {
        throw std::runtime_error("Could not load robot model");
    }
    this->resize();
    return true;
}

ControlVariable TaskBase::getControlVariable() const
{
    return control_var_;
}

void TaskBase::setName(const std::string& name)
{
    MutexLock lock(this->mutex);

    name_ = name;
}

const std::string& TaskBase::getName() const
{
    MutexLock lock(this->mutex);

    return name_;
}

std::shared_ptr<RobotDynTree> TaskBase::robot()
{
    return robot_;
}

bool TaskBase::activate()
{
    MutexLock lock(this->mutex);

    if(is_activated_)
    {
        LOG_WARNING << "[" << TaskBase::getName() << "] " << "Already activated";
        return true;
    }
    else
    {
        LOG_INFO << "[" << TaskBase::getName() << "] " << "Activating";
        is_activated_ = true;
        return true;
    }
    return false;
}

bool TaskBase::desactivate()
{
    MutexLock lock(this->mutex);

    if(!is_activated_)
    {
        LOG_ERROR << "[" << TaskBase::getName() << "] " << "Contact already desactivated ";
    }
    else
    {
        LOG_INFO << "[" << TaskBase::getName() << "] " << "Desactivating";
        is_activated_ = false;
    }
    return true;
}

bool TaskBase::isActivated() const
{
    MutexLock lock(this->mutex);

    return is_activated_;
}

bool TaskBase::insertInProblem()
{
    if(hasProblem())
    {
        LOG_ERROR << "[" << TaskBase::getName() << "] " << "Inserting task in problem";
        return problem_->add(this);
    }

    throw std::runtime_error("Problem is not set");
}

bool TaskBase::removeFromProblem()
{
    if(hasProblem())
        return problem_->remove(this);
    return false;
}


bool TaskBase::setProblem(std::shared_ptr<Problem> problem, bool insert)
{
    MutexLock lock(this->mutex);
    
    if(!problem)
    {
        throw std::runtime_error(util::Formatter() << "[" << TaskBase::getName() << "] "<< "Problem is null");
    }
    
    if(hasProblem())
    {
        LOG_ERROR << "[" << TaskBase::getName() << "] " << "Problem is already set, NOT overriding";
        return false;
    }
    this->problem_ = problem;
    if(robot_)
        this->resize();
    if(insert)
        problem_->add(this);
    return true;
}

std::shared_ptr<Problem> TaskBase::problem()
{
    return problem_;
}

void TaskBase::printStateIfErrors() const
{
    if(!hasRobot())
    {
        LOG_ERROR << "[" << TaskBase::getName() << "] " << "Robot is not loaded";
    }
    
    if(!robot_->isInitialized())
    {
        LOG_ERROR << "[" << TaskBase::getName() << "] " << "Robot is not initialised (first state not set)";
    }
    
    if(!hasProblem())
    {
        LOG_ERROR << "[" << TaskBase::getName() << "] " << "Problem is not set";
    }
}

bool TaskBase::isInitialized() const
{
    MutexLock lock(this->mutex);
    
    return hasProblem() && hasRobot() && robot_->isInitialized();
}

bool TaskBase::hasProblem() const
{
    return static_cast<bool>(problem_);
}

bool TaskBase::hasRobot() const
{
    return static_cast<bool>(robot_);
}
