//
// File: normal_acceleration_controller_data.cpp
//
// Code generated for Simulink model 'normal_acceleration_controller'.
//
// Model version                  : 1.2
// Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
// C/C++ source code generated on : Mon Feb  3 04:19:24 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "normal_acceleration_controller.h"

// Block parameters (default storage)
normal_acceleration_controller::P_normal_acceleration_control_T
  normal_acceleration_controller::normal_acceleration_controlle_P{

  // Mask Parameter: DiscreteDerivative_ICPrevScaled
  //  Referenced by: '<S1>/UD'

  0.0F,

  // Computed Parameter: Constant_Value
  //  Referenced by: '<Root>/Constant'

  -9.81F,

  // Computed Parameter: Saturation_UpperSat
  //  Referenced by: '<Root>/Saturation'

  40.0F,

  // Computed Parameter: Saturation_LowerSat
  //  Referenced by: '<Root>/Saturation'

  -40.0F,

  // Computed Parameter: DiscreteTimeIntegrator_gainval
  //  Referenced by: '<Root>/Discrete-Time Integrator'

  0.0025F,

  // Computed Parameter: DiscreteTimeIntegrator_IC
  //  Referenced by: '<Root>/Discrete-Time Integrator'

  0.0F,

  // Computed Parameter: TSamp_WtEt
  //  Referenced by: '<S1>/TSamp'

  400.0F
};

//
// File trailer for generated code.
//
// [EOF]
//
