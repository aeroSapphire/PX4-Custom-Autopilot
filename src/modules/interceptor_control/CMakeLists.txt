px4_add_module(
	MODULE modules__interceptor_control
	MAIN interceptor_control
	SRCS
		interceptor_control.cpp
		interceptor_control.hpp
		pitch_damper_controller.cpp
		roll_damper_controller.cpp
		yaw_damper_controller.cpp
	DEPENDS
		uORB
		px4_work_queue
	)
