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

#include "orca/robot/RobotDynTree.h"
#include "orca/common/TaskBase.h"

namespace orca
{
namespace common
{

class Wrench : public TaskBase
{
public:
    Wrench(const std::string& name);

    virtual ~Wrench();

    void setBaseFrame(const std::string& base_ref_frame);

    void setControlFrame(const std::string& control_frame);

    void setCurrentValue(const Eigen::Matrix<double,6,1>& current_wrench_from_ft_sensor);

    const Eigen::Matrix<double,6,1>& getCurrentValue() const;

    const std::string& getBaseFrame() const;

    const std::string& getControlFrame() const;

    const Eigen::MatrixXd& getJacobianTranspose() const;

    const Eigen::MatrixXd& getJacobian() const;

    virtual void print() const;

protected:
    virtual void onResize();
    virtual void onStart(){}
    virtual void onUpdate(double current_time, double dt);
    virtual void onStop(){}
private:
    std::string base_ref_frame_,control_frame_;
    Eigen::MatrixXd jacobian_transpose_;
    Eigen::MatrixXd jacobian_;
    Eigen::MatrixXd zero_;
    Eigen::Matrix<double,6,1> current_wrench_;
    std::shared_ptr<robot::RobotDynTree> robot_;
};

}
}
