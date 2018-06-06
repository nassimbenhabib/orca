#include "orca/common/TaskBase.h"
#include "orca/utils/Utils.h"

using namespace orca::common;
using namespace orca::optim;
using namespace orca::robot;
using namespace orca::utils;

#define assertRobotLoaded(robot) \
    if(!robot) \
        orca_throw(Formatter() << "[" << TaskBase::getName() << "] Robot is not set"); \
    if(robot->getNrOfDegreesOfFreedom() <= 0) \
        orca_throw(Formatter() << "[" << TaskBase::getName() << "] Robot pointer is valid, but does not seem to have any DOF. Did you loadModelFromURDF() ?");

#define assertRobotInitialized(robot) \
    assertRobotLoaded(robot); \
    if(!robot->isInitialized()) \
        orca_throw(Formatter() << "[" << TaskBase::getName() << "] Robot is not initialized. Initialize it by setting at least one state (robot->setRobotState())");


TaskBase::TaskBase(const std::string& name,ControlVariable control_var)
: control_var_(control_var)
, name_(name)
{}

TaskBase::~TaskBase()
{}

void TaskBase::link(std::shared_ptr<TaskBase> e)
{
    if(std::find(linked_elements_.begin(),linked_elements_.end(),e) == linked_elements_.end())
    {
        linked_elements_.push_back(e);
    }
    else
        LOG_ERROR << "Cannot link task " << e->getName() << " because it already exists";
}

bool TaskBase::isActivated() const
{
    return state_ == Activated;
}

bool TaskBase::isComputing() const
{
    return state_ == Activating || state_ == Activated || state_ == Deactivating;
}

void TaskBase::print() const
{
    std::cout << "[" << TaskBase::getName() << "]" << '\n';
    std::cout << " - Control variable   " << getControlVariable() << '\n';
    std::cout << " - Current state " << getState() << '\n';
    std::cout << " - hasProblem         " << hasProblem() << '\n';
    std::cout << " - hasRobot           " << hasRobot() << '\n';
    std::cout << " - hasWrench          " << hasWrench() << '\n';
    std::cout << " - isRobotInitialized " << isRobotInitialized() << '\n';
}

bool TaskBase::isRobotInitialized() const
{
    return hasRobot() && robot_->isInitialized();
}

void TaskBase::setRampValue(double new_val)
{
    ramp_value_ = std::min(1.,std::max(0.,new_val));
}

double TaskBase::getCurrentRampValue() const
{
    return ramp_value_;
}

void TaskBase::setRampDuration(double ramp_duration)
{
    if(ramp_duration == 0)
        ramp_value_ = 1;
    if(ramp_duration < 0)
    {
        LOG_ERROR << "[" << TaskBase::getName() << "] " << "Ramp duration must be > 0";
        return;
    }
    ramp_duration_ = ramp_duration;
}

double TaskBase::getRampDuration() const
{
    return ramp_duration_;
}

double TaskBase::getStartTime() const
{
    return start_time_;
}

double TaskBase::getStopTime() const
{
    return stop_time_;
}

TaskBase::State TaskBase::getState() const
{
    return state_;
}

void TaskBase::setRobotModel(std::shared_ptr<RobotDynTree> robot)
{
    assertRobotLoaded(robot);

    // Check if we already have a robot
    if(robot_)
    {
        LOG_WARNING << "[" << TaskBase::getName() << "] " << "Replacing existing robot";
    }
    // Copy the new robot model
    robot_ = robot;

    // Create the wrench if you depend on ExternalWrench
    if(getControlVariable() == ControlVariable::ExternalWrench)
    {
        wrench_ = std::make_shared<Wrench>(name_ + "_wrench");
        wrench_->setRobotModel(robot_);
    }

    for(auto e : linked_elements_)
        e->setRobotModel(robot_);

    if(hasProblem())
    {
        resize();
    }
}

bool TaskBase::dependsOnProblem() const
{
    return control_var_ == ControlVariable::ExternalWrench
        || control_var_ == ControlVariable::ExternalWrenches
        || control_var_ == ControlVariable::X;
}

bool TaskBase::dependsOnRobotJoints() const
{
    return control_var_ == ControlVariable::JointAcceleration
        || control_var_ == ControlVariable::JointTorque
        || control_var_ == ControlVariable::X;
}

bool TaskBase::dependsOnFloatingBase() const
{
    return control_var_ == ControlVariable::GeneralisedAcceleration
        || control_var_ == ControlVariable::GeneralisedTorque
        || control_var_ == ControlVariable::FloatingBaseAcceleration
        || control_var_ == ControlVariable::FloatingBaseWrench
        || control_var_ == ControlVariable::X;
}

