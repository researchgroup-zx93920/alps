#include "baseCamp.h" // includes important moving parts of the workflow
#include "information.h" // includes the dist info

int main(int ac, char* av[])
{
    initiaize_ALPS();

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

    BOOST_LOG_TRIVIAL(info) << "Hello User! This is Alps!\n";
    return 0;
}


// Error 
// -1 : Problems in reading the configuration


