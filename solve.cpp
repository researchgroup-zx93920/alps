#include "baseCamp.h" // includes important moving parts of the workflow
#include "configParser.h"
#include "inputParser.h"

void set_logging() {
    
    // Now initialize logfile and set log formatting >> 
    std::time_t epoch = std::time(nullptr);
    std::stringstream logfile;
    logfile << epoch;
    logging::add_common_attributes();
    logging::register_simple_formatter_factory< logging::trivial::severity_level, char >("Severity");
    logging::add_file_log(
        keywords::file_name = "./logs/ALPS_"+logfile.str()+".log",                                        
        // keywords::rotation_size = 10 * 1024 * 1024,                                   
        // keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), 
        keywords::format = "[%TimeStamp%] | <%Severity%> : %Message%"
        );

    logging::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%]: %Message%");

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::debug
    );
}

Config initialize_ALPS(int ac, char* av[]) {
     /*
        Initializes the logger, read configuration files and the necessary environment
    */

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
        Config cfg(vm);

        if (vm.count("help")) {
            std::cout << cmdline_options << "\n";
            cfg.invalidate_config();
            exit(0);
        }

        if (vm.count("version")) {
            std::cout << "ALPS Version: " << version << "\n";
            cfg.invalidate_config();
            exit(0);
        }    

        set_logging();
        BOOST_LOG_TRIVIAL(debug) << "Command Line Arguments were read successfully!";
        return cfg;
    }

    catch(ALPS_Exception &e) {
        BOOST_LOG_TRIVIAL(error) << "Error in validating command line arguments" << e.what();
        exit(-1);
    }
    
    catch(...) {
        std::string msg = "Unkown Exception!";
        BOOST_LOG_TRIVIAL(fatal) << msg;
        exit(-2);
    }
}

int main(int ac, char* av[])
{   
    
    Config cfg = initialize_ALPS(ac, av);
    BOOST_LOG_TRIVIAL(debug) << "Reading and validating the configuration";
    try {
        cfg.parse();
        BOOST_LOG_TRIVIAL(debug) << "Valid Configuration, Reading input model file";
        readInput(cfg.inputFile, cfg.inputType);

        /* The result of a readInput call is an LP Construct >>
        // Say your LP is in the form -
            min C^T x
            st. Ax <= b
            
        */
    }
    catch (ALPS_Exception &e) {
        BOOST_LOG_TRIVIAL(error) << "Error in reading input file" << e.what();
        exit(-3);
    }
    catch (...) {
        std::string msg = "Unkown Exception!";
        BOOST_LOG_TRIVIAL(fatal) << msg;
        exit(-2);
    }

    BOOST_LOG_TRIVIAL(info) << "Hello User! This is Alps!\n";
    return 0;
}
