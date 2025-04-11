//
// File: pitch_damper_controller.h
//
// Code generated for Simulink model 'pitch_damper_controller'.
//
// Model version                  : 1.1
// Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
// C/C++ source code generated on : Thu Apr 10 03:25:24 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_pitch_damper_controller_h_
#define RTW_HEADER_pitch_damper_controller_h_
#include "rtwtypes_pitch.h"
#include "pitch_damper_controller_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Class declaration for model pitch_damper_controller
class pitch_damper_controller final
{
  // public data and function members
 public:
  // Block states (default storage) for system '<Root>'
  struct DW_pitch_damper_controller_T {
    real32_T DiscreteTimeIntegrator_DSTATE;// '<Root>/Discrete-Time Integrator'
  };

  // Constant parameters (default storage)
  struct ConstP_pitch_damper_controlle_T {
    // Computed Parameter: Kp_Pitch_Damper_tableData
    //  Referenced by: '<Root>/Kp_Pitch_Damper'

    real32_T Kp_Pitch_Damper_tableData[11];

    // Computed Parameter: Kp_Pitch_Damper_bp01Data
    //  Referenced by: '<Root>/Kp_Pitch_Damper'

    real32_T Kp_Pitch_Damper_bp01Data[11];

    // Computed Parameter: Ki_Pitch_Damper_tableData
    //  Referenced by: '<Root>/Ki_Pitch_Damper'

    real32_T Ki_Pitch_Damper_tableData[11];

    // Computed Parameter: Ki_Pitch_Damper_bp01Data
    //  Referenced by: '<Root>/Ki_Pitch_Damper'

    real32_T Ki_Pitch_Damper_bp01Data[11];
  };

  // Real-time Model Data Structure
  struct RT_MODEL_pitch_damper_control_T {
    const char_T * volatile errorStatus;
  };

  // Copy Constructor
  pitch_damper_controller(pitch_damper_controller const&) = delete;

  // Assignment Operator
  pitch_damper_controller& operator= (pitch_damper_controller const&) & = delete;

  // Move Constructor
  pitch_damper_controller(pitch_damper_controller &&) = delete;

  // Move Assignment Operator
  pitch_damper_controller& operator= (pitch_damper_controller &&) = delete;

  // Real-Time Model get method
  pitch_damper_controller::RT_MODEL_pitch_damper_control_T * getRTM();

  // model initialize function
  static void initialize();

  // model step function
  void step(real32_T arg_pitch_rate_command, real32_T arg_pitch_rate_body,
            real32_T arg_speed_magnitude, real32_T &arg_elevator_deflection);

  // model terminate function
  static void terminate();

  // Constructor
  pitch_damper_controller();

  // Destructor
  ~pitch_damper_controller();

  // private data and function members
 private:
  // Block states
  DW_pitch_damper_controller_T pitch_damper_controller_DW;

  // Real-Time Model
  RT_MODEL_pitch_damper_control_T pitch_damper_controller_M;
};

// Constant parameters (default storage)
extern const pitch_damper_controller::ConstP_pitch_damper_controlle_T
  pitch_damper_controller_ConstP;

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
//  '<Root>' : 'pitch_damper_controller'
//  '<S1>'   : 'pitch_damper_controller/Radians to Degrees'

#endif                                 // RTW_HEADER_pitch_damper_controller_h_

//
// File trailer for generated code.
//
// [EOF]
//
