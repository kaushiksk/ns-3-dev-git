/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef NS3_LOG_H
#define NS3_LOG_H

#include <string>
#include <iostream>
#include <stdint.h>
#include <map>

namespace ns3 {

/**
 * \ingroup debugging
 * \defgroup logging Logging
 *
 * \brief Logging functions and macros
 *
 * LOG functionality: macros which allow developers to
 * send information to the std::clog output stream. All logging messages 
 * are disabled by default. To enable selected logging 
 * messages, use the ns3::LogComponentEnable
 * function or use the NS_LOG environment variable 
 *
 * Use the environment variable NS_LOG to define a ':'-separated list of
 * logging components to enable. For example (using bash syntax), 
 * NS_LOG="OlsrAgent" would enable one component at all log levels. 
 * NS_LOG="OlsrAgent:Ipv4L3Protocol" would enable two components, 
 * at all log levels, etc.
 * NS_LOG="*" will enable all available log components at all levels.
 *
 * To control more selectively the log levels for each component, use
 * this syntax: NS_LOG='Component1=func|warn:Component2=error|debug'
 * This example would enable the 'func', and 'warn' log
 * levels for 'Component1' and the 'error' and 'debug' log levels
 * for 'Component2'.  The wildcard can be used here as well.  For example
 * NS_LOG='*=level_all|prefix' would enable all log levels and prefix all
 * prints with the component and function names.
 *
 * A note on NS_LOG_FUNCTION() and NS_LOG_FUNCTION_NOARGS():
 * generally, use of (at least) NS_LOG_FUNCTION(this) is preferred.
 * Use NS_LOG_FUNCTION_NOARGS() only in static functions.
 */

/**
 *  \ingroup logging
 *
 *  Logging severity classes and levels.
 */
enum LogLevel {
  LOG_NONE           = 0x00000000, //!< no logging

  LOG_ERROR          = 0x00000001, //!< serious error messages only
  LOG_LEVEL_ERROR    = 0x00000001,

  LOG_WARN           = 0x00000002, //!< warning messages
  LOG_LEVEL_WARN     = 0x00000003,

  LOG_DEBUG          = 0x00000004, //!< rare ad-hoc debug messages
  LOG_LEVEL_DEBUG    = 0x00000007,

  LOG_INFO           = 0x00000008, //!< informational messages (e.g., banners)
  LOG_LEVEL_INFO     = 0x0000000f,

  LOG_FUNCTION       = 0x00000010, //!< function tracing
  LOG_LEVEL_FUNCTION = 0x0000001f, 

  LOG_LOGIC          = 0x00000020, //!< control flow tracing within functions
  LOG_LEVEL_LOGIC    = 0x0000003f,

  LOG_ALL            = 0x0fffffff, //!< print everything
  LOG_LEVEL_ALL      = LOG_ALL,

  LOG_PREFIX_FUNC    = 0x80000000, //!< prefix all trace prints with function
  LOG_PREFIX_TIME    = 0x40000000, //!< prefix all trace prints with simulation time
  LOG_PREFIX_NODE    = 0x20000000, //!< prefix all trace prints with simulation node
  LOG_PREFIX_LEVEL   = 0x10000000, //!< prefix all trace prints with log level (severity)
  LOG_PREFIX_ALL     = 0xf0000000  //!< all prefixes
};

/**
 * \ingroup logging
 *
 * Enable the logging output associated with that log component.
 *
 * The logging output can be later disabled with a call
 * to ns3::LogComponentDisable.
 *
 * Same as running your program with the NS_LOG environment
 * variable set as NS_LOG='name=level'
 *
 * \param name a log component name
 * \param level a logging level
 */
void LogComponentEnable (char const *name, enum LogLevel level);

/**
 * \ingroup logging
 *
 * Enable the logging output for all registered log components.
 *
 * Same as running your program with the NS_LOG environment
 * variable set as NS_LOG='*=level'
 *
 * \param level a logging level
 */
void LogComponentEnableAll (enum LogLevel level);


/**
 * \ingroup logging
 *
 * Disable the logging output associated with that log component.
 *
 * The logging output can be later re-enabled with a call
 * to ns3::LogComponentEnable.
 *
 * \param name a log component name
 * \param level a logging level
 */
void LogComponentDisable (char const *name, enum LogLevel level);

/**
 * \ingroup logging
 *
 * Disable all logging for all components.
 *
 * \param level a logging level
 */
void LogComponentDisableAll (enum LogLevel level);


} // namespace ns3

