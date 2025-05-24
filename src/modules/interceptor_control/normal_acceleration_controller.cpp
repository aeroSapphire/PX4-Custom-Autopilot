//
// File: normal_acceleration_controller.cpp
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
#include "normal_acceleration_controller_data.cpp"
#include "rtwtypes_nac.h"
#include <cmath>

// Model step function
void normal_acceleration_controller::step(real32_T arg_Az_cmd, real32_T
  (&arg_Axyz_Body)[3], real32_T arg_alpha, real32_T arg_beta, real32_T
  &arg_pitch_rate_commanded, real32_T Kp_normal_acc, real32_T Ki_normal_acc, real32_T Kd_normal_acc)
{
  int32_T i;
  real32_T rtb_Sum_tmp_0[9];
  real32_T tmp[3];
  real32_T rtb_Sum;
  real32_T rtb_Sum_tmp;
  real32_T rtb_Sum_tmp_tmp;
  real32_T rtb_TSamp;

  // MATLAB Function: '<Root>/MATLAB Function' incorporates:
  //   Inport: '<Root>/alpha'
  //   Inport: '<Root>/beta'

  rtb_Sum = std::cos(arg_beta);
  rtb_TSamp = std::cos(arg_alpha);
  rtb_Sum_tmp = std::sin(arg_beta);
  rtb_Sum_tmp_tmp = std::sin(arg_alpha);
  rtb_Sum_tmp_0[0] = rtb_TSamp * rtb_Sum;
  rtb_Sum_tmp_0[1] = rtb_Sum_tmp;
  rtb_Sum_tmp_0[2] = rtb_Sum_tmp_tmp * rtb_Sum;
  rtb_Sum_tmp_0[3] = -rtb_TSamp * rtb_Sum_tmp;
  rtb_Sum_tmp_0[4] = rtb_Sum;
  rtb_Sum_tmp_0[5] = -rtb_Sum_tmp_tmp * rtb_Sum_tmp;
  rtb_Sum_tmp_0[6] = -rtb_Sum_tmp_tmp;
  rtb_Sum_tmp_0[7] = 0.0F;
  rtb_Sum_tmp_0[8] = rtb_TSamp;

  // Math: '<Root>/Transpose' incorporates:
  //   Inport: '<Root>/Axyz_Body'
  //   Product: '<Root>/MatrixMultiply'

  rtb_Sum = arg_Axyz_Body[1];
  rtb_TSamp = arg_Axyz_Body[0];
  rtb_Sum_tmp = arg_Axyz_Body[2];

  // Product: '<Root>/MatrixMultiply' incorporates:
  //   Inport: '<Root>/Axyz_Body'
  //   Math: '<Root>/Transpose'

  for (i = 0; i < 3; i++) {
    tmp[i] = (rtb_Sum_tmp_0[3 * i + 1] * rtb_Sum + rtb_Sum_tmp_0[3 * i] *
              rtb_TSamp) + rtb_Sum_tmp_0[3 * i + 2] * rtb_Sum_tmp;
  }

  // Saturate: '<Root>/Saturation' incorporates:
  //   Inport: '<Root>/Az_cmd'

  if (arg_Az_cmd > normal_acceleration_controlle_P.Saturation_UpperSat) {
    rtb_Sum = normal_acceleration_controlle_P.Saturation_UpperSat;
  } else if (arg_Az_cmd < normal_acceleration_controlle_P.Saturation_LowerSat) {
    rtb_Sum = normal_acceleration_controlle_P.Saturation_LowerSat;
  } else {
    rtb_Sum = arg_Az_cmd;
  }

  // Sum: '<Root>/Sum' incorporates:
  //   Constant: '<Root>/Constant'
  //   Product: '<Root>/MatrixMultiply'
  //   Saturate: '<Root>/Saturation'
  //   Sum: '<Root>/Subtract'

  rtb_Sum -= tmp[2] - normal_acceleration_controlle_P.Constant_Value;

  // SampleTimeMath: '<S1>/TSamp' incorporates:
  //   Gain: '<Root>/Gain2'
  //
  //  About '<S1>/TSamp':
  //   y = u * K where K = 1 / ( w * Ts )

  rtb_TSamp = Kd_normal_acc * rtb_Sum *
    normal_acceleration_controlle_P.TSamp_WtEt;

  // Outport: '<Root>/pitch_rate_commanded' incorporates:
  //   DiscreteIntegrator: '<Root>/Discrete-Time Integrator'
  //   Gain: '<Root>/Gain1'
  //   Sum: '<Root>/Sum1'
  //   Sum: '<S1>/Diff'
  //   UnitDelay: '<S1>/UD'
  //
  //  Block description for '<S1>/Diff':
  //
  //   Add in CPU
  //
  //  Block description for '<S1>/UD':
  //
  //   Store in Global RAM

  arg_pitch_rate_commanded = (Kp_normal_acc *
    rtb_Sum + normal_acceleration_controll_DW.DiscreteTimeIntegrator_DSTATE) +
    (rtb_TSamp - normal_acceleration_controll_DW.UD_DSTATE);

  // Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' incorporates:
  //   Gain: '<Root>/Gain3'

  normal_acceleration_controll_DW.DiscreteTimeIntegrator_DSTATE +=
    Ki_normal_acc * rtb_Sum *
    normal_acceleration_controlle_P.DiscreteTimeIntegrator_gainval;

  // Update for UnitDelay: '<S1>/UD'
  //
  //  Block description for '<S1>/UD':
  //
  //   Store in Global RAM

  normal_acceleration_controll_DW.UD_DSTATE = rtb_TSamp;
}

// Model initialize function
void normal_acceleration_controller::initialize()
{
  // InitializeConditions for DiscreteIntegrator: '<Root>/Discrete-Time Integrator'
  normal_acceleration_controll_DW.DiscreteTimeIntegrator_DSTATE =
    normal_acceleration_controlle_P.DiscreteTimeIntegrator_IC;

  // InitializeConditions for UnitDelay: '<S1>/UD'
  //
  //  Block description for '<S1>/UD':
  //
  //   Store in Global RAM

  normal_acceleration_controll_DW.UD_DSTATE =
    normal_acceleration_controlle_P.DiscreteDerivative_ICPrevScaled;
}

// Model terminate function
void normal_acceleration_controller::terminate()
{
  // (no terminate code required)
}

// Constructor
normal_acceleration_controller::normal_acceleration_controller() :
  normal_acceleration_controll_DW(),
  normal_acceleration_controll_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
normal_acceleration_controller::~normal_acceleration_controller() = default;

// Real-Time Model get method
normal_acceleration_controller::RT_MODEL_normal_acceleration__T
  * normal_acceleration_controller::getRTM()
{
  return (&normal_acceleration_controll_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
