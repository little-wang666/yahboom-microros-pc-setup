param(
    [string]$ProjectPath = "D:\codex project\microros_pc_setup\esp32_firmware\yahboom_esp32_micro_ros_car",
    [string]$Target = "esp32s3"
)

# 本脚本只在当前 PowerShell 进程内激活 ESP-IDF，不修改系统全局环境变量。
# ccache 在中文 Windows 用户路径下可能触发编码问题，所以这里主动关闭。

$ErrorActionPreference = "Stop"

Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force

$profileScript = "C:\Espressif\tools\Microsoft.v5.2.5.PowerShell_profile.ps1"
if (-not (Test-Path -LiteralPath $profileScript)) {
    throw "ESP-IDF profile not found: $profileScript"
}

New-Item -ItemType Directory -Force -Path "D:\Espressif\tmp" | Out-Null

. $profileScript

$env:TEMP = "D:\Espressif\tmp"
$env:TMP = "D:\Espressif\tmp"
$env:IDF_CCACHE_ENABLE = "0"
$env:CCACHE_DISABLE = "1"
$env:PATH = (($env:PATH -split ";") | Where-Object { $_ -notmatch "\\ccache\\" }) -join ";"

idf.py -C $ProjectPath set-target $Target build
