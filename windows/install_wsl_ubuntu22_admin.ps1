# Run this script in an Administrator PowerShell window.
$ErrorActionPreference = "Stop"

Write-Host "Checking WSL status..."
wsl.exe --status

Write-Host "Installing WSL with Ubuntu-22.04..."
wsl.exe --install -d Ubuntu-22.04

Write-Host ""
Write-Host "If Windows asks for a reboot, reboot now."
Write-Host "After reboot, open Ubuntu-22.04 once and create your Linux user."
Write-Host "Then run:"
Write-Host 'bash "/mnt/d/codex project/microros_pc_setup/wsl/setup_ros2_humble_pc.sh"'
