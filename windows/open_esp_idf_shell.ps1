param(
    [string]$ProjectPath = "D:\codex project\microros_pc_setup\esp32_firmware\yahboom_esp32_micro_ros_car"
)

# 打开一个新的 ESP-IDF PowerShell，并停留在 ESP32 工程目录。
# 使用方式：
#   powershell -ExecutionPolicy Bypass -File "D:\codex project\microros_pc_setup\windows\open_esp_idf_shell.ps1"

$quotedProfile = "C:\Espressif\tools\Microsoft.v5.2.5.PowerShell_profile.ps1"
$command = @"
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force
. '$quotedProfile'
`$env:TEMP = 'D:\Espressif\tmp'
`$env:TMP = 'D:\Espressif\tmp'
`$env:IDF_CCACHE_ENABLE = '0'
`$env:CCACHE_DISABLE = '1'
`$env:PATH = ((`$env:PATH -split ';') | Where-Object { `$_ -notmatch '\\ccache\\' }) -join ';'
Set-Location -LiteralPath '$ProjectPath'
Write-Host ''
Write-Host 'ESP-IDF shell ready. Common commands:'
Write-Host '  idf.py build'
Write-Host '  idf.py -p COMx flash monitor'
"@

Start-Process powershell -ArgumentList @(
    "-NoExit",
    "-ExecutionPolicy", "Bypass",
    "-Command", $command
) -WindowStyle Normal
