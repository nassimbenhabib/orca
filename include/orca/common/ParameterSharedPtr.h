#pragma once
#include <orca/common/Parameter.h>
#include <orca/common/TaskBase.h>
#include <orca/common/Factory.h>

namespace orca
{
namespace common
{

inline std::string findType(const YAML::Node& n)
{
    for(auto c : n)
    {
        try{
            if(c.first.as<std::string>() == "type")
                return c.second.as<std::string>();
        }catch(...){}
    }
    return std::string();
}

template<class T>
class Parameter<std::shared_ptr<T> > : public ParameterBase, public ParameterData< std::shared_ptr<T> >
{
public:    
    Parameter<std::shared_ptr<T> >()
    : ParameterBase(true)
    {
        std::cout << "Parameter<std::shared_ptr<T> > constructor subparam" << '\n';
    }
    bool loadFromString(const std::string& s)
    {
        YAML::Node node = YAML::Load(s);
        auto type_name = findType(node);
        if(type_name.empty())
        {
            utils::orca_throw(utils::Formatter() << "Param " << getName() << " is of type TaskBase::Ptr be could not find its \"type\"" );
        }
        
        std::cout << "Param " << getName() << " is of type " << type_name;
        auto task_base = Factory::Instance()->createPtr(getName(),type_name);
        
        if(!task_base)
        {
            utils::orca_throw(utils::Formatter() << "Param " << getName() << " Could not create a class of type " << type_name << "" );
        }
        
        std::cout << "Param " << getName() << " created class of type " << type_name << ", now configuring it if it has params itself."<< '\n';
        
        if(!task_base->configureFromString(s))
        {
            return false;
        }
        this->set( std::dynamic_pointer_cast<T>( task_base ));
        std::cout << "Parameter<std::shared_ptr<T> > " << getName() << ": " << this->get() << '\n';
        return true;
    }

    void print() const
    {
        std::cout << "Parameter<std::shared_ptr<T> >  " << getName() << ": " << this->get() << '\n';
    }
    
    bool isSet() const
    {
        return ParameterData<std::shared_ptr<T> >::isSet();
    }
    
    template<class T2>
    Parameter<std::shared_ptr<T> >& operator=(std::shared_ptr<T2> val)
    {
        this->set(val);
        return *this;
    }
};

} // namespace common
} // namespace orca
