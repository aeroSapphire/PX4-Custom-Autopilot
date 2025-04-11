//
// File: roll_damper_controller.cpp
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
#include "rtwtypes_roll.h"
#include "roll_damper_controller_private.h"
#include "roll_damper_controller_data.cpp"

real_T look1_binlxpw(real_T u0, const real_T bp0[], const real_T table[],
                     uint32_T maxIndex)
{
  real_T frac;
  real_T yL_0d0;
  uint32_T iLeft;

  // Column-major Lookup 1-D
  // Search method: 'binary'
  // Use previous index: 'off'
  // Interpolation method: 'Linear point-slope'
  // Extrapolation method: 'Linear'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Remove protection against out-of-range input in generated code: 'off'

  // Prelookup - Index and Fraction
  // Index Search method: 'binary'
  // Extrapolation method: 'Linear'
  // Use previous index: 'off'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Remove protection against out-of-range input in generated code: 'off'

  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex]) {
    uint32_T bpIdx;
    uint32_T iRght;

    // Binary Search
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex - 1U;
    frac = (u0 - bp0[maxIndex - 1U]) / (bp0[maxIndex] - bp0[maxIndex - 1U]);
  }

  // Column-major Interpolation 1-D
  // Interpolation method: 'Linear point-slope'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Overflow mode: 'portable wrapping'

  yL_0d0 = table[iLeft];
  return (table[iLeft + 1U] - yL_0d0) * frac + yL_0d0;
}

// Model step function
void roll_damper_controller::step(real_T arg_roll_rate_command, real_T
  arg_roll_rate_body, real_T arg_speed_magnitude, real_T &arg_aileron_deflection)
{
  real_T rtb_Sum1;

  // Sum: '<Root>/Sum1' incorporates:
  //   Inport: '<Root>/roll_rate_body'
  //   Inport: '<Root>/roll_rate_command'

  rtb_Sum1 = arg_roll_rate_command - arg_roll_rate_body;

  // Outport: '<Root>/aileron_deflection' incorporates:
  //   DiscreteIntegrator: '<Root>/Discrete-Time Integrator'
  //   Gain: '<Root>/Gain'
  //   Gain: '<S1>/Gain'
  //   Inport: '<Root>/speed_magnitude'
  //   Lookup_n-D: '<Root>/Kp_Roll_Damper'
  //   Product: '<Root>/Product1'
  //   Sum: '<Root>/Sum'

  arg_aileron_deflection = -(rtb_Sum1 * look1_binlxpw(arg_speed_magnitude,
    roll_damper_controller_ConstP.Kp_Roll_Damper_bp01Data,
    roll_damper_controller_ConstP.Kp_Roll_Damper_tableData, 10U) +
    roll_damper_controller_DW.DiscreteTimeIntegrator_DSTATE) *
    57.295779513082323;

  // Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' incorporates:
  //   Inport: '<Root>/speed_magnitude'
  //   Lookup_n-D: '<Root>/Ki_Roll_Damper'
  //   Product: '<Root>/Product2'

  roll_damper_controller_DW.DiscreteTimeIntegrator_DSTATE += look1_binlxpw
    (arg_speed_magnitude, roll_damper_controller_ConstP.Ki_Roll_Damper_bp01Data,
     roll_damper_controller_ConstP.Ki_Roll_Damper_tableData, 10U) * rtb_Sum1 *
    0.004;
}

// Model initialize function
void roll_damper_controller::initialize()
{
  // (no initialization code required)
}

// Model terminate function
void roll_damper_controller::terminate()
{
  // (no terminate code required)
}

// Constructor
roll_damper_controller::roll_damper_controller() :
  roll_damper_controller_DW(),
  roll_damper_controller_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
roll_damper_controller::~roll_damper_controller() = default;

// Real-Time Model get method
roll_damper_controller::RT_MODEL_roll_damper_controll_T * roll_damper_controller::
  getRTM()
{
  return (&roll_damper_controller_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
