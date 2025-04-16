//
// File: roll_damper_controller_data.cpp
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
#include "roll_damper_controller.h"

// Constant parameters (default storage)
const roll_damper_controller::ConstP_roll_damper_controller_T
  roll_damper_controller_ConstP{
  // Expression: [0.5;0.2;0.025;0.015;0.015;0.0085;0.0065;0.0045;0.0025;0.0015;0.00105]*0.25
  //  Referenced by: '<Root>/Kp_Roll_Damper'

  { 0.125, 0.05, 0.00625, 0.00375, 0.00375, 0.002125, 0.001625, 0.001125,
    0.000625, 0.000375, 0.0002625 },

  // Expression: [25;50;100;160;170;200;250;275;290;350;450]
  //  Referenced by: '<Root>/Kp_Roll_Damper'

  { 25.0, 50.0, 100.0, 160.0, 170.0, 200.0, 250.0, 275.0, 290.0, 350.0, 450.0 },

  // Expression: [0;0;0.2;0.1;0.08;0.062;0.031;0.026;0.014;0.01;0.008]*0.25
  //  Referenced by: '<Root>/Ki_Roll_Damper'

  { 0.0, 0.0, 0.05, 0.025, 0.02, 0.0155, 0.00775, 0.0065, 0.0035, 0.0025, 0.002
  },

  // Expression: [25;50;100;150;160;170;200;250;300;400;450]
  //  Referenced by: '<Root>/Ki_Roll_Damper'

  { 25.0, 50.0, 100.0, 150.0, 160.0, 170.0, 200.0, 250.0, 300.0, 400.0, 450.0 }
};

//
// File trailer for generated code.
//
// [EOF]
//
