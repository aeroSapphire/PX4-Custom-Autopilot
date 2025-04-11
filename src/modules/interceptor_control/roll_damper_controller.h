//
// File: roll_damper_controller.h
//
// Code generated for Simulink model 'roll_damper_controller'.
//
// Model version                  : 1.5
// Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
// C/C++ source code generated on : Thu Apr 10 03:36:04 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_roll_damper_controller_h_
#define RTW_HEADER_roll_damper_controller_h_
#include "rtwtypes_roll.h"
#include "roll_damper_controller_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Class declaration for model roll_damper_controller
class roll_damper_controller final
{
  // public data and function members
 public:
  // Block states (default storage) for system '<Root>'
  struct DW_roll_damper_controller_T {
    real_T DiscreteTimeIntegrator_DSTATE;// '<Root>/Discrete-Time Integrator'
  };

  // Constant parameters (default storage)
  struct ConstP_roll_damper_controller_T {
    // Expression: [0.5;0.2;0.025;0.015;0.015;0.0085;0.0065;0.0045;0.0025;0.0015;0.00105]*0.25
    //  Referenced by: '<Root>/Kp_Roll_Damper'

    real_T Kp_Roll_Damper_tableData[11];

    // Expression: [25;50;100;160;170;200;250;275;290;350;450]
    //  Referenced by: '<Root>/Kp_Roll_Damper'

    real_T Kp_Roll_Damper_bp01Data[11];

    // Expression: [0;0;0.2;0.1;0.08;0.062;0.031;0.026;0.014;0.01;0.008]*0.25
    //  Referenced by: '<Root>/Ki_Roll_Damper'

    real_T Ki_Roll_Damper_tableData[11];

    // Expression: [25;50;100;150;160;170;200;250;300;400;450]
    //  Referenced by: '<Root>/Ki_Roll_Damper'

    real_T Ki_Roll_Damper_bp01Data[11];
  };

  // Real-time Model Data Structure
  struct RT_MODEL_roll_damper_controll_T {
    const char_T * volatile errorStatus;
  };

  // Copy Constructor
  roll_damper_controller(roll_damper_controller const&) = delete;

  // Assignment Operator
  roll_damper_controller& operator= (roll_damper_controller const&) & = delete;

  // Move Constructor
  roll_damper_controller(roll_damper_controller &&) = delete;

  // Move Assignment Operator
  roll_damper_controller& operator= (roll_damper_controller &&) = delete;

  // Real-Time Model get method
  roll_damper_controller::RT_MODEL_roll_damper_controll_T * getRTM();

  // model initialize function
  static void initialize();

  // model step function
  void step(real_T arg_roll_rate_command, real_T arg_roll_rate_body, real_T
            arg_speed_magnitude, real_T &arg_aileron_deflection);

  // model terminate function
  static void terminate();

  // Constructor
  roll_damper_controller();

  // Destructor
  ~roll_damper_controller();

  // private data and function members
 private:
  // Block states
  DW_roll_damper_controller_T roll_damper_controller_DW;

  // Real-Time Model
  RT_MODEL_roll_damper_controll_T roll_damper_controller_M;
};

// Constant parameters (default storage)
extern const roll_damper_controller::ConstP_roll_damper_controller_T
  roll_damper_controller_ConstP;

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
//  '<Root>' : 'roll_damper_controller'
//  '<S1>'   : 'roll_damper_controller/Radians to Degrees'

#endif                                 // RTW_HEADER_roll_damper_controller_h_

//
// File trailer for generated code.
//
// [EOF]
//
