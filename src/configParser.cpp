#include "configParser.h"

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
            {
                BOOST_LOG_TRIVIAL(debug) << "File path valid";
            }
        else {
            BOOST_LOG_TRIVIAL(error)<<"Input file not found!";
            throw ALPS_Exception();
        }
    }
    else {
        std::string msg = "No Input file provided, check help!";
        BOOST_LOG_TRIVIAL(error) << msg;
        throw ALPS_Exception();
    }
}

void Config::_set_input_type() {
    std::string _file_ext = inputFile.substr(inputFile.find_last_of(".") + 1);
    if (_file_ext == "lp") {
        BOOST_LOG_TRIVIAL(debug)<<"Detected input format is .lp";
        inputType = acceptableInput::LP;
    }
    else if (_file_ext == "mps"){
        BOOST_LOG_TRIVIAL(debug)<<"Detected input format is .mps";
        inputType = acceptableInput::MPS;
    } 
    else {
        BOOST_LOG_TRIVIAL(error)<<"Input format: ." + _file_ext + " is not recognized";
        throw ALPS_Exception();
    }
}