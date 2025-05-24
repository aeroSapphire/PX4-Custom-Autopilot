#include "interceptor_control.hpp"
#include <px4_platform_common/log.h>
#include <mathlib/mathlib.h>
#include <matrix/math.hpp>
#include <uORB/topics/actuator_servos.h>
#include <uORB/topics/actuator_motors.h>

using namespace matrix;

InterceptorControl::InterceptorControl() :
    ModuleParams(nullptr),
    ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::nav_and_controllers),

    // subscriptions in header order
    _vehicle_attitude_sub(this, ORB_ID(vehicle_attitude)),
    _airspd_sub(ORB_ID(airspeed_validated)),
    _vehicle_local_position_sub(ORB_ID(vehicle_local_position)),
    _vehicle_angular_velocity_sub(ORB_ID(vehicle_angular_velocity)),
    _vehicle_control_mode_sub(ORB_ID(vehicle_control_mode)),
    _vehicle_status_sub(ORB_ID(vehicle_status)),
    _sensor_accel_sub(ORB_ID(sensor_accel)),

    // publications
    _servos_pub(ORB_ID(actuator_servos)),
    _motors_pub(ORB_ID(actuator_motors))
{
    parameters_update();
    _servos_pub.advertise();
    _motors_pub.advertise();
}

InterceptorControl::~InterceptorControl() {}

bool InterceptorControl::init()
{
    ScheduleOnInterval(8_ms);
    return true;
}

void InterceptorControl::set_accel_commands(float Ay, float Az)
{
    _accel_commands.Ay_cmd = Ay;
    _accel_commands.Az_cmd = Az;
    _accel_commands.valid = true;
}

bool InterceptorControl::get_accel_command(Vector3f &vec)
{
    if (!_accel_commands.valid) {
        return false;
    }
    vec = Vector3f{0.f, _accel_commands.Ay_cmd, _accel_commands.Az_cmd};
    return true;
}

void InterceptorControl::Run()
{
    if (should_exit()) {
        exit_and_cleanup();
        return;
    }

    if (!_initialized) {
        _pitch_damper.initialize();
        _yaw_damper.initialize();
        _roll_damper.initialize();
        _normal_acceleration_controller.initialize();
        _lateral_acceleration_controller.initialize();
        _initialized = true;
    }

    // Retrieve commands
    Vector3f accel_cmds;
    if (!get_accel_command(accel_cmds)) {
        PX4_DEBUG("No acceleration commands");
        return;
    }

    // 1) local position
    vehicle_local_position_s local_pos{};
    if (!_vehicle_local_position_sub.copy(&local_pos)) {
        PX4_WARN("local_position copy failed");
        return;
    }

    // 2) attitude
    vehicle_attitude_s att{};
    if (!_vehicle_attitude_sub.copy(&att)) {
        PX4_WARN("attitude copy failed");
        return;
    }

    // 3) body-frame velocity
    Quatf q(att.q);
    Quatf q_inv = q.inversed();
    Vector3f vel_local(local_pos.vx, local_pos.vy, local_pos.vz);
    q_inv.rotate(vel_local); // now vel_local is body-frame
//     float u = vel_local(0);
//     float v = vel_local(1);
//     float w = vel_local(2);
    float speed_body = vel_local.norm();
//     PX4_INFO("Body Vel: u=%.3f, v=%.3f, w=%.3f, |v|=%.3f", (double)u, (double)v, (double)w, (double)speed_body);

    // 4) angular velocity
    vehicle_angular_velocity_s ang{};
    if (!_vehicle_angular_velocity_sub.copy(&ang)) {
        PX4_WARN("angular_velocity copy failed");
        return;
    }
    Vector3f gyro(ang.xyz);

    // 5) raw accel
    sensor_accel_s accel{};
    if (!_sensor_accel_sub.copy(&accel)) {
        PX4_WARN("accel copy failed");
        return;
    }
    // prepare C array for controller
    float acc_body_arr[3] = {accel.x, accel.y, accel.z};

    // 6) control + dampers
    float pitch_rate = gyro(1);
    float elevator_deflection = 0.f;
    float pr_cmd = 0.f;
    _normal_acceleration_controller.step(
        _accel_commands.Az_cmd,
        acc_body_arr,
        0.f, 0.f,
        pr_cmd,
        -0.03f,
        0.f, 0.f
    );
    _pitch_damper.step(pr_cmd, pitch_rate, speed_body, elevator_deflection);

    float ay_body = acc_body_arr[1];
    float yr = gyro(2);
    float yr_cmd = 0.f;
    _lateral_acceleration_controller.step(
        ay_body,
        _accel_commands.Ay_cmd,
        yr,
        yr_cmd,
        0.010737f,
        0.f,
        0.001f
    );
    float rudder_deflection = 0.f;
    _yaw_damper.step(yr_cmd, yr, speed_body, rudder_deflection);

    float elevator_norm = math::constrain(elevator_deflection / 10.0f, -1.0f, 1.0f);
    float rudder_norm   = math::constrain(rudder_deflection / 10.0f, -1.0f, 1.0f);

    PX4_INFO("Az_cmd: %.3f, Ay_cmd: %.3f, Rudder Deflection: %.3f",
	 (double)_accel_commands.Az_cmd,
	 (double)_accel_commands.Ay_cmd,
	 (double)rudder_deflection);

    // 7) publish outputs
    actuator_servos_s servos{};
    servos.timestamp = hrt_absolute_time();
    servos.timestamp_sample = servos.timestamp;
    servos.control[0] = 0.f;
    servos.control[1] = elevator_norm;
    servos.control[2] = rudder_norm;
    _servos_pub.publish(servos);

    actuator_motors_s motors{};
    motors.timestamp = hrt_absolute_time();
    motors.timestamp_sample = motors.timestamp;
    motors.reversible_flags = 0;
    motors.control[0] = (1400.f - 1000.f) / 1000.f;
    _motors_pub.publish(motors);
}

void InterceptorControl::parameters_update() {}

int InterceptorControl::main(int argc, char *argv[])
{
    if (argc < 2) {
        print_usage("Missing command");
        return 1;
    }
    if (!strcmp(argv[1], "start")) {
        if (_object.load()) {
            PX4_WARN("Already running");
            return 1;
        }
        _object.store(new InterceptorControl());
        if (!_object.load() || !_object.load()->init()) {
            PX4_ERR("Start failed");
            delete _object.load();
            _object.store(nullptr);
            return 1;
        }
        return 0;
    }
    if (!strcmp(argv[1], "stop")) {
        if (!_object.load()) {
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

int InterceptorControl::custom_command(int argc, char *argv[]) { return print_usage(); }
int InterceptorControl::print_usage(const char *reason) {
    PRINT_MODULE_USAGE_NAME("interceptor_control", "controller");
    PRINT_MODULE_USAGE_COMMAND("start");
    PRINT_MODULE_USAGE_COMMAND_DESCR("stop", "Stop the module");
    return 0;
}

extern "C" __EXPORT int interceptor_control_main(int argc, char *argv[]) {
    return InterceptorControl::main(argc, argv);
}
