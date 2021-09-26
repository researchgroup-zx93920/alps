/* 
    All files would include basecamp.h to get the necessary initialization,
    general library includes, and bunch of utilities
*/

#ifndef BASECAMP_INCLUDE
#define BASECAMP_INCLUDE

// cpp stl
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<ctime>
#include<assert.h>

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

// Native exception type
struct ALPS_Exception : public std::exception
{
	const char * what () const throw ()
    {
    	return "ALPS Exception";
    }
};

// Acceptable IO Types
enum acceptableInput {
    LP, MPS
};

#endif