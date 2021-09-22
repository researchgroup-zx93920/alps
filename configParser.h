#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "baseCamp.h"

namespace po = boost::program_options;

class Config {

    public:
    
        Config(po::variables_map vm);
        ~Config();
        bool Valid;
        std::string inputFile, input_type;
        void invalidate_config();
        void parse();

    private:
        po::variables_map _vm;
        void _set_input_name();
        void _set_input_type();

};