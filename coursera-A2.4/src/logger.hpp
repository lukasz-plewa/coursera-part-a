/*
 * logger.hpp
 *
 *  Created on: Nov 28, 2018
 *      Author: L.Plewa
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>
#include <sstream>

/* consider adding boost thread id since we'll want to know whose writing and
 * won't to repeat it for every single call */

/* consider adding policy class to allow users to redirect logging to specific
 * files via the command line
 */

namespace logger{

enum class logLevel_e {logERROR, logWARNING, logINFO, logDEBUG};

class Logger
{
    std::string m_module;
public:
    /**
     * @brief Constructor
     * @param aModule software module that logs e.g class
     */
    explicit Logger(const std::string& aModule) : m_module(aModule) {}
    virtual ~Logger() { std::cerr << "closin logger!!!"<< std::endl; }                   // @brief Destructor

    template <typename T>
    friend Logger & operator<<(Logger& logger, T const & value)
    {
        std::cerr << value;
        return logger;
    }

    // Overload << operator using log level
    friend Logger& operator<<(Logger& logger, const logLevel_e level)
    {
        switch (level)
        {
        case logLevel_e::logERROR:
            std::cerr << "[ERROR]: ";
            break;
        case logLevel_e::logWARNING:
            std::cerr << "[WARNING]: ";
            break;
        case logLevel_e::logINFO:
            std::cerr << "[INFO]: ";
            break;
        case logLevel_e::logDEBUG:
            std::cerr << "[DEBUG]: ";
            break;
        default:
            break;
        }
        return logger;
    }

};

} // namespace logger

#endif /* LOGGER_HPP_ */
