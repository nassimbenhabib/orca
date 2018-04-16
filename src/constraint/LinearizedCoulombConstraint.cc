#include "orca/constraint/LinearizedCoulombConstraint.h"
#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif
using namespace orca::constraint;
using namespace orca::common;

LinearizedCoulombConstraint::LinearizedCoulombConstraint()
: GenericConstraint(optim::ControlVariable::ExternalWrench)
{
    R_cone_.setIdentity();
    this->resize();
}
double LinearizedCoulombConstraint::getFrictionCoeff() const
{
    MutexLock lock(this->mutex);

    return friction_coeff_;
}

double LinearizedCoulombConstraint::getMargin() const
{
    MutexLock lock(this->mutex);

    return margin_;
}

void LinearizedCoulombConstraint::setAngleOffset(double angle_offset)
{
    MutexLock lock(this->mutex);

    angle_offset_ = angle_offset;
}

void LinearizedCoulombConstraint::setFrictionCoeff(double coeff)
{
    MutexLock lock(this->mutex);

    friction_coeff_ = coeff;
}

void LinearizedCoulombConstraint::setMargin(double margin)
{
    MutexLock lock(this->mutex);

    margin_ = margin;
}

void LinearizedCoulombConstraint::setConeOrientation(const Eigen::Matrix3d& R)
{
    MutexLock lock(this->mutex);

    R_cone_ = R;
}

const Eigen::Matrix3d& LinearizedCoulombConstraint::getConeOrientation() const
{
    MutexLock lock(this->mutex);

    return R_cone_;
}

void LinearizedCoulombConstraint::setNumberOfFaces(int nfaces)
{
    MutexLock lock(this->mutex);

    if (nfaces < 3)
    {
        LOG_ERROR << "[orca::LinearizedCoulombFunction] Number of faces is less than 3";
        return;
    }

    if(nfaces != constraintFunction().rows())
    {
        number_of_faces_ = nfaces;
        this->resize();
    }
}

void LinearizedCoulombConstraint::resize()
{
    MutexLock lock(this->mutex);

    if(number_of_faces_ != constraintFunction().rows())
    {
        constraintFunction().resize(number_of_faces_, 3);
        constraintFunction().lowerBound().setConstant( - math::Infinity );
        constraintFunction().upperBound().setConstant(margin_);
    }
}

void LinearizedCoulombConstraint::updateConstraintFunction()
{
    MutexLock lock(this->mutex);
    
    const double angleIncr = 2. * M_PI/(double)number_of_faces_;

    v1_[0] = friction_coeff_ * std::cos(angle_offset_);                 //ray of the discreatized cone
    v1_[1] = friction_coeff_ * std::sin(angle_offset_);
    v1_[2] = 1;

    for (int i=0; i < number_of_faces_; ++i)
    {
        angle_offset_ += angleIncr;
        v2_[0] = friction_coeff_ * std::cos(angle_offset_);                 //ray of the discretized cone
        v2_[1] = friction_coeff_ * std::sin(angle_offset_);
        v2_[2] = 1;

        n_ = v2_.cross(v1_);                     //normal vector to a face
        n_.normalize();

        constraintFunction().constraintMatrix().row(i) = n_.transpose();

        v1_ = v2_;
    }

    constraintFunction().constraintMatrix() *= R_cone_.transpose();
    constraintFunction().upperBound().setConstant(margin_);
}
