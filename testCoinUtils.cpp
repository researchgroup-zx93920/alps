#include "baseCamp.h" // includes important moving parts of the workflow
#include "configParser.h"

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
            return cfg;
        }

        if (vm.count("version")) {
            std::cout << version << "\n";
            cfg.invalidate_config();
            return cfg;
        }    

        // Now initialize logfile and set log formatting >> 
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
            logging::trivial::severity >= logging::trivial::debug
        );

        BOOST_LOG_TRIVIAL(debug) << "Command Line Arguments were read successfully!";
        cfg.parse();
        return cfg;
    }

    catch(std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Error in reading configuration " << e.what();
        std::cerr<< "\nALPS terminated due to error!";
        throw;
    }
    
    catch(...) {
        std:: string msg = "Unkown Exception!";
        BOOST_LOG_TRIVIAL(fatal) << msg;
        std::cerr << msg <<"!\n";
        throw;
    }
}

int main(int ac, char* av[])
{   
    
    Config cfg = initialize_ALPS(ac, av);
    
    BOOST_LOG_TRIVIAL(info) << "Hello User! This is Alps!\n";
    return 0;
}


// Error 
// -1 : Problems in reading the configuration


