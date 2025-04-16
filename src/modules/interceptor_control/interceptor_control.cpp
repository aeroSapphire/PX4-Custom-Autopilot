#include "interceptor_control.hpp"
#include <px4_platform_common/log.h>
#include <mathlib/mathlib.h>
#include <matrix/math.hpp>
#include <uORB/topics/actuator_servos.h>  // Include actuator_servos

using namespace matrix;

InterceptorControl::InterceptorControl() :
    ModuleParams(nullptr),
    ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::nav_and_controllers)
{
    parameters_update();

    _vehicle_local_position_sub.subscribe();
    _vehicle_angular_velocity_sub.subscribe();
    _sensor_accel_sub.subscribe();

    // Initialize the actuator_servos publisher for raw PWM output.
    _actuator_servos_pub.advertise();
    _actuator_controls_0_pub.advertise();
}

InterceptorControl::~InterceptorControl()
{
    // Cleanup if necessary
}

bool InterceptorControl::init()
{
    ScheduleOnInterval(10_ms);  // Runs every 10 milliseconds
    return true;
}

void InterceptorControl::set_accel_commands(float Ay, float Az)
{
    _accel_commands.Ay_cmd = Ay;
    _accel_commands.Az_cmd = Az;
    _accel_commands.valid = true;
}

bool InterceptorControl::get_accel_command(matrix::Vector3f &vec)
{
    if (!_accel_commands.valid) {
        return false;
    }
    vec(0) = 0.0f;  // Assuming X acceleration is not used
    vec(1) = _accel_commands.Ay_cmd;
    vec(2) = _accel_commands.Az_cmd;
    return true;
}

void InterceptorControl::Run()
{
    if (should_exit()) {
        exit_and_cleanup();
        return;
    }

    if (!_initialized) {
        // Initialize controllers used for damping
        _pitch_damper.initialize();
        _yaw_damper.initialize();
        _roll_damper.initialize();
        _initialized = true;
    }

    Vector3f acceleration_commands;
    if (!get_accel_command(acceleration_commands)) {
        PX4_DEBUG("No acceleration commands");
        return;
    }

    // Get vehicle local position
    vehicle_local_position_s local_pos{};
    if (!_vehicle_local_position_sub.update(&local_pos)) {
        PX4_WARN("Failed to get missile position/velocity.");
        // return;
    }

    // Get angular velocity (gyro data)
    vehicle_angular_velocity_s angular_velocity{};
    if (!_vehicle_angular_velocity_sub.update(&angular_velocity)) {
        PX4_WARN("Failed to get angular velocity.");
        // return;
    }
    Vector3f gyro_latest(angular_velocity.xyz[0], angular_velocity.xyz[1], angular_velocity.xyz[2]);

    // Get accelerometer data (optional)
    sensor_accel_s accel_data{};
    if (!_sensor_accel_sub.update(&accel_data)) {
        PX4_WARN("Failed to get accelerometer data.");
        // return;
    }
    Vector3f acc_latest(accel_data.x, accel_data.y, accel_data.z);

    // Determine pitch rate (Y-axis corresponds to pitch)
    float pitch_rate_body = gyro_latest(1);

    // Calculate speed magnitude (for scaling, if needed)
    float speed_magnitude = sqrtf(local_pos.vx * local_pos.vx +
                                  local_pos.vy * local_pos.vy +
                                  local_pos.vz * local_pos.vz);

    // Compute deflections using your custom controllers/dampers
    float elevator_deflection = 0.0f;
    _pitch_damper.step(0.0f, pitch_rate_body, speed_magnitude, elevator_deflection);

    float rudder_deflection = 0.0f;
    _yaw_damper.step(0.0f, gyro_latest(2), speed_magnitude, rudder_deflection);

    // Convert deflections to raw PWM values.
    // Here we assume full deflection ±7.0 corresponds to PWM range [1000, 2000] with 1500 as neutral.
//     float elevator_PWM = math::constrain((elevator_deflection * 500.0f / 7.0f) + 1500.0f, 1000.0f, 2000.0f);
//     float rudder_PWM   = math::constrain((rudder_deflection * 500.0f / 7.0f) + 1500.0f, 1000.0f, 2000.0f);

//     // For channels not controlled by your module, use a default (neutral) PWM.
//     float aileron_PWM  = 1500.0f;  // Neutral (if no roll control)
//     float throttle_PWM = 1500.0f;  // For SITL, neutral throttle might be 1500 or as required.

//     // Build and populate the actuator_servos message with the raw PWM values.
//     actuator_servos_s actuators{};
//     actuators.timestamp = hrt_absolute_time();
//     // Set the first 4 channels (adjust indices if your simulator expects a different order):
//     actuators.control[0] = aileron_PWM;   // e.g., aileron
//     actuators.control[1] = elevator_PWM;  // e.g., elevator
//     actuators.control[2] = throttle_PWM;  // e.g., throttle
//     actuators.control[3] = rudder_PWM;    // e.g., rudder
//     // Optionally set other channels to neutral if needed:
//     for (size_t i = 4; i < actuator_servos_s::NUM_CONTROLS; i++) {
//         actuators.control[i] = 1500.0f;  // or your desired neutral value
//     }

//     // Publish the raw PWM signal message.
//     _actuator_servos_pub.publish(actuators);




// --- START CHANGES ---

    // Convert deflections to NORMALIZED values [-1, +1].
    // Adjust the divisor (7.0f) based on the actual max deflection your dampers output.
    float elevator_norm = math::constrain(elevator_deflection / 7.0f, -1.0f, 1.0f);
    float rudder_norm   = math::constrain(rudder_deflection / 7.0f, -1.0f, 1.0f);

    // Determine other axis commands (set defaults or get from RC/other source)
    float roll_norm = 0.0f;     // Assuming no roll control from this module
    float throttle_norm = 0.0f; // Assuming no throttle control (0.0 = min, 1.0 = max)
                                // Or set based on desired speed, e.g., 0.5 for mid-throttle


    // Build and populate the actuator_controls_0 message.
    actuator_controls_s actuators_ctl{};
    actuators_ctl.timestamp = hrt_absolute_time();

    // Assign normalized values to the correct indices for fixed-wing:
    actuators_ctl.control[actuator_controls_s::INDEX_ROLL]     = roll_norm;
    actuators_ctl.control[actuator_controls_s::INDEX_PITCH]    = elevator_norm;
    actuators_ctl.control[actuator_controls_s::INDEX_YAW]      = rudder_norm;
    actuators_ctl.control[actuator_controls_s::INDEX_THROTTLE] = throttle_norm;
    // Optionally set other controls if needed (indices 4-7 for flaps, etc.)
    // actuators_ctl.control[4] = ... ;

    // Publish the normalized control message.
    _actuator_controls_0_pub.publish(actuators_ctl);

//     PX4_INFO("Pitch rate: %.4f, Elevator PWM: %.2f, Rudder PWM: %.2f, Speed: %.2f",
//              static_cast<double>(pitch_rate_body),
//              static_cast<double>(elevator_norm),
//              static_cast<double>(rudder_norm),
//              static_cast<double>(speed_magnitude));
}

