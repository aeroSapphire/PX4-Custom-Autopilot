#include "interceptor_control.hpp"
#include <px4_platform_common/log.h>
#include <mathlib/mathlib.h>
#include <matrix/math.hpp>

using namespace matrix;

InterceptorControl::InterceptorControl() :
    ModuleParams(nullptr),
    ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::nav_and_controllers)
{
    parameters_update();

    _vehicle_local_position_sub.subscribe();
    _vehicle_angular_velocity_sub.subscribe();
    _sensor_accel_sub.subscribe();

    // Remove the actuator_servos publisher since we won't publish raw PWM;
    // instead, we publish normalized values.
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
        return;
    }

    // Get angular velocity (gyro data)
    vehicle_angular_velocity_s angular_velocity{};
    if (!_vehicle_angular_velocity_sub.update(&angular_velocity)) {
        PX4_WARN("Failed to get angular velocity.");
        return;
    }
    Vector3f gyro_latest(angular_velocity.xyz[0], angular_velocity.xyz[1], angular_velocity.xyz[2]);

    // Get accelerometer data (unused here but could be used for other control laws)
    sensor_accel_s accel_data{};
    if (!_sensor_accel_sub.update(&accel_data)) {
        PX4_WARN("Failed to get accelerometer data.");
        return;
    }
    Vector3f acc_latest(accel_data.x, accel_data.y, accel_data.z);

    // Use gyro reading to determine the pitch rate (Y-axis corresponds to pitch)
    float pitch_rate_body = gyro_latest(1);

    // Calculate speed magnitude (used by the control laws for scaling, if necessary)
    float speed_magnitude = sqrtf(local_pos.vx * local_pos.vx +
                                  local_pos.vy * local_pos.vy +
                                  local_pos.vz * local_pos.vz);

    // Compute the elevator deflection using your pitch damper
    float elevator_deflection = 0.0f;
    _pitch_damper.step(0.0f, pitch_rate_body, speed_magnitude, elevator_deflection);

    // Similarly, compute the rudder deflection using your yaw damper
    float rudder_deflection = 0.0f;
    _yaw_damper.step(0.0f, gyro_latest(2), speed_magnitude, rudder_deflection);

    // Convert the computed deflections to normalized values.
    // Assume that ±full deflection corresponds to ±7.0 (your chosen maximum deflection value)
    float elevator_norm = math::constrain(elevator_deflection / 7.0f, -1.0f, 1.0f);
    float rudder_norm   = math::constrain(rudder_deflection / 7.0f, -1.0f, 1.0f);

    // Build the normalized actuator_controls message.
    // For fixed-wing aircraft, actuator_controls_0 typically carries:
    // Index 0: Roll [-1,1]
    // Index 1: Pitch [-1,1]
    // Index 2: Yaw [-1,1]
    // Index 3: Throttle [0,1]
    // For any channel not controlled by this module, use 0 (or another appropriate neutral value)
    actuator_controls_s controls{};
    controls.timestamp = hrt_absolute_time();

    // Example mapping for fixed-wing:
    controls.control[actuator_controls_s::INDEX_ROLL]     = 0.0f;        // No roll command (or add roll control if needed)
    controls.control[actuator_controls_s::INDEX_PITCH]    = elevator_norm; // Elevator (pitch)
    controls.control[actuator_controls_s::INDEX_YAW]      = rudder_norm;   // Rudder (yaw)
    controls.control[actuator_controls_s::INDEX_THROTTLE] = 0.7f;        // No throttle command here; adjust if needed

    // Publish the normalized actuator controls; the mixer (configured via a .mix or .toml file) will use these
    // to calculate the final PWM values for the actuator outputs.
    _actuator_controls_0_pub.publish(controls);

    // Log for debugging: shows the normalized values being published.
//     PX4_INFO("Pitch rate: %.4f, Elevator norm: %.2f, Rudder norm: %.2f, Speed: %.2f",
//              static_cast<double>(pitch_rate_body),
//              static_cast<double>(elevator_norm),
//              static_cast<double>(rudder_norm),
//              static_cast<double>(speed_magnitude));
}

void InterceptorControl::parameters_update()
{
    // Update parameters if needed
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
