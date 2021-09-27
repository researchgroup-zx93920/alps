// Reading Specific
#include<coin-or/CoinLpIO.hpp>
#include<coin-or/CoinMpsIO.hpp>
#include<coin-or/CoinFileIO.hpp>
#include<coin-or/CoinPresolveMatrix.hpp>

// Internals
#include "baseCamp.h" // includes important components of the workflow

// Verify the problem support and Create the pre-solve object
template <typename T>
CoinPresolveMatrix common_construction(T &fileHandler) {
    
    // Apply the check >> All columns are non-integers - performs checks upto a tolerance
    BOOST_LOG_TRIVIAL (debug) << "Verifying the nature of variables";
    int _integer_type_count = 0;
    for (int colIndx = 0; colIndx < fileHandler.getNumCols(); colIndx++){
        if (fileHandler.isInteger(colIndx)==1 || fileHandler.getColLower()[colIndx] != 0.0) {
            _integer_type_count += 1;
            BOOST_LOG_TRIVIAL(error) << "Variable :"<<fileHandler.columnName(colIndx)<<" is an integer or negative. This is not an acceptable type!";
            if (_integer_type_count >= _MAX_INTEGER_TRACES) {
                break;
            }
        }
    }
    if (_integer_type_count > 0){
        if (_integer_type_count >= _MAX_INTEGER_TRACES) {
            BOOST_LOG_TRIVIAL(error) << "> "<<_MAX_INTEGER_TRACES<<" integer or negative variable domains found! Not supported in the current scope :(";
            throw ALPS_Exception();
        }
        else {
            BOOST_LOG_TRIVIAL(error) << "Integer or Negative variable domain found! Not supported in the current scope :(";
            throw ALPS_Exception();
        }
    }
    
    // Apply the check >> All constraints are equality type - performs checks upto a tolerance
    BOOST_LOG_TRIVIAL (debug) << "Verifying the sense of constraints";
    const char * row_senses = fileHandler.getRowSense();
    int _inequality_type_count = 0;
    for (int rowIndx = 0; rowIndx < fileHandler.getNumCols(); rowIndx++) {
        if (row_senses[rowIndx]!='E') {
            _inequality_type_count += 1;
            if (_inequality_type_count >= _MAX_INEQUALITY_TRACES) {
                break;
            }
        }
    }

    if (_inequality_type_count > 0){
        if (_integer_type_count >= _MAX_INTEGER_TRACES) {
            BOOST_LOG_TRIVIAL(error) << "> "<<_MAX_INEQUALITY_TRACES<<" inequality constraints found! Not supported in the current scope :(";
            throw ALPS_Exception();
        }
        else {
            BOOST_LOG_TRIVIAL(error) << "inequality constraints found! Not supported in the current scope :(";
            throw ALPS_Exception();
        }
    }

    BOOST_LOG_TRIVIAL(debug) << "Start LP - Presolve Construction";
    CoinPresolveMatrix lp_pre_construct = CoinPresolveMatrix(fileHandler.getNumCols(), fileHandler.getNumRows(), fileHandler.getNumElements());
    
    // Set A - Matrix >>
    BOOST_LOG_TRIVIAL(debug) << "\tSTEP 1: Matrix Construction";
    lp_pre_construct.setMatrix(fileHandler.getMatrixByCol());
        
    // Set constraint sense and bounds (rhs vector and sense) >>
    BOOST_LOG_TRIVIAL(debug) << "\tSTEP 2: Adding Row Bounds";
    lp_pre_construct.setRowUpper(fileHandler.getRowUpper(), -1);
    lp_pre_construct.setRowLower(fileHandler.getRowLower(), -1);

    // Set bounds >>
    BOOST_LOG_TRIVIAL(debug) << "\tSTEP 3: Adding Column Bounds";
    lp_pre_construct.setColUpper(fileHandler.getColUpper(), -1);
    lp_pre_construct.setColLower(fileHandler.getColLower(), -1);

    // Set Objective
    BOOST_LOG_TRIVIAL(debug) << "\tSTEP 4: Adding Objective/Costs and minimization sense";
    lp_pre_construct.setCost(fileHandler.getObjCoefficients(), -1);
    // Apply Minimization sense to LP
    lp_pre_construct.setObjSense(1.0);

    // Return pre-construct for further pre-processing >> 
    return lp_pre_construct;
}

void readInput(std::string filepath, acceptableInput _type, const double * A_elements, 
    const int * A_indices, const int * A_vectorStarts, const double * b, const double * c) {
    
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
        
        // todo : Common Presolve for pre-construct
    
        // 0. Determine how to use - CoinORPrepost Solve and
        // 1. Routine pre-process with control conditions
        
        CoinPresolveMatrix pre_construct = common_construction(fileHandler);
        // Update pointers to Cpp standard constructs >> 
        
        // Ideally this should come through CoinPostSolveMatrix Object 
        // todo: Look into this ^ later
        A_elements = fileHandler.getMatrixByRow()->getElements();
        A_indices = fileHandler.getMatrixByRow()->getIndices();
        A_vectorStarts = fileHandler.getMatrixByRow()->getVectorStarts();
        b = fileHandler.getRightHandSide();
        c = fileHandler.getObjCoefficients();
        break;
    }
    case LP:
    {
        CoinLpIO fileHandler = CoinLpIO();
        fileHandler.messageHandler()->setLogLevel(1);
        fileHandler.readLp(filepath.c_str());
        // File was read and processed successfully at this stage
        BOOST_LOG_TRIVIAL(debug) << "File Processed Successfully!";
        CoinPresolveMatrix pre_construct = common_construction(fileHandler);
        
        // Ready my comments in the case MPS
        // todo : This needs to go in a common method
        A_elements = fileHandler.getMatrixByRow()->getElements();
        A_indices = fileHandler.getMatrixByRow()->getIndices();
        A_vectorStarts = fileHandler.getMatrixByRow()->getVectorStarts();
        b = fileHandler.getRightHandSide();
        c = fileHandler.getObjCoefficients();
        break;
    }
    default:
    {
        BOOST_LOG_TRIVIAL(fatal)<<"File Extension unrecognized! Unknown Exception";
        throw ALPS_Exception();
    }
    }
}