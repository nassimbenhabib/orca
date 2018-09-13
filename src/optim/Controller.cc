#include "orca/optim/Controller.h"
#include "orca/common/Wrench.h"
#include "orca/common/TaskBase.h"
#include "orca/constraint/Contact.h"
#include "orca/task/RegularisationTask.h"
#include "orca/constraint/DynamicsEquationConstraint.h"
#include "orca/common/ParameterSharedPtr.h"

using namespace orca;
using namespace orca::optim;
using namespace orca::utils;
using namespace orca::common;
using namespace orca::robot;

Controller::Controller(const std::string& name)
: ConfigurableOrcaObject(name)
{
    this->addParameter("robot_model",&robot_);
    this->addParameter("resolution_strategy",&resolution_strategy_str_);
    this->addParameter("qpsolver_implementation",&solver_type_str_);
    this->addParameter("remove_gravity_torques_from_solution",&remove_gravity_torques_);
    this->addParameter("remove_coriolis_torques_from_solution",&remove_coriolis_torques_);
    this->config()->onSuccess([&](){

        resolution_strategy_ = ResolutionStrategyfromString(resolution_strategy_str_.get());
        solver_type_ = QPSolverImplTypefromString(solver_type_str_.get());

        joint_acceleration_command_.setZero(robot()->getNrOfDegreesOfFreedom());
        joint_torque_command_.setZero(robot()->getNrOfDegreesOfFreedom());
        kkt_torques_.setZero(robot()->getNrOfDegreesOfFreedom());

        if(resolution_strategy_ != ResolutionStrategy::OneLevelWeighted)
        {
            orca_throw("Only ResolutionStrategy::OneLevelWeighted is supported for now");
        }
        insertNewLevel();
    });
}

// bool Controller::configureFromString(const std::string& yaml_str)
// {
//     if(yaml_str.empty())
//         orca_throw(Formatter() << "Provided yaml file is empty");
//     
//     YAML::Node config;
//     try {
//         config = YAML::Load(yaml_str);
//     } catch(std::exception& e) {
//         orca_throw(Formatter() << e.what() << "\n\n" << yaml_str << "\n\nYaml file does not seem to be valid, usually bad formatting.\n\n");
//     }
//     
//     YAML::Node robot_model_node;
//     YAML::Node controller_node;
//     
//     try {
//         robot_model_node = config["robot_model"];
//     } catch(std::exception& e) {
//         orca_throw(Formatter() << e.what() << "\n\n" << yaml_str << "\n\nYaml file must have a 'robot_model' key\n\n");
//     }
//     
//     try {
//         controller_node = config["controller"];
//     } catch(std::exception& e) {
//         orca_throw(Formatter() << e.what() << "\n\n" << yaml_str << "\n\nYaml file must have a 'controller_node' key\n\n");
//     }
//     
//     auto to_string = [](const YAML::Node& n) -> std::string 
//         { YAML::Emitter out; out << n; return out.c_str(); };
//     
//     if(!config()->loadFromString(to_string(controller_node)))
//         return false;
//     
//     resolution_strategy_ = ResolutionStrategyfromString(resolution_strategy_str_.get());
//     solver_type_ = QPSolverImplTypefromString(solver_type_str_.get());
//     
//     if(!robot()->configureFromString(to_string(robot_model_node)))
//         return false;
//     
//     joint_acceleration_command_.setZero(robot_->getNrOfDegreesOfFreedom());
//     joint_torque_command_.setZero(robot_->getNrOfDegreesOfFreedom());
//     kkt_torques_.setZero(robot_->getNrOfDegreesOfFreedom());
// 
//     if(resolution_strategy_ != ResolutionStrategy::OneLevelWeighted)
//     {
//         orca_throw("Only ResolutionStrategy::OneLevelWeighted is supported for now");
//     }
//     insertNewLevel();
//     
//     return true;
// }

Controller::Controller(const std::string& name
    , std::shared_ptr<robot::RobotModel> robot
    ,ResolutionStrategy resolution_strategy
    ,QPSolverImplType solver_type)
: Controller(name)
{
    resolution_strategy_ = resolution_strategy;
    solver_type_ = solver_type;
    robot_ = robot;
    
    joint_acceleration_command_.setZero(robot->getNrOfDegreesOfFreedom());
    joint_torque_command_.setZero(robot->getNrOfDegreesOfFreedom());
    kkt_torques_.setZero(robot->getNrOfDegreesOfFreedom());

    if(resolution_strategy != ResolutionStrategy::OneLevelWeighted)
    {
        orca_throw("Only ResolutionStrategy::OneLevelWeighted is supported for now");
    }
    insertNewLevel();
}

void Controller::print() const
{
    for(auto problem : problems_)
    {
        problem->print();
    }
}

void Controller::setPrintLevel(int level)
{
    for(auto problem : problems_)
    {
        problem->qpSolver()->setPrintLevel(level);
    }
}

const std::string& Controller::getName() const
{
    return name_.get();
}

std::shared_ptr<robot::RobotModel> Controller::robot()
{
    if(!robot_.get())
        orca_throw(Formatter() << "Robot is not set");
    return robot_.get();
}