/**
 * \ingroup logging
 *
 * Define a Log component with a specific name.
 *
 * This macro should be used at the top of every file in which you want 
 * to use the NS_LOG macro. This macro defines a new
 * "log component" which can be later selectively enabled
 * or disabled with the ns3::LogComponentEnable and 
 * ns3::LogComponentDisable functions or with the NS_LOG
 * environment variable.
 *
 * \param name a string
 */
#define NS_LOG_COMPONENT_DEFINE(name)                           \
  static ns3::LogComponent g_log = ns3::LogComponent (name)

/**
 * \ingroup logging
 *
 * Define a logging component with a mask.
 *
 * See LogComponent().
 *
 * \param name a string
 * \param mask the default mask
 */
#define NS_LOG_COMPONENT_DEFINE_MASK(name, mask)                \
  static ns3::LogComponent g_log = ns3::LogComponent (name, mask)

/**
 * \ingroup logging
 * Append the simulation time to a log message.
 * \internal
 * Logging implementation macro; should not be called directly.
 */
#define NS_LOG_APPEND_TIME_PREFIX                               \
  if (g_log.IsEnabled (ns3::LOG_PREFIX_TIME))                   \
    {                                                           \
      ns3::LogTimePrinter printer = ns3::LogGetTimePrinter ();  \
      if (printer != 0)                                         \
        {                                                       \
          (*printer)(std::clog);                                \
          std::clog << " ";                                     \
        }                                                       \
    }

/**
 * \ingroup logging
 * Append the simulation node id to a log message.
 * \internal
 * Logging implementation macro; should not be called directly.
 */
#define NS_LOG_APPEND_NODE_PREFIX                               \
  if (g_log.IsEnabled (ns3::LOG_PREFIX_NODE))                   \
    {                                                           \
      ns3::LogNodePrinter printer = ns3::LogGetNodePrinter ();  \
      if (printer != 0)                                         \
        {                                                       \
          (*printer)(std::clog);                                \
          std::clog << " ";                                     \
        }                                                       \
    }

/**
 * \ingroup logging
 * Append the function name to a log message.
 * \internal
 * Logging implementation macro; should not be called directly.
 */
#define NS_LOG_APPEND_FUNC_PREFIX                               \
  if (g_log.IsEnabled (ns3::LOG_PREFIX_FUNC))                   \
    {                                                           \
      std::clog << g_log.Name () << ":" <<                      \
      __FUNCTION__ << "(): ";                                   \
    }                                                           \

/**
 * \ingroup logging
 * Append the log severity level to a log message.
 * \internal
 * Logging implementation macro; should not be called directly.
 */
#define NS_LOG_APPEND_LEVEL_PREFIX(level)                       \
  if (g_log.IsEnabled (ns3::LOG_PREFIX_LEVEL))                  \
    {                                                           \
      std::clog << "[" << g_log.GetLevelLabel (level) << "] ";  \
    }                                                           \
  

#ifndef NS_LOG_APPEND_CONTEXT
/**
 * \ingroup logging
 * Append the node id to a log message.
 *
 * This is implemented locally in `.cc` files because
 * the the relevant variable is only known there.
 *
 * Preferred format is something like (assuming the node id is
 * accessible from `var`:
 * \code
 *   if (var)
 *     {
 *       std::clog << "[node " << var->GetObject<Node> ()->GetId () << "] ";
 *     }
 * \endcode
 *
 * \internal
 * Logging implementation macro; should not be called directly.
 *
 */
#define NS_LOG_APPEND_CONTEXT
#endif /* NS_LOG_APPEND_CONTEXT */


#ifdef NS3_LOG_ENABLE

/**
 * \ingroup logging
 *
 * This macro allows you to log an arbitrary message at a specific
 * log level.
 *
 * The log message is expected to be a C++ ostream
 * message such as "my string" << aNumber << "my oth stream".
 *
 * Typical usage looks like:
 * \code
 * NS_LOG (LOG_DEBUG, "a number="<<aNumber<<", anotherNumber="<<anotherNumber);
 * \endcode
 *
 * \param level the log level
 * \param msg the message to log
 * \internal
 * Logging implementation macro; should not be called directly.
 */
