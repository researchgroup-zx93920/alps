// Reading Specific
#include<coin-or/CoinLpIO.hpp>
#include<coin-or/CoinMpsIO.hpp>

// Internals
#include "baseCamp.h" // includes important components of the workflow

void readInput(std::string filepath, acceptableInput _type) {
    BOOST_LOG_TRIVIAL(info) << "Reading Input File";
    switch (_type)
    {
    case MPS:
    {
        CoinMpsIO fileHandler = CoinMpsIO();
        fileHandler.messageHandler()->setLogLevel(1);
        fileHandler.readMps(filepath.c_str());
        break;
    }
    case LP:
    {
        CoinLpIO fileHandler = CoinLpIO();
        fileHandler.messageHandler()->setLogLevel(1);
        fileHandler.readLp(filepath.c_str());
        break;
    }
    default:
    {
        BOOST_LOG_TRIVIAL(fatal)<<"File Extension unrecognized! Unknown Exception";
        throw ALPS_Exception();
    }
    }
    // File was read and processed successfully at this stage
    BOOST_LOG_TRIVIAL(info) << "File Processed Successfully!";

}