void Controller::setRobotModel(std::shared_ptr<robot::RobotModel> robot)
{
    robot_ = robot;
}

void Controller::setUpdateCallback(std::function<void(double,double)> update_cb)
{
    this->update_cb_ = update_cb;
}

bool Controller::isProblemDry(std::shared_ptr<const optim::Problem> problem)
{
    // Returns true if there is only one task computing something,
    // and it's the GlobalRegularisation
    int ntasks_computing = 0;
    int task_index = 0;
    for(auto t : problem->getTasks())
    {
        if(t->isComputing())
            ntasks_computing++;

        if(ntasks_computing > 1)
            return false;

        task_index++;
    }
    // Here ntasks_computing == 1
    for(auto t : problem->getTasks())
    {
        if(t->isComputing() || t->getName() == "GlobalRegularisation")
            return true;
    }
    return false;
}

bool Controller::update(double current_time, double dt)
{
    MutexLock lock(mutex);
    solution_found_ = false;

    switch (resolution_strategy_)
    {
        case ResolutionStrategy::OneLevelWeighted:
        {
            updateTasks(current_time,dt);
            updateConstraints(current_time,dt);
            auto problem = getProblemAtLevel(0);
            problem->build();
            solution_found_ = problem->solve();

            if(this->update_cb_)
                this->update_cb_(current_time,dt);

            return solution_found_;
        }
        default:
            orca_throw(Formatter() << "unsupported resolution strategy");
    }
}

bool Controller::solutionFound() const
{
    return solution_found_;
}

common::ReturnCode Controller::getReturnCode() const
{
    switch (resolution_strategy_)
    {
        case ResolutionStrategy::OneLevelWeighted:
            return problems_.front()->getReturnCode();
        default:
            orca_throw(Formatter() << "unsupported resolution strategy");
    }
}

bool Controller::addTask(std::shared_ptr<task::GenericTask> task)
{
    if(resolution_strategy_ == ResolutionStrategy::OneLevelWeighted)
    {
        if(!problems_.front()->taskExists(task))
        {
            task->setRobotModel(robot());
            task->setProblem(problems_.front());
            return problems_.front()->addTask(task);
        }
        else
        {
            LOG_WARNING << "Task " << task->getName() << " already exists.";
            return true;
        }
    }
    return false;
}

std::shared_ptr<task::GenericTask> Controller::getTask(const std::string& name, int level)
{
    auto problem = getProblemAtLevel(level);
    for(auto t : problem->getTasks())
        if(t->getName() == name)
            return t;
    orca_throw(Formatter() << "Task " << name << " does not exist at level " << level);
}

std::shared_ptr<task::GenericTask> Controller::getTask(unsigned int index, int level)
{
    auto problem = getProblemAtLevel(level);
    if(problem->getTasks().size() < index)
        orca_throw(Formatter() <<"Problem at level " << level << " only have " << problem->getTasks().size() << "tasks, cannot retrieve task index " << index);

    auto it = problem->getTasks().begin();
    std::advance(it, level);
    return *it;
}

bool Controller::addConstraint(std::shared_ptr<constraint::GenericConstraint> cstr)
{
    if(resolution_strategy_ == ResolutionStrategy::OneLevelWeighted)
    {
        if(!problems_.front()->constraintExists(cstr))
        {
            cstr->setRobotModel(robot_.get());
            cstr->setProblem(problems_.front());
            return problems_.front()->addConstraint(cstr);
        }
        else
        {
            LOG_WARNING << "Constraint " << cstr->getName() << " already exists.";
            return true;
        }

    }
    return false;
}

const Eigen::VectorXd& Controller::getSolution()
{
    switch (resolution_strategy_)
    {
        case ResolutionStrategy::OneLevelWeighted:
            return problems_.front()->getSolution();
        default:
            orca_throw(Formatter() << "Unsupported resolution strategy");
    }
}

const Eigen::VectorXd& Controller::getJointTorqueCommand(bool remove_gravity_torques /*= false*/
                                                    , bool remove_coriolis_torques /*= false*/)
{
    switch (resolution_strategy_)
    {
        case ResolutionStrategy::OneLevelWeighted:
        {
            auto problem = problems_.front();

            static bool print_warning = true;
            if(solution_found_ && isProblemDry(problem) && print_warning)
            {
                print_warning = false;
                LOG_WARNING << "\n\n"
                    <<" Solution found but the problem is dry !\n"
                    << "It means that an optimal solution is found but the problem \n"
                    << "only has one task computing anything, ans it's the"
                    << "GlobalRegularisation task (This will only be printed once)\n\n"
                    << "/!\\ Resulting torques will cause the robot to fall /!\\";
            }

            if(!solutionFound())
                orca_throw(Formatter() << "Cannot return JointTorqueCommand as the problem is not solved."
                    <<"Use controller.solutionFound() to check if the controller computed a valid solution");

            joint_torque_command_ = problem->getSolution(ControlVariable::JointTorque);

            if(remove_gravity_torques || remove_gravity_torques_.get())
                joint_torque_command_ -= robot()->getJointGravityTorques();

            if(remove_coriolis_torques || remove_coriolis_torques_.get())
                joint_torque_command_ -= robot()->getJointCoriolisTorques();

            return joint_torque_command_;
        }
        default:
            orca_throw(Formatter() << "Unsupported resolution strategy");
    }
}