#define NS_LOG(level, msg)                                      \
  do                                                            \
    {                                                           \
      if (g_log.IsEnabled (level))                              \
        {                                                       \
          NS_LOG_APPEND_TIME_PREFIX;                            \
          NS_LOG_APPEND_NODE_PREFIX;                            \
          NS_LOG_APPEND_CONTEXT;                                \
          NS_LOG_APPEND_FUNC_PREFIX;                            \
          NS_LOG_APPEND_LEVEL_PREFIX (level);                   \
          std::clog << msg << std::endl;                        \
        }                                                       \
    }                                                           \
  while (false)

/**
 * \ingroup logging
 *
 * Use \ref NS_LOG to output a message of level LOG_ERROR.
 *
 * \param msg the message to log
 */
#define NS_LOG_ERROR(msg) \
  NS_LOG (ns3::LOG_ERROR, msg)

/**
 * \ingroup logging
 *
 * Use \ref NS_LOG to output a message of level LOG_WARN.
 *
 * \param msg the message to log
 */
#define NS_LOG_WARN(msg) \
  NS_LOG (ns3::LOG_WARN, msg)

/**
 * \ingroup logging
 *
 * Use \ref NS_LOG to output a message of level LOG_DEBUG.
 *
 * \param msg the message to log
 */
#define NS_LOG_DEBUG(msg) \
  NS_LOG (ns3::LOG_DEBUG, msg)

/**
 * \ingroup logging
 *
 * Use \ref NS_LOG to output a message of level LOG_INFO.
 *
 * \param msg the message to log
 */
#define NS_LOG_INFO(msg) \
  NS_LOG (ns3::LOG_INFO, msg)

/**
 * \ingroup logging
 *
 * Output the name of the function.
 *
 * This should be used only in static functions; most member functions
 * should instead use NS_LOG_FUNCTION().
 */
#define NS_LOG_FUNCTION_NOARGS()                                \
  do                                                            \
    {                                                           \
      if (g_log.IsEnabled (ns3::LOG_FUNCTION))                  \
        {                                                       \
          NS_LOG_APPEND_TIME_PREFIX;                            \
          NS_LOG_APPEND_NODE_PREFIX;                            \
          NS_LOG_APPEND_CONTEXT;                                \
          std::clog << g_log.Name () << ":"                     \
                    << __FUNCTION__ << "()" << std::endl;       \
        }                                                       \
    }                                                           \
  while (false)


/**
 * \ingroup logging
 *
 * If log level LOG_FUNCTION is enabled, this macro will output
 * all input parameters separated by ", ".
 *
 * Typical usage looks like:
 * \code
 * NS_LOG_FUNCTION (aNumber<<anotherNumber);
 * \endcode
 * And the output will look like:
 * \code
 * Component:Function (aNumber, anotherNumber)
 * \endcode
 *
 * To facilitate function tracing, most functions should begin with
 * (at least) NS_LOG_FUNCTION(this).  Static functions should use
 * NS_LOG_FUNCTION_NOARGS() instead.
 *
 * \param parameters the parameters to output.
 */
#define NS_LOG_FUNCTION(parameters)                             \
  do                                                            \
    {                                                           \
      if (g_log.IsEnabled (ns3::LOG_FUNCTION))                  \
        {                                                       \
          NS_LOG_APPEND_TIME_PREFIX;                            \
          NS_LOG_APPEND_NODE_PREFIX;                            \
          NS_LOG_APPEND_CONTEXT;                                \
          std::clog << g_log.Name () << ":"                     \
                    << __FUNCTION__ << "(";                     \
          ns3::ParameterLogger (std::clog) << parameters;       \
          std::clog << ")" << std::endl;                        \
        }                                                       \
    }                                                           \
  while (false)


/**
 * \ingroup logging
 *
 * Use \ref NS_LOG to output a message of level LOG_LOGIC
 *
 * \param msg the message to log
 */
#define NS_LOG_LOGIC(msg) \
  NS_LOG (ns3::LOG_LOGIC, msg)

