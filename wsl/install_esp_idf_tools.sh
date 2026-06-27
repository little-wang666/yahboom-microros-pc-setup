#!/usr/bin/env bash
set -euo pipefail

# 这个脚本只在 WSL2 Ubuntu 中安装 ESP-IDF 工具链。
# 它不会烧录 ESP32，也不会修改 Windows 原生环境。
# 默认安装 ESP-IDF release/v5.2；如需换版本，可在运行前设置 ESP_IDF_VERSION。

ESP_ROOT="${HOME}/esp"
ESP_IDF_DIR="${ESP_ROOT}/esp-idf"
ESP_IDF_VERSION="${ESP_IDF_VERSION:-release/v5.2}"

sudo apt update
sudo apt install -y git wget flex bison gperf python3 python3-pip python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

mkdir -p "$ESP_ROOT"

if [ -d "$ESP_IDF_DIR/.git" ]; then
  echo "ESP-IDF already exists, updating ${ESP_IDF_DIR}..."
  git -C "$ESP_IDF_DIR" fetch --all --tags
  git -C "$ESP_IDF_DIR" checkout "$ESP_IDF_VERSION"
  git -C "$ESP_IDF_DIR" pull --ff-only || true
else
  echo "Cloning ESP-IDF ${ESP_IDF_VERSION}..."
  git clone --recursive --branch "$ESP_IDF_VERSION" https://github.com/espressif/esp-idf.git "$ESP_IDF_DIR"
fi

git -C "$ESP_IDF_DIR" submodule update --init --recursive

"$ESP_IDF_DIR/install.sh" esp32s3

cat <<MSG

ESP-IDF tools are installed under:
  ${ESP_IDF_DIR}

Before building firmware in a new terminal, run:
  source ${ESP_IDF_DIR}/export.sh

Then you can inspect the learning firmware skeleton:
  cd "/mnt/d/codex project/microros_pc_setup/esp32_firmware/yahboom_esp32_micro_ros_car"

Do not flash yet until the exact Yahboom board model, serial port,
Wi-Fi settings, and official tutorial version are confirmed.
MSG
