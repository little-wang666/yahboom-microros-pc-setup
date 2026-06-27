$ErrorActionPreference = "Stop"

Write-Host "Checking Ubuntu-22.04 availability..."
wsl.exe -d Ubuntu-22.04 -- bash -lc "cat /etc/os-release"

Write-Host "Installing ROS2 Humble PC environment inside WSL..."
wsl.exe -d Ubuntu-22.04 -- bash -lc 'bash "/mnt/d/codex project/microros_pc_setup/wsl/setup_ros2_humble_pc.sh"'

Write-Host "Preparing ESP32 reference source repositories inside WSL..."
wsl.exe -d Ubuntu-22.04 -- bash -lc 'bash "/mnt/d/codex project/microros_pc_setup/wsl/setup_esp32_micro_ros_sources.sh"'

Write-Host "Installing ESP-IDF tools inside WSL..."
wsl.exe -d Ubuntu-22.04 -- bash -lc 'bash "/mnt/d/codex project/microros_pc_setup/wsl/install_esp_idf_tools.sh"'

Write-Host ""
Write-Host "Environment setup finished."
Write-Host "Open the ROS2 workspace with:"
Write-Host 'wsl.exe -d Ubuntu-22.04 -- bash -lc "cd ~/ros2_ws && code ."'
