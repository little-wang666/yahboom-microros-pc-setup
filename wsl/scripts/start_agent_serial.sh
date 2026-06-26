#!/usr/bin/env bash
set -euo pipefail

DEVICE="${1:-/dev/ttyUSB0}"
BAUDRATE="${2:-115200}"
VERBOSITY="${MICRO_ROS_AGENT_VERBOSITY:-6}"

source /opt/ros/humble/setup.bash
exec ros2 run micro_ros_agent micro_ros_agent serial --dev "$DEVICE" -b "$BAUDRATE" -v"$VERBOSITY"