const Eigen::VectorXd& Controller::computeKKTTorques()
{
    orca_throw(Formatter() << "computeKKTTorques() is not yet implemented");
    return kkt_torques_;
}

const Eigen::VectorXd& Controller::getJointAccelerationCommand()
{
    switch (resolution_strategy_)
    {
        case ResolutionStrategy::OneLevelWeighted:
            joint_acceleration_command_ = problems_.front()->getSolution(ControlVariable::JointAcceleration);
            return joint_acceleration_command_;
        default:
            orca_throw(Formatter() << "Unsupported resolution strategy");
    }
}

void Controller::activateTasksAndConstraints()
{
    activateTasks();
    activateConstraints();
}

void Controller::deactivateTasksAndConstraints()
{
    deactivateTasks();
    deactivateConstraints();
}

void Controller::deactivateTasks()
{
    for(auto problem : problems_)
    {
        for(auto t : problem->getTasks())
        {
            if(t->getName() == "GlobalRegularisation")
                continue;
            t->deactivate();
        }
    }
}

void Controller::deactivateConstraints()
{
    for(auto problem : problems_)
    {
        for(auto c : problem->getConstraints())
        {
            if(c->getName() == "DynamicsEquation")
                continue;
            c->deactivate();
        }
    }
}

void Controller::activateTasks()
{
    for(auto problem : problems_)
    {
        for(auto t : problem->getTasks())
        {
            t->activate();
        }
    }
}

void Controller::activateConstraints()
{
    for(auto problem : problems_)
    {
        for(auto c : problem->getConstraints())
        {
            c->activate();
        }
    }
}

bool Controller::tasksAndConstraintsDeactivated()
{
    for(auto problem : problems_)
    {
        for(auto t : problem->getTasks())
        {
            if(t->getName() == "GlobalRegularisation")
                continue;
            if(t->getState() != common::TaskBase::Deactivated)
                return false;
        }
        for(auto c : problem->getConstraints())
        {
            if(c->getName() == "DynamicsEquation")
                continue;
            if(c->getState() != common::TaskBase::Deactivated)
                return false;
        }
    }
    return true;
}

void Controller::removeGravityTorquesFromSolution(bool do_remove)
{
    remove_gravity_torques_.get() = do_remove;
}
void Controller::removeCoriolisTorquesFromSolution(bool do_remove)
{
    remove_coriolis_torques_.get() = do_remove;
}

std::shared_ptr<Problem> Controller::getProblemAtLevel(int level)
{
    if(level < problems_.size())
    {
        auto it = problems_.begin();
        std::advance(it, level);
        return *it;
    }
    orca_throw(Formatter() << "Level " << level << " does not exist.\n"
                            << "There is only " << problems_.size() << " level(s)");
}

std::shared_ptr<task::RegularisationTask<ControlVariable::X> > Controller::globalRegularization(int level)
{
    auto problem = getProblemAtLevel(level);
    for(auto t : problem->getTasks())
    {
        if(t->getName() == "GlobalRegularisation")
        {
            return std::dynamic_pointer_cast<task::RegularisationTask<ControlVariable::X> >(t);
        }
    }
    return 0;
}

void Controller::insertNewLevel()
{
    LOG_INFO << "Inserting new dry Problem at level " << problems_.size();
    auto problem = std::make_shared<Problem>(robot_.get(),solver_type_);
    problems_.push_back(problem);

    auto dynamics_equation = std::make_shared<constraint::DynamicsEquationConstraint>("DynamicsEquation");
    auto global_regularisation = std::make_shared<task::RegularisationTask<ControlVariable::X> >("GlobalRegularisation");

    addConstraint(dynamics_equation);
    addTask(global_regularisation);

    LOG_INFO << "Controller has now " << problems_.size() << " levels";
}

void Controller::updateTasks(double current_time, double dt)
{
    for(auto problem : problems_)
    {
        for(auto t : problem->getTasks())
        {
            // Checking size
            int cv = problem->getSize(t->getControlVariable());
            if(t->cols() != cv)
            {
                orca_throw(Formatter() << "Size of task " << t->getName()
                            << " (control var " << t->getControlVariable()
                            << " should be " << cv << " but is " << t->cols() << ")");
            }
            t->update(current_time,dt);
        }
    }
}

void Controller::updateConstraints(double current_time, double dt)
{
    for(auto problem : problems_)
    {
        for(auto c : problem->getConstraints())
        {
            // Checking size
            int cv = problem->getSize(c->getControlVariable());
            if(c->cols() != cv)
            {
                orca_throw(Formatter() << "Size of constraint " << c->getName()
                            << " (control var " << c->getControlVariable()
                            << " should be " << cv << " but is " << c->cols() << ")");
            }
            c->update(current_time,dt);
        }
    }
}
