// Reading Specific
#include<coin-or/CoinLpIO.hpp>
#include<coin-or/CoinMpsIO.hpp>
#include<coin-or/CoinFileIO.hpp>
#include<coin-or/CoinPresolveMatrix.hpp>
#include<coin-or/CoinPostSolveMatrix.hpp>


// Internals
#include "baseCamp.h" // includes important components of the workflow

// CoinPostsolveMatrix basicPreProcess(CoinPresolveMatrix pre_)

CoinPresolveMatrix common_construction(const CoinMpsIO &fileHandler) {
    
    // Alps should only accept continuous domains
    // todo: Apply This check >> fileHandler.isInteger()
    // Find all/any methods for c++
    BOOST_LOG_TRIVIAL(debug) << "Start Construction";
    CoinPresolveMatrix lp_pre_construct = CoinPresolveMatrix(fileHandler.getNumCols(), fileHandler.getNumRows(), fileHandler.getNumElements());
    
    // Set A - Matrix
    BOOST_LOG_TRIVIAL(debug) << "STEP 1: Matrix Construction";
    lp_pre_construct.setMatrix(fileHandler.getMatrixByCol());
        
    // Set constraint sense and bounds (rhs vector and sense)
    BOOST_LOG_TRIVIAL(debug) << "STEP 2: Adding Row Bounds";
    lp_pre_construct.setRowUpper(fileHandler.getRowUpper(), -1);
    lp_pre_construct.setRowLower(fileHandler.getRowLower(), -1);

    // Set bounds
    BOOST_LOG_TRIVIAL(debug) << "STEP 3: Adding Column Bounds";
    lp_pre_construct.setColUpper(fileHandler.getColUpper(), -1);
    lp_pre_construct.setColLower(fileHandler.getColLower(), -1);

    // Set Objective
    BOOST_LOG_TRIVIAL(debug) << "STEP 4: Adding Objective/Costs";
    lp_pre_construct.setCost(fileHandler.getObjCoefficients(), -1);

    return lp_pre_construct;
}


void readInput(std::string filepath, acceptableInput _type) {
    BOOST_LOG_TRIVIAL(info) << "Reading Input File";
    switch (_type)
    {
    case MPS:
    {
        CoinMpsIO fileHandler = CoinMpsIO();
        fileHandler.messageHandler()->setLogLevel(1);
        fileHandler.readMps(filepath.c_str());
        // File was read and processed successfully at this stage
        BOOST_LOG_TRIVIAL(debug) << "File Processed Successfully!";
        CoinPresolveMatrix pre_construct = common_construction(fileHandler);
        break;
    }
    case LP:
    {
        CoinLpIO fileHandler = CoinLpIO();

        fileHandler.messageHandler()->setLogLevel(1);
        fileHandler.readLp(filepath.c_str());
        // File was read and processed successfully at this stage
        BOOST_LOG_TRIVIAL(debug) << "File Processed Successfully!";
        // CoinPresolveMatrix pre_construct = common_construction(fileHandler);
        break;
    }
    default:
    {
        BOOST_LOG_TRIVIAL(fatal)<<"File Extension unrecognized! Unknown Exception";
        throw ALPS_Exception();
    }
    
    std::cout<<"Here!"<<std::endl;
    }
    
    // Todo: 
    // 0. Determine how to use - CoinORPrepost Solve and
    // 1. Routine pre-process with control conditions
    // 2. extract vectors c,b and packed matrix A
}