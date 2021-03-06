#pragma once
#include <orca/common/Parameter.h>
#include <orca/common/Factory.h>
#include <typeinfo>

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
    Parameter() {}
    Parameter(const std::shared_ptr<T >& t)
    {
        this->set(t);
    }
    bool onLoadFromString(const std::string& s)
    {
        YAML::Node node = YAML::Load(s);
        auto type_name = findType(node);
        
        if(type_name.empty())
            utils::orca_throw(utils::Formatter() << "Could not find \"type\" key in the yaml file for param " 
                << getName() << " of type " << typeid(T).name() << "::Ptr"
                << "\nConfig provided : \n" << s );
        
        LOG_DEBUG << "Param " << getName() << " is of type " << type_name;
        // If already set, then we just need to configure it
        if(this->isSet())
        {
            LOG_DEBUG << "Param " << getName() << " is already set, just configuring " << type_name;
            return this->get()->configureFromString(s);
        }
        // If not set, we have to create it from the factory
        auto task_base = Factory::Instance()->createPtr(getName(),type_name);
        
        if(!task_base)
            utils::orca_throw(utils::Formatter() << "Param " << getName() << " Could not create a class of type " << type_name << "" );
        
        LOG_DEBUG << "Param " << getName() << " created class of type " << type_name << ", now configuring it if it has params itself.";
        
        if(!task_base->configureFromString(s))
            return false;
            
        this->set( std::dynamic_pointer_cast<T>( task_base ));
        return true;
    }

    void print() const
    {
        std::cout << getName() << " [ " << typeid(T).name() << "::Ptr" << '\n';
    }
    
    bool isSet() const
    {
        return ParameterData<std::shared_ptr<T> >::isSet();
    }
    std::shared_ptr<T>& get()
    {
        try {
            return ParameterData< std::shared_ptr<T> >::get();
        } catch (std::exception& e) {
            utils::orca_throw(utils::Formatter() << "Parameter '" << getName() << "' : " << e.what());
        }
        return __fix_warnings__;
    }
    const std::shared_ptr<T>& get() const
    {
        try {
            return ParameterData< std::shared_ptr<T> >::get();
        } catch (std::exception& e) {
            utils::orca_throw(utils::Formatter() << "Parameter '" << getName() << "' : " << e.what());
        }
        return __fix_warnings__;
    }
    template<class T2>
    Parameter<std::shared_ptr<T> >& operator=(std::shared_ptr<T2> val)
    {
        this->set(val);
        return *this;
    }
private:
  std::shared_ptr<T> __fix_warnings__ = nullptr;
};

} // namespace common
} // namespace orca
