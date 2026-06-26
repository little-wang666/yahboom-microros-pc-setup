#!/usr/bin/env bash
set -euo pipefail

PORT="${1:-8888}"
VERBOSITY="${MICRO_ROS_AGENT_VERBOSITY:-6}"

source /opt/ros/humble/setup.bash
exec ros2 run micro_ros_agent micro_ros_agent udp4 --port "$PORT" -v"$VERBOSITY"
