import subprocess
import os
import time

# Set environment variables for PX4
env = os.environ.copy()
env["PX4_SIM_HOST_ADDR"] = "192.168.1.79"
env["PX4_SIM_MODEL"] = "gazebo-classic_plane"

# PX4 command
px4_cmd = [
    "./build/px4_sitl_default/bin/px4",
    "-s", "etc/init.d-posix/rcS"
]

# Set your PX4 autopilot root directory here
px4_root_dir = "/home/asad/PX4-Custom-Autopilot"

# Launch PX4 SITL from the correct working directory
px4_process = subprocess.Popen(px4_cmd, env=env, cwd=px4_root_dir)

# Wait for PX4 SITL to stabilize
time.sleep(5)

# Launch simulink_to_mavlink.py in a new terminal
subprocess.Popen([
    "gnome-terminal",
    "--", "bash", "-c",
    "python3 simulink_to_mavlink.py; exec bash"
])

# Wait for PX4 to exit
px4_process.wait()