void InterceptorControl::parameters_update()
{
    // Update parameters if needed.
}

int InterceptorControl::main(int argc, char *argv[])
{
    if (argc < 2) {
        print_usage("Missing command");
        return 1;
    }

    if (!strcmp(argv[1], "start")) {
        if (_object.load() != nullptr) {
            PX4_WARN("Already running");
            return 1;
        }

        _object.store(new InterceptorControl());

        if (_object.load() == nullptr) {
            PX4_ERR("Allocation failed");
            return 1;
        }

        if (!_object.load()->init()) {
            delete _object.load();
            _object.store(nullptr);
            PX4_ERR("Init failed");
            return 1;
        }

        return 0;
    }

    if (!strcmp(argv[1], "stop")) {
        if (_object.load() == nullptr) {
            PX4_WARN("Not running");
            return 1;
        }
        delete _object.load();
        _object.store(nullptr);
        return 0;
    }

    if (!strcmp(argv[1], "status")) {
        PX4_INFO(_object.load() ? "Running" : "Not running");
        return 0;
    }

    return print_usage("Unknown command");
}

int InterceptorControl::custom_command(int argc, char *argv[])
{
    return print_usage("Unknown command");
}

int InterceptorControl::print_usage(const char *reason)
{
    PRINT_MODULE_USAGE_NAME("interceptor_control", "controller");
    PRINT_MODULE_USAGE_COMMAND("start");
    PRINT_MODULE_USAGE_COMMAND_DESCR("stop", "Stop the module");
    return 0;
}

extern "C" __EXPORT int interceptor_control_main(int argc, char *argv[])
{
    return InterceptorControl::main(argc, argv);
}
