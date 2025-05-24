//
// File: lateral_acceleration_controller_data.cpp
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
#include "lateral_acceleration_controller.h"

// Block parameters (default storage)
lateral_acceleration_controller::P_lateral_acceleration_contro_T
  lateral_acceleration_controller::lateral_acceleration_controll_P{

  // Mask Parameter: DiscreteDerivative_ICPrevScaled
  //  Referenced by: '<S1>/UD'

  0.0F,

  // Computed Parameter: DiscreteTimeIntegrator_gainval
  //  Referenced by: '<Root>/Discrete-Time Integrator'

  0.0025F,

  // Computed Parameter: DiscreteTimeIntegrator_IC
  //  Referenced by: '<Root>/Discrete-Time Integrator'

  0.0F,

  // Computed Parameter: Saturation_UpperSat
  //  Referenced by: '<Root>/Saturation'

  40.0F,

  // Computed Parameter: Saturation_LowerSat
  //  Referenced by: '<Root>/Saturation'

  -40.0F,

  // Computed Parameter: Gain3_Gain
  //  Referenced by: '<Root>/Gain3'

  0.4F,

  // Computed Parameter: TSamp_WtEt
  //  Referenced by: '<S1>/TSamp'

  400.0F
};

//
// File trailer for generated code.
//
// [EOF]
//
