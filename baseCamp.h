/* 
    All files would include basecamp.h to get the necessary initialization,
    general library includes, and bunch of utilities
*/

// cpp stl
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<ctime>

// // Reading Specific
// #include<coin-or/CoinMpsIO.hpp>
// #include<coin-or/CoinLpIO.hpp>

// Command line options
#include<boost/program_options.hpp>

// Logging setup
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

// includes the dist info
#include "information.h" 

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

namespace po = boost::program_options;