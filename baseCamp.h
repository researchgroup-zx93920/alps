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

// Reading Specific
#include<coin-or/CoinMpsIO.hpp>
#include<coin-or/CoinLpIO.hpp>

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

#define BOOST_LOG_DYN_LINK 1

// includes the dist info
#include "information.h" 
#include "configParser.h"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

namespace po = boost::program_options;

void initiaize_ALPS() {
    /*
        Initializes the logger file and sets the environment
    */
    std::time_t epoch = std::time(nullptr);
    std::stringstream logfile;
    logfile << epoch;

    logging::add_common_attributes();

    logging::add_file_log(
        keywords::file_name = "./logs/ALPS_"+logfile.str()+".log",                                        
        // keywords::rotation_size = 10 * 1024 * 1024,                                   
        // keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), 
        keywords::format = "[%TimeStamp%]: %Message%"
        );

    logging::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%]: %Message%");

     logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}

int readConfig(int ac, char* av[]) {

    try {
        
        std::string description = "ALPS - Accelerated LP Solver\n"
        "\tVersion : "+std::to_string(version)+" | Maintained by: "+contributor+"\n\n"
        "Following are the command line arguments accepted by this program";
        po::options_description generic(description);
        generic.add_options()
            ("help,h", "Display CLI help guide")
            ("version,v", "Determine program version");

        po::options_description _usage("Usage");
        _usage.add_options()
            ("inputFile,i", po::value<std::string>(), 
            "An MPS/LP file to parse the linear program to be solved");

        po::options_description cmdline_options;
        cmdline_options.add(generic).add(_usage);

        po::options_description config_file_options;
        config_file_options.add(_usage);

        po::variables_map vm;        
        po::store(po::command_line_parser(ac, av).options(cmdline_options).run(), vm);

        std::ifstream ifs("./run_config.cfg");
        po::store(parse_config_file(ifs, config_file_options), vm);
        notify(vm);

        if (vm.count("help")) {
            std::cout << cmdline_options << "\n";
            return 0;
        }

        if (vm.count("version")) {
            std::cout << version << "\n";
            return 0;
        }    

        BOOST_LOG_TRIVIAL(info) << "Hello! This is ALPS!";
        if (vm.count("inputFile")) {
            std::cout << "inputFile received as : " 
                 << vm["inputFile"].as<std::string>() << ".\n";
        } else {
            std::string msg = "No Input file provided";
            BOOST_LOG_TRIVIAL(error) << msg;
            return -1;    
        }
    }

    catch(std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Error in reading configuration " << e.what();
        std::cerr<< "\nALPS terminated due to error!";
        return -1;
    }
    
    catch(...) {
        std:: string msg = "Unkown Exception!";
        BOOST_LOG_TRIVIAL(fatal)<< msg;
        std::cerr << msg <<"!\n";
    }
}