bool TaskBase::rampUp(double time_since_start)
{
    return time_since_start >= ramp_duration_;
}

bool TaskBase::rampDown(double time_since_stop)
{
    return time_since_stop >= ramp_duration_;
}

void TaskBase::resize()
{
    LOG_INFO << "[" << TaskBase::getName() << "] Resizing";
    // Calling the user callback
    // NOTE: the need to resize the task is handled in the the user callback
    // i.e verify if new_size != current_size, which is specific to said task

    for(auto e : linked_elements_)
        e->resize();

    this->onResize();

    switch (state_)
    {
        case Init:
            state_ = Resized;
            onResized();
            if(on_resized_cb_)
                on_resized_cb_();
            break;
        default:
            // NOTE: If the task is running, then just call instantaneous resize
            // and do not change the state.
            // Otherwise you would have to activate() again the task
            // which is not what user expect
            break;
    }
    LOG_INFO << "[" << TaskBase::getName() << "] Resizing done";
}

ControlVariable TaskBase::getControlVariable() const
{
    return control_var_;
}

const std::string& TaskBase::getName() const
{
    return name_;
}

std::shared_ptr<RobotDynTree> TaskBase::robot()
{
    assertRobotLoaded(robot_);
    return robot_;
}

std::shared_ptr<const RobotDynTree> TaskBase::getRobot() const
{
    assertRobotLoaded(robot_);
    return robot_;
}

std::shared_ptr< Wrench > TaskBase::wrench()
{
    if(!wrench_)
    {
        orca_throw(Formatter() << "[" << TaskBase::getName() << "] "
            << "Wrench is not set, this happens when the task does not depend on ExternalWrench\n"
            << "This task control variabe is " << getControlVariable());
    }
    return wrench_;
}

bool TaskBase::activate()
{
    assertRobotLoaded(robot_);

    if(state_ == Resized || state_ == Deactivated)
    {
        state_ = Activating;
        LOG_INFO << "[" << TaskBase::getName() << "] Activation requested";

        this->activation_requested_ = true;

        if(hasWrench())
            wrench_->activate();

        for(auto t : linked_elements_)
            t->activate();

        return true;
    }
    else
    {
        LOG_ERROR << "[" << TaskBase::getName() << "] " << "Could not activate because state is " << state_;
        return false;
    }
}

void TaskBase::update(double current_time, double dt)
{
    for(auto t : linked_elements_)
        t->update(current_time,dt);

    switch (state_)
    {
        case Init:
            orca_throw(Formatter() << "[" << TaskBase::getName() << "] "
                << "Calling update, but the task state is Init.\n"
                << "Please insert the task in the controller or set the robot model + "
                << "set the problem to trigger a resize(). Then you'll be able to update the task.");
        case Resized:
            break;
        case Deactivated:
            break;
        case Activating:
        case Activated:
        case Deactivating:
        {
            if(hasWrench())
                wrench_->update(current_time,dt);

            if(state_ == Activating)
            {
                if(this->activation_requested_)
                {
                    LOG_INFO << "[" << TaskBase::getName() << "] " << state_;
                    onActivation();
                    if(on_activation_cb_)
                        on_activation_cb_();
                    this->activation_requested_ = false;
                    start_time_ = current_time;
                }

                if(this->rampUp(current_time - start_time_))
                {
                    state_ = Activated;

                    if(this->getRampDuration() > 0)
                        LOG_DEBUG << "[" << TaskBase::getName() << "] " << "Ramping up is done, state is now " << state_;
                    else
                        LOG_DEBUG << "[" << TaskBase::getName() << "] " << state_;

                    onActivated();
                    if(on_activated_cb_)
                        on_activated_cb_();
                }
            }

            if(state_ == Deactivating)
            {
                if(this->deactivation_requested_)
                {
                    LOG_DEBUG << "[" << TaskBase::getName() << "] " << state_;
                    onDeactivation();
                    if(on_deactivation_cb_)
                        on_deactivation_cb_();
                    this->deactivation_requested_ = false;
                    stop_time_ = current_time;
                }

                if(this->rampDown(current_time - stop_time_))
                {
                    state_ = Deactivated;

                    if(this->getRampDuration() > 0)
                        LOG_DEBUG << "[" << TaskBase::getName() << "] " << "Ramping down is done, state is now " << state_;
                    else
                        LOG_DEBUG << "[" << TaskBase::getName() << "] " << state_;

                    onDeactivated();
                    if(on_deactivated_cb_)
                        on_deactivated_cb_();

                    break;
                }
            }

            if(on_update_begin_cb_)
                on_update_begin_cb_(current_time,dt);

            onCompute(current_time, dt);

            if(on_update_end_cb_)
                on_update_end_cb_(current_time,dt);

            break;
        }
        default:
            //LOG_ERROR << "[" << TaskBase::getName() << "] " << "Should not be calling update when state is " << state_;
            break;
    }
}

