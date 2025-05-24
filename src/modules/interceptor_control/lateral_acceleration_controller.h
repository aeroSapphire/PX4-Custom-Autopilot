//
// File: lateral_acceleration_controller.h
//
// Code generated for Simulink model 'lateral_acceleration_controller'.
//
// Model version                  : 1.3
// Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
// C/C++ source code generated on : Mon Feb  3 04:23:50 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_lateral_acceleration_controller_h_
#define RTW_HEADER_lateral_acceleration_controller_h_
#include "rtwtypes_lac.h"
#include "lateral_acceleration_controller_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Class declaration for model lateral_acceleration_controller
class lateral_acceleration_controller final
{
  // public data and function members
 public:
  // Block states (default storage) for system '<Root>'
  struct DW_lateral_acceleration_contr_T {
    real32_T DiscreteTimeIntegrator_DSTATE;// '<Root>/Discrete-Time Integrator'
    real32_T UD_DSTATE;                // '<S1>/UD'
  };

  // Parameters (default storage)
  struct P_lateral_acceleration_contro_T {

    real32_T DiscreteDerivative_ICPrevScaled;
                              // Mask Parameter: DiscreteDerivative_ICPrevScaled
                                 //  Referenced by: '<S1>/UD'

    real32_T DiscreteTimeIntegrator_gainval;
                           // Computed Parameter: DiscreteTimeIntegrator_gainval
                              //  Referenced by: '<Root>/Discrete-Time Integrator'

    real32_T DiscreteTimeIntegrator_IC;
                                // Computed Parameter: DiscreteTimeIntegrator_IC
                                   //  Referenced by: '<Root>/Discrete-Time Integrator'

    real32_T Saturation_UpperSat;     // Computed Parameter: Saturation_UpperSat
                                         //  Referenced by: '<Root>/Saturation'

    real32_T Saturation_LowerSat;     // Computed Parameter: Saturation_LowerSat
                                         //  Referenced by: '<Root>/Saturation'

    real32_T Gain3_Gain;               // Computed Parameter: Gain3_Gain
                                          //  Referenced by: '<Root>/Gain3'

    real32_T TSamp_WtEt;               // Computed Parameter: TSamp_WtEt
                                          //  Referenced by: '<S1>/TSamp'

  };

  // Real-time Model Data Structure
  struct RT_MODEL_lateral_acceleration_T {
    const char_T * volatile errorStatus;
  };

  // Copy Constructor
  lateral_acceleration_controller(lateral_acceleration_controller const&) =
    delete;

  // Assignment Operator
  lateral_acceleration_controller& operator= (lateral_acceleration_controller
    const&) & = delete;

  // Move Constructor
  lateral_acceleration_controller(lateral_acceleration_controller &&) = delete;

  // Move Assignment Operator
  lateral_acceleration_controller& operator= (lateral_acceleration_controller &&)
    = delete;

  // Real-Time Model get method
  lateral_acceleration_controller::RT_MODEL_lateral_acceleration_T * getRTM();

  // Tunable parameters
  static P_lateral_acceleration_contro_T lateral_acceleration_controll_P;

  // model initialize function
  void initialize();

  // model step function
  void step(real32_T arg_Ay_body, real32_T arg_Ay_cmd, real32_T arg_yaw_rate,
            real32_T &arg_yaw_rate_command, real32_T Kp_lateral_acc, real32_T Ki_lateral_acc, real32_T Kd_lateral_acc);

  // model terminate function
  static void terminate();

  // Constructor
  lateral_acceleration_controller();

  // Destructor
  ~lateral_acceleration_controller();

  // private data and function members
 private:
  // Block states
  DW_lateral_acceleration_contr_T lateral_acceleration_control_DW;

  // Real-Time Model
  RT_MODEL_lateral_acceleration_T lateral_acceleration_control_M;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S1>/Data Type Duplicate' : Unused code path elimination


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'lateral_acceleration_controller'
//  '<S1>'   : 'lateral_acceleration_controller/Discrete Derivative'

#endif                         // RTW_HEADER_lateral_acceleration_controller_h_

//
// File trailer for generated code.
//
// [EOF]
//
