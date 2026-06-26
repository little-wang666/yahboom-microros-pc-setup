#!/usr/bin/env bash
set -euo pipefail

WORKSPACE="${HOME}/esp32_microros_ws"
mkdir -p "$WORKSPACE"
cd "$WORKSPACE"

clone_or_update() {
  local url="$1"
  local dir="$2"

  if [ -d "$dir/.git" ]; then
    echo "Updating $dir..."
    git -C "$dir" pull --ff-only
  else
    echo "Cloning $url..."
    git clone "$url" "$dir"
  fi
}

sudo apt update
sudo apt install -y git curl python3-venv python3-pip

clone_or_update "https://github.com/YahboomTechnology/MicroROS-Board.git" "MicroROS-Board"
clone_or_update "https://github.com/YahboomTechnology/Mirco-Ros-Car_VM.git" "Mirco-Ros-Car_VM"
clone_or_update "https://github.com/micro-ROS/micro_ros_espidf_component.git" "micro_ros_espidf_component"
clone_or_update "https://github.com/micro-ROS/micro-ROS-Agent.git" "micro-ROS-Agent"

cat <<'MSG'

ESP32 reference sources are ready under ~/esp32_microros_ws.

This script intentionally does not flash firmware.
Before flashing, confirm the exact Yahboom board model, ESP32 chip,
serial device, Wi-Fi settings, UDP port, and the latest Yahboom tutorial.
MSG