void TaskBase::onActivationCallback(std::function<void(void)> cb)
{
    LOG_DEBUG << "[" << TaskBase::getName() << "] " << "Registering onActivation callback";
    this->on_activation_cb_ = cb;
}

void TaskBase::onActivatedCallback(std::function<void ()> cb)
{
    LOG_DEBUG << "[" << TaskBase::getName() << "] " << "Registering onActivated callback";
    this->on_activated_cb_ = cb;
}

void TaskBase::onComputeBeginCallback(std::function<void(double,double)> cb)
{
    LOG_DEBUG << "[" << TaskBase::getName() << "] " << "Registering onUpdateBegin callback";
    this->on_update_begin_cb_ = cb;
}

void TaskBase::onComputeEndCallback(std::function<void(double,double)> cb)
{
    LOG_DEBUG << "[" << TaskBase::getName() << "] " << "Registering onUpdateEnd callback";
    this->on_update_end_cb_ = cb;
}

void TaskBase::onDeactivationCallback(std::function<void(void)> cb)
{
    LOG_DEBUG << "[" << TaskBase::getName() << "] " << "Registering onDeactivation callback";
    this->on_deactivation_cb_ = cb;
}

void TaskBase::onDeactivatedCallback(std::function<void ()> cb)
{
    LOG_DEBUG << "[" << TaskBase::getName() << "] " << "Registering onDeactivated callback";
    this->on_deactivated_cb_ = cb;
}

bool TaskBase::deactivate()
{
    if(state_ == Activating || state_ == Activated)
    {
        state_ = Deactivating;
        LOG_INFO << "[" << TaskBase::getName() << "] Deactivation requested";

        this->deactivation_requested_ = true;

        if(hasWrench())
            wrench_->deactivate();

        for(auto t : linked_elements_)
            t->deactivate();

        return true;
    }
    else
    {
        LOG_ERROR << "[" << TaskBase::getName() << "] "
            << "Could not deactivate because state is " << state_
            << "\nDeactivation is only possible when state is Activating or Activated";
        return false;
    }
}

bool TaskBase::setProblem(std::shared_ptr<const Problem> problem)
{
    if(!problem)
    {
        orca_throw(Formatter() << "[" << TaskBase::getName() << "] "<< "Problem is null");
    }

    if(state_ != Init) {
        orca_throw(Formatter() << "[" << TaskBase::getName() << "] Calling setRobotModel is only valid when state is Init, but now it's " << state_);
    }

    if(hasProblem())
    {
        LOG_WARNING << "[" << TaskBase::getName() << "] " << "Problem is already set";
        return false;
    }
    this->problem_ = problem;

    for(auto e : linked_elements_)
        e->setProblem(problem_);

    if(hasRobot())
    {
        // Resize if we have all the parameters
        resize();
    }
    return true;
}

std::shared_ptr<const Problem> TaskBase::getProblem() const
{
    if(!problem_)
    {
        orca_throw(Formatter() << "[" << TaskBase::getName() << "] "<< "Problem is not set");
    }
    return problem_;
}

void TaskBase::checkIfUpdatable() const
{
    if(!hasRobot())
    {
        orca_throw(Formatter() << "[" << TaskBase::getName() << "] " << "Robot is not loaded");
    }

    if(!robot_->isInitialized())
    {
        orca_throw(Formatter() << "[" << TaskBase::getName() << "] " << "Robot is not initialised (first state not set)");
    }

    if(!hasProblem())
    {
        orca_throw(Formatter() << "[" << TaskBase::getName() << "] " << "Problem is not set");
    }
}

bool TaskBase::hasProblem() const
{
    return static_cast<bool>(problem_);
}

bool TaskBase::hasRobot() const
{
    return static_cast<bool>(robot_);
}

bool TaskBase::hasWrench() const
{
    return static_cast<bool>(wrench_);
}

std::shared_ptr<const Wrench> TaskBase::getWrench() const
{
    if(!wrench_)
    {
        orca_throw(Formatter() << "[" << TaskBase::getName() << "] "
            << "Wrench is not set, this happens when the task does not depend on ExternalWrench\n"
            << "This task control variabe is " << getControlVariable());
    }
    return wrench_;
}
