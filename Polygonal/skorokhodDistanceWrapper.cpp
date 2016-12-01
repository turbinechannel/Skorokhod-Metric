#include "skorokhodDistanceWrapper.h"
#include <iostream>
#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  skorokhodDistanceWrapper

/* Original author: Xiaoqing Jin
 * Author : Jyotirmoy V. Deshmukh
 * Notes:
 * 0.0 This file was obtained by minor shameless modifications to Xiaoqing's code 
 * */

// Need to include simstruc.h for the definition of the SimStruct and
// its associated macro definitions.
#include "simstruc.h"

#define NPARAMS 4

#define NOUTPORT 1
#define TIME_IDX 0
#define VALUE_IDX 1

#if !defined(MATLAB_MEX_FILE)
/*
 * This file cannot be used directly with the Real-Time Workshop. However,
 * this S-function does work with the Real-Time Workshop via
 * the Target Language Compiler technology. See
 * matlabroot/toolbox/simulink/blocks/tlc_c/sfun_multiport.tlc
 * for the C version
 * matlabroot/toolbox/simulink/blocks/tlc_ada/sfun_multiport.tlc
 * for the Ada version
 */
# error This_file_can_be_used_only_during_simulation_inside_Simulink
#endif


/*====================*
 * S-function methods *
 *====================*/
#define IS_PARAM_DOUBLE(pVal) (mxIsNumeric(pVal) && !mxIsLogical(pVal) &&\
!mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal))

#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)
/* Function: mdlCheckParameters =============================================
 * Abstract:
 *    Validate our parameters to verify they are okay.
 */
static void mdlCheckParameters(SimStruct *S)
{    
    const mxArray *windowSize = ssGetSFcnParam(S,0);
    if ( !IS_PARAM_DOUBLE(windowSize) && !mxIsInt16(windowSize) ) {
        ssSetErrorStatus(S, "Window size must be a 16-bit integer");
        return;
    } 
    const mxArray *delta = ssGetFcnParam(S,1);
    if ( !IS_PARAM_DOUBLE(delta)) {
        ssSetErrorStatus(S, "Delta must be a double scalar");
        return;
    }
    const mxArray *timeScale = ssGetSFcnParam(S,2);
    if ( !IS_PARAM_DOUBLE(timeScale)) {
        ssSetErrorStatus(S, "Time scaling factor must be a double scalar");
        return;
    }
    const mxArray *signalScale = ssGetSFcnParam(S,3);
    if ( !IS_PARAM_DOUBLE(signalScale)) {
        ssSetErrorStatus(S, "Signal Scaling vector must be a double vector");
        return;
    }
}
#endif /* MDL_CHECK_PARAMETERS */

// Function: mdlInitializeSizes ===============================================
// Abstract:
//    The sizes information is used by Simulink to determine the S-function
//    block's characteristics (number of inputs, outputs, states, etc.).
static void mdlInitializeSizes(SimStruct *S)
{
    // Check the number of expected parameters
    ssSetNumSFcnParams(S, NPARAMS);

    // Parameter mismatch will be reported by Simulink
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* report error if parameters set by this file is not 
         * equal to number of parameters set by the block */
        return;
    }
    mdlCheckParameters(S);
    if (ssGetErrorStatus(S) != NULL) {
        /* report error if there is error checking parameters */
        return;
    }

    ssSetSFcnParamTunable(S,0,0);
    ssSetSFcnParamTunable(S,1,0);
    ssSetSFcnParamTunable(S,2,0);
    ssSetSFcnParamTunable(S,3,0);
    // Specify I/O

    /* Set number of input ports */
    if (!ssSetNumInputPorts(S, 2)) return;

    if (!ssSetInputPortDimensionInfo(S, 0, DYNAMIC_DIMENSION)) return;
    if (!ssSetInputPortDimensionInfo(S, 1, DYNAMIC_DIMENSION)) return;

    /* Set that the input port can be of dynamic dimension */
    ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);
    ssSetInputPortWidth(S, 1, DYNAMICALLY_SIZED);

    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    
    if (!ssSetNumOutputPorts(S,2)) return;
    ssSetOutputPortWidth(S, 0, 1);
    ssSetOutputPortWidth(S, 1, 1);
        
    ssSetOutputPortDataType(S, 0, SS_BOOLEAN);
    ssSetOutputPortDataType(S, 1, SS_BOOLEAN);
    
    ssSetNumSampleTimes(S, -1);
    
    // Reserve place for C++ object
    ssSetNumPWork(S, 1);
    
    ssSetSimStateCompliance(S, USE_CUSTOM_SIM_STATE);
    
    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE | 
                    SS_OPTION_EXCEPTION_FREE_CODE);    
}

