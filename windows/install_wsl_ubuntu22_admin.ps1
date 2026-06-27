# Run this script in an Administrator PowerShell window.
$ErrorActionPreference = "Stop"

$principal = New-Object Security.Principal.WindowsPrincipal([Security.Principal.WindowsIdentity]::GetCurrent())
if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    throw "Please run this script from an Administrator PowerShell window."
}

Write-Host "Enabling Windows features required by WSL2..."
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart

Write-Host "Checking WSL status after feature enable..."
wsl.exe --status

Write-Host "Installing WSL with Ubuntu-22.04..."
wsl.exe --install -d Ubuntu-22.04

Write-Host "Setting WSL default version to 2..."
wsl.exe --set-default-version 2

Write-Host ""
Write-Host "If Windows asks for a reboot, reboot now."
Write-Host "After reboot, open Ubuntu-22.04 once and create your Linux user."
Write-Host "Then run:"
Write-Host 'powershell -ExecutionPolicy Bypass -File "D:\codex project\microros_pc_setup\windows\run_wsl_environment_setup.ps1"'