/**
 * \ingroup logging
 *
 * Output the requested message unconditionaly.
 *
 * \param msg the message to log
 */
#define NS_LOG_UNCOND(msg)              \
  do                                    \
    {                                   \
      std::clog << msg << std::endl;    \
    }                                   \
  while (false)

#else /* LOG_ENABLE */

#define NS_LOG(level, msg)
#define NS_LOG_ERROR(msg)
#define NS_LOG_WARN(msg)
#define NS_LOG_DEBUG(msg)
#define NS_LOG_INFO(msg)
#define NS_LOG_FUNCTION_NOARGS()
#define NS_LOG_FUNCTION(msg)
#define NS_LOG_LOGIC(msg)
#define NS_LOG_UNCOND(msg)

#endif /* LOG_ENABLE */

namespace ns3 {

/**
 * \ingroup logging
 *
 * Print the list of logging messages available.
 * Same as running your program with the NS_LOG environment
 * variable set as NS_LOG=print-list
 */
void LogComponentPrintList (void);

typedef void (*LogTimePrinter)(std::ostream &os);
typedef void (*LogNodePrinter)(std::ostream &os);

void LogSetTimePrinter (LogTimePrinter);
LogTimePrinter LogGetTimePrinter (void);

void LogSetNodePrinter (LogNodePrinter);
LogNodePrinter LogGetNodePrinter (void);


/**
 * \ingroup logging
 *
 * A single log component configuration.
 */
class LogComponent
{
public:
  /**
   * Constructor
   *
   * \param [in] name the user-visible name for this component.
   * \param [in] mask LogLevels blocked for this LogComponent.  Blocking
   *                  a log level helps prevent recursion by logging in
   *                  functions which help implement the logging facility.
   */
  LogComponent (const std::string & name, const enum LogLevel mask = LOG_NONE);
  /**
   * Check if this LogComponent is enabled for \pname{level}
   *
   * \param [in] level the level to check for.
   * \return true if \pname{level} is enabled.
   */
  bool IsEnabled (const enum LogLevel level) const;
  /**
   * Check if all levels are disabled.
   *
   * \return true if all levels are disabled.
   */
  bool IsNoneEnabled (void) const;
  /**
   * Enable this LogComponent at \pname{level}
   *
   * \param [in] level the LogLevel to enable.
   */
  void Enable (const enum LogLevel level);
  /**
   * Disable logging at \pname{level} for this LogComponent.
   *
   * \param [in] level the LogLevel to disable.
   */
  void Disable (const enum LogLevel level);
  /**
   * Get the name of this LogComponent.
   *
   * \return the name of this LogComponent.
   */
  char const *Name (void) const;
  /**
   * Get the string label for the given LogLevel.
   *
   * \param [in] level the LogLevel to get the label for.
   * \return the string label for \pname{level}
   */
  static std::string GetLevelLabel(const enum LogLevel level);
  /**
   * Prevent the enabling of a specific LogLevel.
   *
   * \param level the LogLevel to block
   */
  void SetMask (const enum LogLevel level);
private:
  /**
   * Parse the `NS_LOG` environment variable for options relating to this
   * LogComponent.
   */
  void EnvVarCheck (void);
  
  int32_t     m_levels;  //!< Enabled LogLevels
  int32_t     m_mask;    //!< Blocked LogLevels
  std::string m_name;    //!< LogComponent name

};  // class LogComponent

/**
 * \ingroup logging
 *
 * Insert `, ` when streaming function arguments.
 */
class ParameterLogger
{
  bool m_first;        //!< First argument flag, doesn't get `, `.
  std::ostream &m_os;  //!< Underlying output stream.
public:
  /**
   * Constructor.
   *
   * \param [in] os Underlying output stream.
   */
  ParameterLogger (std::ostream &os);

  /**
   * Write a function parameter on the output stream,
   * separating paramters after the first by `,` strings.
   *
   * \param [in] param the function parameter
   */
  template<typename T>
  ParameterLogger& operator<< (T param)
  {
    if (m_first)
      {
        m_os << param;
        m_first = false;
      }
    else
      {
        m_os << ", " << param;
      }
    return *this;
  }
};

} // namespace ns3


#endif /* NS3_LOG_H */