#if defined(MATLAB_MEX_FILE)
#define MDL_SET_INPUT_PORT_DIMENSION_INFO
static void mdlSetInputPortDimensionInfo(SimStruct *S,
        int_T port, const DimsInfo_T *dimsInfo)
{
    if(!ssSetInputPortDimensionInfo(S, port, dimsInfo)) return;
}
#endif

#if defined(MATLAB_MEX_FILE)
#define MDL_SET_OUTPUT_PORT_DIMENSION_INFO
static void mdlSetOutputPortDimensionInfo(SimStruct *S,
        int_T port, const DimsInfo_T *dimsInfo)
{
    if(!ssSetOutputPortDimensionInfo(S, port, dimsInfo)) return;
}
#endif

#if defined(MATLAB_MEX_FILE)
#define MDL_SET_DEFAULT_PORT_DIMENSION_INFO
/* Function: mdlSetDefaultPortDimensionInfo ===========================================
 * Abstract:
 *   In case no ports were specified, the default is an input port of width 2
 *   and an output port of width 1.
 */
static void mdlSetDefaultPortDimensionInfo(SimStruct *S)
{
    ssSetInputPortWidth(S, 0, 1);
    ssSetInputPortWidth(S, 1, 1);
    ssSetOutputPortWidth(S, 0, 1);
    ssSetOutputPortWidth(S, 1, 1);
}
#endif

// Function: mdlInitializeSampleTimes =========================================
// Abstract:
//   This function is used to specify the sample time(s) for your
//   S-function. You must register the same number of sample times as
//   specified in ssSetNumSampleTimes.
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

// Function: mdlStart =======================================================
// Abstract:
//   This function is called once at start of model execution. If you
//   have states that should be initialized once, this is the place
//   to do it.
#define MDL_START
static void mdlStart(SimStruct *S)
{
    // Store new C++ object in the pointers vector        
    uint16 window           = (uint16) mxGetScalar(ssGetSFcnParam(S,0));
    double delta            = mxGetScalar(ssGetSFcnParam(S,1));
    vector<double> scaleVector;  
    double timeScaleFactor  = mxGetScalar(ssGetSFcnParam(S,2));
    scaleVector.push_back(timeScaleFactor);
    const mxArray* a = ssGetSFcnParam(S,2);
    size_t numSignals = mxGetNumberOfElements(a);
    double *s = mxGetPr(a);
    for (size_t i=0; i<numSignals; ++i) {
        scaleVector.push_back(s[i]);
    }    
    SkorokhodDistanceWrapper *wrapper  = 
            new SkorokhodDistanceWrapper(window, delta, scaleVector);
    ssGetPWork(S)[0] = wrapper;
}

// Function: mdlOutputs =======================================================
// Abstract:
//   In this function, you compute the outputs of your S-function
//   block.
static void mdlOutputs(SimStruct *S, int_T tid)
{
    // Retrieve C++ object from the pointers vector
    SkorokhodDistanceWrapper *wrapper = 
            static_cast<SkorokhodDistanceWrapper *>(ssGetPWork(S)[0]);
    
    // Get data addresses of I/O
    InputRealPtrsType  u0 = ssGetInputPortRealSignalPtrs(S,0);
    InputRealPtrsType  u1 = ssGetInputPortRealSignalPtrs(S,1);
    int_T u0_width  = ssGetInputPortWidth(S,0);
    int_T u1_width  = ssGetInputPortWidth(S,1);
    
    if (u0_width != u1_width) {
        ssSetErrorStatus(S, "The two input ports should be of the same width!!");
        return;
    }
    vector<double> u0_vec;
    vector<double> u1_vec;

    for (int_T i=0;i<u0_width;i++) {
        u0_vec.push_back(*u0[i]);
        u1_vec.push_back(*u1[i]);
    }
    boolean_T *status = ssGetOutputPortSignal(S, 0);
    boolean_T *result = ssGetOutputPortSignal(S, 1);

    timePoint* point0 = new timePoint(ssGetT(S), u0_vec);
    timePoint* point1 = new timePoint(ssGetT(S), u1_vec);
    
    // Call ComputeSkorokhodDistance method to add a pair of inputs
    status[TIME_IDX] = ssGetT(S);
    result[TIME_IDX] = ssGetT(S);

    /* You may have to pass window size as an argument to this function */
    pair<bool,bool> monitor_result = wrapper->scaleAndMonitor(point0, point1);
    status[TIME_IDX] = monitor_result.first;
    result[TIME_IDX] = monitor_result.second;
}


// Function: mdlTerminate =====================================================
// Abstract:
//   In this function, you should perform any actions that are necessary
//   at the termination of a simulation.  For example, if memory was
//   allocated in mdlStart, this is the place to free it.
static void mdlTerminate(SimStruct *S)
{
    // Retrieve and destroy C++ object
    SkorokhodDistanceWrapper *wrapper = 
            static_cast<SkorokhodDistanceWrapper *>(ssGetPWork(S)[0]);
    delete wrapper;
}

// Required S-function trailer
#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
