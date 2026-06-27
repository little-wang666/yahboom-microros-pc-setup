#!/usr/bin/env bash
set -euo pipefail

if ! grep -q 'VERSION_ID="22.04"' /etc/os-release; then
  echo "This installer expects Ubuntu 22.04. Current /etc/os-release:"
  cat /etc/os-release
  exit 1
fi

if ! grep -qi microsoft /proc/version; then
  echo "Warning: this does not look like WSL. Continuing because Ubuntu 22.04 is confirmed."
fi

echo "Updating apt metadata and installing base tools..."
sudo apt update
sudo apt install -y software-properties-common curl gnupg lsb-release python3-venv python3-pip

echo "Enabling Ubuntu universe repository..."
sudo add-apt-repository -y universe

echo "Installing ROS apt source package from latest official ros-apt-source release..."
sudo apt update
ROS_APT_SOURCE_VERSION="$(curl -s https://api.github.com/repos/ros-infrastructure/ros-apt-source/releases/latest | grep -F '"tag_name"' | awk -F'"' '{print $4}')"
. /etc/os-release
UBUNTU_CODENAME="${UBUNTU_CODENAME:-${VERSION_CODENAME}}"
curl -L -o /tmp/ros2-apt-source.deb "https://github.com/ros-infrastructure/ros-apt-source/releases/download/${ROS_APT_SOURCE_VERSION}/ros2-apt-source_${ROS_APT_SOURCE_VERSION}.${UBUNTU_CODENAME}_all.deb"
sudo dpkg -i /tmp/ros2-apt-source.deb

echo "Upgrading system packages before ROS2 install, as recommended by ROS2 Humble docs..."
sudo apt update
sudo apt upgrade -y

echo "Installing ROS2 Humble desktop, development tools, colcon, and micro-ROS Agent..."
sudo apt install -y ros-humble-desktop ros-dev-tools python3-colcon-common-extensions ros-humble-micro-ros-agent

if ! grep -Fq "source /opt/ros/humble/setup.bash" "$HOME/.bashrc"; then
  echo "source /opt/ros/humble/setup.bash" >> "$HOME/.bashrc"
fi

echo "Creating isolated ROS2 workspace..."
mkdir -p "$HOME/ros2_ws/src" "$HOME/ros2_ws/scripts"
python3 -m venv "$HOME/ros2_ws/.venv"
"$HOME/ros2_ws/.venv/bin/python" -m pip install --upgrade pip

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
find "$SCRIPT_DIR/ros2_ws_src" -mindepth 1 -maxdepth 1 -type d -exec cp -r {} "$HOME/ros2_ws/src/" \;
cp "$SCRIPT_DIR/scripts/start_agent_udp.sh" "$HOME/ros2_ws/scripts/"
cp "$SCRIPT_DIR/scripts/start_agent_serial.sh" "$HOME/ros2_ws/scripts/"
chmod +x "$HOME/ros2_ws/scripts/"*.sh

echo "Building workspace..."
source /opt/ros/humble/setup.bash
cd "$HOME/ros2_ws"
colcon build

echo ""
echo "ROS2 Humble PC environment is ready."
echo "Verify with:"
echo "  source /opt/ros/humble/setup.bash"
echo "  source ~/ros2_ws/install/setup.bash"
echo "  ros2 launch yahboom_pc_control cmd_vel_test.launch.py"
