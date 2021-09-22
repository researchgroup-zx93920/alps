#include "configParser.h"
#include "baseCamp.h"

Config::Config(po::variables_map vm) {
    _vm = vm;
}

Config::~Config(){
}

void Config::invalidate_config() {
    Valid = false;
}

void Config::parse() {
    // todo: Handle relative and absolute path
    _set_input_name();
    _set_input_type();

}

void Config::_set_input_name() {
    
    if (_vm.count("inputFile")) {    
        inputFile = _vm["inputFile"].as<std::string>();
        BOOST_LOG_TRIVIAL(info) << "Reading the input File " << inputFile;
        BOOST_LOG_TRIVIAL(debug) << "Verifying file path";     

        if (boost::filesystem::exists(inputFile))    // does filePath actually exist?
            BOOST_LOG_TRIVIAL(debug) << "File path valid";

        else
            BOOST_LOG_TRIVIAL(error)<<"Input file not found!";
    }
    else {
        std::string msg = "No Input file provided, check help!";
        BOOST_LOG_TRIVIAL(error) << msg;
        throw;
    }
}

void Config::_set_input_type(){

}