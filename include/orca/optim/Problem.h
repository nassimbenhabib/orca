// This file is a part of the orca framework.
// Copyright 2017, ISIR / Universite Pierre et Marie Curie (UPMC)
// Main contributor(s): Antoine Hoarau, hoarau@isir.upmc.fr
//
// This software is a computer program whose purpose is to [describe
// functionalities and technical features of your software].
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

#pragma once
#include "orca/math/Utils.h"
#include "orca/utils/Utils.h"
#include "orca/utils/Logger.h"
#include "orca/optim/ControlVariable.h"
#include "orca/optim/ControlVariableMapping.h"
#include "orca/optim/ProblemData.h"
#include "orca/optim/QPSolver.h"
#include <map>
#include <list>

namespace orca
{
    namespace common
    {
        class Wrench;
        class TaskBase;
    }
    namespace task
    {
        class GenericTask;
        class WrenchTask;
        template<optim::ControlVariable C> class RegularisationTask;
    }
    namespace constraint
    {
        class GenericConstraint;
        class DynamicsEquationConstraint;
    }
    namespace robot
    {
        class RobotDynTree;
    }
}

namespace orca
{
namespace optim
{

class Problem
{
public:
    Problem(std::shared_ptr<robot::RobotDynTree> robot,QPSolver::SolverType solver_type);

    virtual ~Problem();

    void build();
    bool solve();

    bool addConstraint(std::shared_ptr<constraint::GenericConstraint> cstr);
    bool addTask(std::shared_ptr<task::WrenchTask> task);
    bool addTask(std::shared_ptr<task::GenericTask> task);

    const std::list< std::shared_ptr< const common::Wrench> >& getWrenches() const;
    const std::list< std::shared_ptr< task::GenericTask> >& getTasks() const;
    const std::list< std::shared_ptr< constraint::GenericConstraint> >& getConstraints() const;

    unsigned int getIndex(ControlVariable var) const;
    unsigned int getSize(ControlVariable var) const;
    unsigned int getTotalSize() const;
    const std::map<ControlVariable, unsigned int >& getIndexMap() const;
    const std::map<ControlVariable, unsigned int >& getSizeMap() const;
    void print() const;
    Eigen::VectorXd getSolution(ControlVariable var) const;
    std::shared_ptr<QPSolver> qpSolver();
protected:
    std::list< std::shared_ptr< const common::Wrench > > wrenches_;
    std::list< std::shared_ptr<task::GenericTask> > tasks_;
    std::list< std::shared_ptr<constraint::GenericConstraint> > constraints_;

    ControlVariableMapping mapping_;

    std::shared_ptr<QPSolver> qpsolver_;
    ProblemData data_;
    unsigned int number_of_variables_ = 0;
    unsigned int number_of_constraints_rows_ = 0;
    unsigned int ndof_ = 0;
private:
    void resize();
    bool addWrench(std::shared_ptr<const common::Wrench> wrench);
    unsigned int computeNumberOfConstraintRows(std::list< std::shared_ptr<constraint::GenericConstraint> > constraints) const;
    void resizeProblemData(int nvar, int nconstr);
    void resizeSolver(int nvar,int nconstr);
    void resizeTasks();
    void resizeConstraints();

    std::shared_ptr<robot::RobotDynTree> robot_;
};

} // namespace optim
} // namespace orca