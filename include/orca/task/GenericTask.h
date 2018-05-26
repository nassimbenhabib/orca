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
#include "orca/math/WeightedEuclidianNormFunction.h"
#include "orca/robot/RobotDynTree.h"
#include "orca/common/TaskBase.h"

namespace orca
{
namespace task
{

class GenericTask : public common::TaskBase
{

public:
    GenericTask(const std::string& name,optim::ControlVariable control_var);

    virtual ~GenericTask();

    double getWeight() const;
    void setWeight(double weight);

    math::Size getSize() const;
    int cols() const;
    int rows() const;

    math::WeightedEuclidianNormFunction& euclidianNorm();
    const math::WeightedEuclidianNormFunction& getEuclidianNorm() const;

    const Eigen::MatrixXd& getE() const;
    const Eigen::VectorXd& getf() const;

    void setE(const Eigen::MatrixXd& newE);
    void setf(const Eigen::VectorXd& newf);

    virtual void print() const;

    const math::WeightedEuclidianNormFunction::QuadraticCost& getQuadraticCost() const;
    
    Eigen::MatrixXd& E();
    Eigen::VectorXd& f();

    virtual bool rampUp(double time_since_start);
    virtual void onUpdateAffineFunction(double current_time, double dt) = 0;
    virtual void onDeactivation();
    virtual bool rampDown(double time_since_stop);
private:
    virtual void onUpdate(double current_time, double dt);
    virtual void computeQuadraticCost();
    math::WeightedEuclidianNormFunction euclidian_norm_;
    double weight_ = 1.0;
};

}
}
