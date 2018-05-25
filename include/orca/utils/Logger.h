#pragma once

#include <plog/Log.h>


namespace orca
{
    namespace utils
    {

        enum class LogLevel
        {
            none = 0,
            fatal = 1,
            error = 2,
            warning = 3,
            info = 4,
            debug = 5,
            verbose = 6
        };

        struct Logger
        {
            Logger();
            void setLogLevel(LogLevel log_level);
            void setLogLevel(int log_level);
            void setLogLevel(const std::string& log_level);
            void parseArgv(int argc,char ** argv);
			void parseArgv(const std::vector<std::string>& v);
            void parseArgv(int argc,char const* argv[]);
        };
    }
}

#ifdef _WIN32
#include "orca/optim/ControlVariable.h"
namespace plog
{
    Record& operator<<(Record& record, const orca::optim::ControlVariable& t);
}
#endif
